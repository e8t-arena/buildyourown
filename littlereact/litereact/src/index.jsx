import React from 'react'
import ReactDOM from 'react-dom'
import Liteact from './liteact'


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
