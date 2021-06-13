const TEXT_ELEMENT = 'TEXT_ELEMENT'  // Symbol
const UPDATE_TAG = 'UPDATE'
const ADD_TAG = 'ADD'
const DELETE_TAG = 'DELETE'

const log = console.log

/* create node */

function createElement(type, props, ...children) {
  log(children)
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
  // log('performUnitOfWork: ', fiber)
  const isFunctionComponent = fiber.type instanceof Function 

  if(isFunctionComponent) {
    updateFunctionComponent(fiber)
  } else {
    updateHostComponent(fiber)
  }

  // add node
  // if(!fiber.node) {
  //   fiber.node = createNode(fiber)
  // }

  // if(fiber.parent) {
  //   fiber.parent.node.appendChild(fiber.node)
  // }

  // create new fiber/children
  // const children = fiber.props.children 
  // reconcileChildren(fiber, children)

  // return next unit of work

  // return child
  if(fiber.child) {
    return fiber.child 
  }
  let nextFiber = fiber
  // Preorder (Root, Left, Right) Traversal
  while(nextFiber) {
    log('while: ', nextFiber)
    if(nextFiber.sibling) {
      // return sibling
      return nextFiber.sibling
    }
    // return parent -> return parent.sibling
    nextFiber = nextFiber.parent
  }
}

// 在 useState 中使用
let wipFiber = null
let hookIndex = null 

function updateFunctionComponent(fiber) {
  wipFiber = fiber
  // keep track of the current hook index.
  hookIndex = 0
  // support 多次调用 useState
  wipFiber.hooks = []

  log('function: ', fiber)
  // 调用 Function Component 的 function 返回 node
  const children = [fiber.type(fiber.props)]
  reconcileChildren(fiber, children)
}

function useState(initial) {
  // 前一次渲染的 hook
  const oldHook = 
    wipFiber.alternate &&
    wipFiber.alternate.hooks &&
    wipFiber.alternate.hooks[hookIndex]
  const hook = {
    state: oldHook ? oldHook.state : initial,
    // return a function to update the state
    queue: []
  }

  const actions = oldHook ? oldHook.queue : []
  // run actions
  actions.forEach(action => {
    hook.state = action(hook.state)
  })
  const setState = action => {
    // push that action to a queue we added to the hook
    hook.queue.push(action)
    wipRoot = {
      node: currentRoot.node,
      props: currentRoot.props,
      alternate: currentRoot
    }
    // 更新数据后渲染
    nextUnitOfWork = wipRoot
    deletions = []
  }
  // wipFiber.hooks.push[hook]
  wipFiber.hooks.push(hook)
  hookIndex++
  return [hook.state, setState]
}

function updateHostComponent(fiber) {
  log('host: ', fiber)
  if(!fiber.node) {
    fiber.node = createNode(fiber)
  }
  const children = fiber.props.children 
  reconcileChildren(fiber, children)
}

function reconcileChildren(wipFiber, children) {
  let index = 0
  // compare child
  let oldFiber = wipFiber.alternate && wipFiber.alternate.child
  let prevSibling = null 

  while (index < children.length || oldFiber != null) {
    const child = children[index]
    console.log('child: ', child)
    let newFiber = null
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

    if (oldFiber) {
      oldFiber = oldFiber.sibling
    }

    if(index === 0) {
      // 设置 child
      // fiber.child = newFiber
      wipFiber.child = newFiber
    } else if (child) {
      // prevSibling 是前一个 child 
      // 设置 sibling
      prevSibling.sibling = newFiber
    }
    prevSibling = newFiber // 更新 prevSibling
    index++
  }
}

requestIdleCallback(workLoop)

/* render */

function createNode(fiber) {
  log(fiber)
  log(fiber.props)

	const node = fiber.type === TEXT_ELEMENT
		? document.createTextNode('')
		// ? document.createTextNode(fiber.props.nodeValue)
		: document.createElement(fiber.type)
	
	// Object.keys(fiber.props)
	// 	.filter(key => key !== 'children')
	// 	.forEach(name => {
	// 		node[name] = fiber.props[name]
	// 	})
	
	// render children

	// fiber.props.children.forEach(child => 
	// 	render(child, node)
	// )

  updateNode(node, {}, fiber.props)

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

  let nodeParentFiber = fiber.parent
  while(!nodeParentFiber.node) {
    nodeParentFiber = nodeParentFiber.parent
  }
  const nodeParent = nodeParentFiber.node 
  log('nodeParent: ', nodeParent)

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
    // nodeParent.removeChild(fiber.node)
    commitDeletion(fiber, nodeParent)
  }
  // fiber.parent.node.appendChild(fiber.node)
  commitWork(fiber.child)
  commitWork(fiber.sibling)
}

function commitDeletion(fiber, nodeParent) {
  if(fiber.node) {
    nodeParent.removeChild(fiber.node)
  } else {
    commitDeletion(fiber.child, nodeParent)
  }
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
  useState,
}

export default Liteact