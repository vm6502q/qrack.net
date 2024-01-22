import React from 'react'
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom'
import App from './App'

export default function MainRouter () {
  return (
    <Router>
      <Routes>
        <Route exact path="/" element={<App />} />
      </Routes>
    </Router>
  )
}