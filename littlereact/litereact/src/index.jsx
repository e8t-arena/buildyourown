import React from 'react'
import ReactDOM from 'react-dom'

function createElement(type, props, ...children) {
	return {
		type, 
		props: {
			...props,
			children
		}
	}
}
ReactDOM.render(
	<div>Works</div>,
	document.getElementById('root')
)
