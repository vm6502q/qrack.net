import './App.css'
import React, { useState, useEffect } from 'react'

import BenchmarkChart from './components/BenchmarkChart.js'

import Qrack from './Qrack.js'
import QrackWASM from './Qrack.wasm'

import logo from './assets/img/qrack_logo.png'

const qrack = Qrack({
  locateFile: () => {
    return QrackWASM
  }
})

function App () {
  const [benchmarkData, setBenchmarkData] = useState([])
  const [fullData, setFullData] = useState([])

  function handleQftDispatch (length, maxLength) {
    qrack.then((core) => {
      handleQftIteration(core, length, maxLength)
    })
  }

  function handleQftIteration (core, length, maxLength) {
    const start = new Date().getTime()
    const mResult = core.qft_u3(length)
    const end = new Date().getTime()

    const nBenchmarkData = benchmarkData
    nBenchmarkData.push({ method: '', label: length, value: (end - start) })

    setBenchmarkData(nBenchmarkData)

    if (length < maxLength) {
      handleQftIteration(core, length + 1, maxLength)
    } else {
      setFullData(nBenchmarkData)
    }
  }

  useEffect(() => {
    handleQftDispatch(1, 16)
  }, [])

  const [width, setWidth] = useState(window.innerWidth)

  useEffect(() => {
    function handleResize () {
      setWidth(window.innerWidth)
    }
    window.addEventListener('resize', handleResize)
  })

  return (
    <div>
      <div className='container text-center'>
        <h1 className='app-title'>WebAssembly Qrack With React.js From Scratch!</h1>
        <h4>QFT (including random unitary initialization, 1 sample)</h4>
      </div>
      <BenchmarkChart data={fullData} width={width} height={400} xLabel='Qubits' xType='number' yLabel='Time (ms)' yType='number' />
      <div className='container text-center'>
        <div>
          <p>These benchmarks for the <b>open source</b> <a href='https://github.com/vm6502q/qrack'>vm6502q/qrack</a> quantum computer simulator library <b>were just run in your browser!</b></p>
          <p>Qrack can compile for browsers as WebAssembly. In the time this page took to load, it served you a copy of the Qrack library built for the purpose, ran a quantum Fourier transform, and graphed the timing results! <a href='https://github.com/WrathfulSpatula/qrack.net'>This page is also open source, as a template.</a></p>
          <p>Qrack also supports GPU workloads with OpenCL, up to multi-device environments. It's the same code to compile!</p>
        </div>
        <div className='qrack-logo'>
          <b>
            <h4>Powered by Qrack</h4>
            <a href='https://github.com/vm6502q/qrack'><img src={logo} width='192px' /></a>
            <h4 className='qrack-logo-bottom-text'>You rock!</h4>
          </b>
        </div>
      </div>
    </div>
  )
}

export default App
