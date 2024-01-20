const path = require('path')
const NodePolyfillPlugin = require("node-polyfill-webpack-plugin")

module.exports = {
  entry: path.resolve(__dirname, 'index.js'),
  externals: {
    fs: 'empty'
  },
  mode: 'production',
  resolve: {
    extensions: [".ts", ".js"],
    fallback: {
      "aws-sdk": false,
      "child_process": false,
      "dns": false,
      "mock-aws-s3": false,
      "net": false,
      "nock": false,
      "node-pre-gyp": false,
      "tls": false
    }
  },
  module: {
    rules: [
      {
        include: [path.join(__dirname)],
        test: /\.(js|jsx)$/,
        loader: 'babel-loader',
        options: {
          presets: ['@babel/preset-react']
        }
      },
      {
        test: /\.(wasm)$/,
        loader: 'file-loader',
        type: 'javascript/auto'
      },
      {
        test: /\.css$/i,
        use: ['style-loader', 'css-loader']
      },
      {
        test: /\.(png|svg|jpg|jpeg|gif)$/i,
        type: 'asset/resource'
      }
    ]
  },
  devServer: {
    headers: {
      'Cross-Origin-Embedder-Policy': 'require-corp',
      'Cross-Origin-Opener-Policy': 'same-origin'
    },
    allowedHosts: 'all'
  },
  plugins: [
    new NodePolyfillPlugin()
  ]
}
