import React from 'react'
import ReactDOM from 'react-dom'
import Liteact from './liteact'

let element

// element = Liteact.createElement(
// 	"div",
// 	{id: "foo"},
// 	Liteact.createElement("a", {href: "https://dev.to", style: "color: blue;font-size: 5rem;", target: "_blank"}, "Dev.to"),
// 	Liteact.createElement("b")
// )

// console.log(element)

// const old = (
// 	<div>Works</div>
// )
// console.log(old)

// const elementJSX = (
// 	<div id="foo">
// 		<a>bar</a>
// 	</div>
// )

function App({name, children}) {
	// return Liteact.createElement(
	// 	'div',
	// 	null,
	// 	'Function Component',
	// 	props.name
	// )
	return <div>
		<h3>{name}</h3>
		{/* {children} */}
		<Counter />
	</div>
}

function Counter() {
	const [state, setState] = Liteact.useState(0)
	return (
		<div>
			Count: {state}
			<button onClick={()=>setState(n => n+1)}>
				UP
			</button>
			<button onClick={()=>setState(n => n-1)}>
				DOWN
			</button>
		</div>
	)
}

// element = Liteact.createElement(
// 	App,
// 	{
// 		name: "Mr. John"
// 	},
// 	<Counter />
// )

/** @jsx Liteact.createElement */
element = () => (
	<App name={"Mr. John"}>
		{/* <Counter /> */}
	</App>
)

Liteact.render(
	// <div>Works</div>,
	element(),
	document.getElementById('root')
)
