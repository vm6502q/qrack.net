import React from 'react'
import ReactDOM from 'react-dom'
import './node_modules/bootstrap/dist/css/bootstrap.min.css'

import MainRouter from './MainRouter'

ReactDOM.render(
  <React.StrictMode>
    <MainRouter />
  </React.StrictMode>,
  document.getElementById('root')
)
