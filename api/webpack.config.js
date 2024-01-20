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
        test: /Qrack\.(js|jsx)$/,
        loader: 'exports-loader',
        options: {
          exports: 'Qrack'
        }
      },
      {
        test: /Qrack\.(wasm)$/,
        loader: 'file-loader',
        type: 'javascript/auto'
      }
    ]
  }
}
