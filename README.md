# qrack.net

This is the source for https://qrack.net. To build the [vm6502q/qrack](https://github.com/vm6502q/qrack) library as WASM, the following build command is suggested, in an active `emsdk` environment:

```sh
user@domain:~/qrack.net$ emcmake cmake -DENABLE_OPENCL=OFF -DENABLE_QUNIT_CPU_PARALLEL=OFF -DENABLE_COMPLEX_X2=OFF -DENABLE_RDRAND=OFF -DUINTPOW=5 -DENABLE_PTHREAD=OFF ..
```

The website code is based upon the following example reference, with thanks: 

## C++ To WebAssembly With React From Scratch!

The associated medium post is available at https://guptanikhil.medium.com/using-webassembly-with-react-from-scratch-fba8cdfa5118.
