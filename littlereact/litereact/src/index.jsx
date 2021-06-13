import React from 'react'
import ReactDOM from 'react-dom'
import Liteact from './liteact'

let element = Liteact.createElement(
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

function App(props) {
	return Liteact.createElement(
		'h1',
		null,
		'Function Component',
		props.name
	)
}

element = Liteact.createElement(
	App,
	{
		name: "Mr. John"
	}
)

Liteact.render(
	// <div>Works</div>,
	element,
	document.getElementById('root')
)
