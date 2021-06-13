import React from 'react'
import ReactDOM from 'react-dom'

const TEXT_ELEMENT = 'TEXT_ELEMENT'  // Symbol

const log = console.log

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

function render(element, container) {
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
	container.appendChild(node)
}

const Liteact = {
	createElement,
	render,
}

const element = Liteact.createElement(
	"div",
	{id: "foo"},
	Liteact.createElement("a", {href: "https://dev.to", style: "color: blue;font-size: 5rem;", target: "_blank"}, "Dev.to"),
	Liteact.createElement("b")
)

console.log(element)

// const old = (
// 	<div>Works</div>
// )
// console.log(old)

/** @jsx Liteact.createElement */
const elementJSX = (
	<div id="foo">
		<a>bar</a>
	</div>
)

Liteact.render(
	// <div>Works</div>,
	element,
	document.getElementById('root')
)
