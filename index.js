import React from 'react'
import ReactDOM from 'react-dom'

import Qrack from './Qrack.js'
import QrackWASM from './Qrack.wasm'

const qrack = Qrack({
  locateFile: () => {
    return QrackWASM
  }
})

qrack.then((core) => {
  console.log(core.qft_perm(8, 0))
})

ReactDOM.render(
  <div>
    <h1>Using WebAssembly with React From Scratch!</h1>
  </div>,
  document.getElementById('root')
)
