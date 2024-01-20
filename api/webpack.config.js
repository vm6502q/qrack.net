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
        include: [path.join(__dirname)],
        test: /\.(js|jsx)$/,
        loader: 'babel-loader'
      },
      {
        test: /\.(wasm)$/,
        loader: 'file-loader',
        type: 'javascript/auto'
      }
    ]
  },
  resolveLoader: {
    modules: [
      __dirname + '/node_modules'
    ]
  }
}
