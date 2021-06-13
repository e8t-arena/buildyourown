const TEXT_ELEMENT = 'TEXT_ELEMENT'  // Symbol
const UPDATE_TAG = 'UPDATE'
const ADD_TAG = 'ADD'
const DELETE_TAG = 'DELETE'

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
let currentRoot = null 
let deletions = null

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

function performUnitOfWork(fiber) {
  // add node
  if(!fiber.node) {
    fiber.node = createNode(fiber)
  }

  // if(fiber.parent) {
  //   fiber.parent.node.appendChild(fiber.node)
  // }

  // create new fiber/children
  const children = fiber.props.children 
  reconcileChildren(fiber, children)

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

function reconcileChildren(wipFiber, children) {
  let index = 0
  // compare child
  let oldFiber = wipFiber.alternate && wipFiber.alternate.child
  let prevSibling = null 

  while (index < children.length || oldFiber != null) {
    const child = children[index]
    // let newFiber = null
    // const newFiber = {
    //   type: child.type,
    //   props: child.props,
    //   parent: fiber,
    //   node: null
    // }

    // compare oldFiber to child
    const sameType = 
      oldFiber &&
      child &&
      child.type == oldFiber.type // 'div' == 'div'
    
    if(sameType) {
      // update oldFiber node
      newFiber = {
        type: oldFiber.type,
        props: child.props,
        node: oldFiber.node,
        parent: wipFiber,
        // 下次 render 前替换为 current
        alternate: oldFiber,
        effectTag: UPDATE_TAG,
      }
    }

    if(child && !sameType) {
      // add new child node
      newFiber = {
        type: child.type,
        props: child.props,
        node: null,
        parent: wipFiber,
        alternate: null,
        effectTag: ADD_TAG,
      }
    }

    if(oldFiber && !sameType) {
      // delete oldFiber node
      oldFiber.effectTag = DELETE_TAG
      // 先保存下来，commitRoot 中删除
      deletions.push(oldFiber)
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
}

requestIdleCallback(workLoop)

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
  log('commit root:')
  log(wipRoot)
  deletions.forEach(commitWork)
  commitWork(wipRoot.child)
  currentRoot = wipRoot
  wipRoot = null
}

const isEvent = key => key.startsWith('on')
const isProperty = key => key !== 'children' && !isEvent(key)
const isNew = (prev, next) => key => prev[key] !== next[key]
const isGone = (prev, next) => key => !(key in next)

function updateNode(node, prevProps, nextProps) {
  // Remove old or changed event listeners
  // 删除变化的 event (isGone or changed(new / update))
  Object.keys(prevProps)
    .filter(isEvent)
    .filter(
      key =>
        // 
        !(key in nextProps) ||
        isNew(prevProps, nextProps)(key)
    ).forEach(name => {
      // onclick -> click
      const eventType = name.toLowerCase().substring(2)
      node.removeEventListener(
        eventType,
        prevProps[name]
      )
    })

  // Remove old properties
  Object.keys(prevProps)
    .filter(isProperty)
    .filter(isGone(prevProps, nextProps))
    .forEach(name => {
      node[name] = ''
    })
  
  // Set new or changed properties
  Object.keys(nextProps)
    .filter(isProperty)
    .filter(isNew(prevProps, nextProps))
    .forEach(name => {
      node[name] = nextProps[name]
    })

  // Add event listeners
  Object.keys(nextProps)
    .filter(isEvent)
    .filter(isNew(prevProps, nextProps))
    .forEach(name => {
      const eventType = name
        .toLowerCase()
        .substring(2)
      node.addEventListener(
        eventType,
        nextProps[name]
      )
    })
}

// 递归添加 dom 

function commitWork(fiber) {
  if(!fiber) return 
  log('commit child:')
  log(fiber.node)

  const nodeParent = fiber.parent.node 
  if(fiber.effectTag === ADD_TAG && fiber.node != null) {
    nodeParent.appendChild(fiber.node)
  } else if (fiber.effectTag === UPDATE_TAG && fiber.node != null) {
    updateNode(
      fiber.node,
      // oldProps
      fiber.alternate.props,
      // newProps
      fiber.props
    )
  } else if (fiber.effectTag === DELETE_TAG) {
    nodeParent.removeChild(fiber.node)
  }
  // fiber.parent.node.appendChild(fiber.node)
  commitWork(fiber.child)
  commitWork(fiber.sibling)
}

function render(element, container) {
  wipRoot = {
    node: container,
    props: {
      children: [element]
    },
    alternate: currentRoot
  }
  deletions = []
  nextUnitOfWork = wipRoot
}
/* library */

const Liteact = {
	createElement,
	render,
}

export default Liteact