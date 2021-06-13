import React from 'react'
import ReactDOM from 'react-dom'

const TEXT_ELEMENT = 'TEXT_ELEMENT'  // Symbol

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

const Liteact = {
	createElement
}

const element = Liteact.createElement(
	"div",
	{id: "foo"},
	Liteact.createElement("a", null, "bar"),
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

ReactDOM.render(
	// <div>Works</div>,
	element,
	document.getElementById('root')
)
