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
      "async_hooks": false,
      "aws-sdk": false,
      "child_process": false,
      "@swc/core": false,
      "dgram": false,
      "dns": false,
      "esbuild": false,
      "fsevents": false,
      "inspector": false,
      "mock-aws-s3": false,
      "module": false,
      "net": false,
      "nock": false,
      "node-pre-gyp": false,
      "tls": false,
      "uglify-js": false,
      "worker_threads": false
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
