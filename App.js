import './App.css'
import React from 'react'

import BenchmarkChart from './components/BenchmarkChart.js'

import Qrack from './Qrack.js'
import QrackWASM from './Qrack.wasm'

import logo from './assets/img/qrack_logo.png'

const qrack = Qrack({
  locateFile: () => {
    return QrackWASM
  }
})

class App extends React.Component {
  constructor (props) {
    super(props)
    this.state = {
      benchmarkData: [],
      fullData: []
    }

    this.handleQftDispatch = this.handleQftDispatch.bind(this)
    this.handleQftIteration = this.handleQftIteration.bind(this)
  }

  componentDidMount () {
    this.handleQftDispatch(1, 12)
  }

  handleQftDispatch (length, maxLength) {
    qrack.then((core) => {
      this.handleQftIteration(core, length, maxLength)
    })
  }

  handleQftIteration (core, length, maxLength) {
    const start = new Date().getTime()
    const mResult = core.qft_u3(length, 0)
    const end = new Date().getTime()

    const nBenchmarkData = this.state.benchmarkData
    nBenchmarkData.push({ method: '', label: length, value: (end - start) })

    this.setState({ benchmarkData: nBenchmarkData })

    if (length < maxLength) {
      this.handleQftIteration(core, length + 1, maxLength)
    } else {
      this.setState({ fullData: nBenchmarkData })
    }
  }

  render () {
    return (
      <div className='container text-center'>
        <h1>WebAssembly Qrack with React From Scratch!</h1>
        <h4>QFT (including random unitary initialization, 1 sample)</h4>
        <BenchmarkChart data={this.state.fullData} width={1000} height={400} xLabel='Qubits' xType='number' yLabel='Time (ms)' yType='number' />
        <div>
          <p>These benchmarks for the <b>open source</b> <a href='https://github.com/vm6502q/qrack'>vm6502q/qrack</a> quantum computer simulator library <b>were just run in your browser!</b></p>
          <p>Qrack can compile for browsers with WebAssembly. In the time this page took to load, it served you a copy of the full Qrack library built for the purpose, ran a quantum Fourier transform, and graphed the timing results! <a href='https://github.com/WrathfulSpatula/qrack.net'>This page is also open source, as a template.</a></p>
          <p>Qrack also supports GPU workloads with OpenCL, up to multi-device environments. It's the same code to compile!</p>
        </div>
        <div className='qrack-logo'>
          <b>
            <h4>Powered by Qrack</h4>
            <img src={logo} width='192px' />
            <h4 className='qrack-logo-bottom-text'>You rock!</h4>
          </b>
        </div>
      </div>
    )
  }
}

export default App
