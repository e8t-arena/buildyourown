const TEXT_ELEMENT = 'TEXT_ELEMENT'  // Symbol

const log = console.log

/* create node */

function createElement(type, props, ...children) {
	return {
		type, 
		props: {
			...props,
			children: children.map(child => 
				typeof child === 'object'
				? child 
				: createTextElement(child)
			)
		}
	}
}

function createTextElement(text) {
	return {
		type: TEXT_ELEMENT,
		props: {
			nodeValue: text,
			children: []
		}
	}
}

/* Concurrent Mode */

// Experimental
// https://developer.mozilla.org/en-US/docs/Web/API/Window/requestIdleCallback

let nextUnitOfWork = null 
let wipRoot = null 

function workLoop(deadline) {
  let shouldYield = false 
  while(nextUnitOfWork && !shouldYield) {
    nextUnitOfWork = performUnitOfWork(
      nextUnitOfWork
    )
    shouldYield = deadline.timeRemaining() < 1
  }

  if(!nextUnitOfWork && wipRoot) {
    commitRoot()
  }

  requestIdleCallback(workLoop)
}

requestIdleCallback(workLoop)

function performUnitOfWork(fiber) {
  // add node
  if(!fiber.node) {
    fiber.node = createNode(fiber)
  }
  // if(fiber.parent) {
  //   fiber.parent.node.appendChild(fiber.node)
  // }

  // create new fiber
  const children = fiber.props.children 
  let index = 0
  let prevSibling = null 

  while (index < children.length) {
    const child = children[index]
    const newFiber = {
      type: child.type,
      props: child.props,
      parent: fiber,
      node: null
    }
    if(index === 0) {
      // 设置 child
      fiber.child = newFiber
    } else {
      // prevSibling 是前一个 child 
      // 设置 sibling
      prevSibling.prevSibling = newFiber
    }
    prevSibling = newFiber // 更新 prevSibling
    index++
  }

  // return next unit of work

  // return child
  if(fiber.child) {
    return fiber.child 
  }
  let nextFiber = fiber
  // Preorder (Root, Left, Right) Traversal
  while(nextFiber) {
    if(nextFiber.sibling) {
      // return sibling
      return nextFiber.sibling
    }
    // return parent -> return parent.sibling
    nextFiber = nextFiber.parent
  }
}

/* render */

function createNode(fiber) {
	const node = element.type === TEXT_ELEMENT
		? document.createTextNode(element.props.nodeValue)
		: document.createElement(element.type)
	
	Object.keys(element.props)
		.filter(key => key !== 'children')
		.forEach(name => {
			node[name] = element.props[name]
		})
	
	// render children
	element.props.children.forEach(child => 
		render(child, node)
	)

	log(node)
	// container.appendChild(node)
  return node
}

// 渲染 wipRoot 

function commitRoot() {
  commitWork(wipRoot.child)
  wipRoot = null
}

// 递归添加 dom 

function commitWork(fiber) {
  if(!fiber) return 
  const nodeParent = fiber.parent.node 
  nodeParent.appendChild(fiber.node)
  // fiber.parent.node.appendChild(fiber.node)
  commitWork(fiber.child)
  commitWork(fiber.sibling)
}


function render(element, container) {
  wipRoot = {
    node: container,
    props: {
      children: [element]
    }
  }
  nextUnitOfWork = wipRoot
}
/* library */

const Liteact = {
	createElement,
	render,
}

export default Liteact