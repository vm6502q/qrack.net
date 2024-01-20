const path = require('path')

module.exports = {
  target: 'node',
  entry: './index.js',
  mode: 'production',
  output: {
    path: path.resolve(__dirname, 'build'),
    filename: 'app.bundle.js'
  },
  module: {
    rules: [
      {
        test: path.resolve(__dirname, 'Qrack.js'),
        loader: 'exports-loader',
        options: {
          exports: 'Qrack'
        }
      },
      {
        test: path.resolve(__dirname, 'Qrack.wasm'),
        loader: 'file-loader',
        type: 'javascript/auto'
      }
    ]
  }
}
