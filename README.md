# qrack.net

This is the source for https://qrack.net. To build the [vm6502q/qrack](https://github.com/vm6502q/qrack) library as WASM, the following build command is suggested, in an active `emsdk` environment:

```sh
emcmake cmake -DENABLE_OPENCL=OFF -DENABLE_QUNIT_CPU_PARALLEL=OFF -DENABLE_RDRAND=OFF -DUINTPOW=5 -DENABLE_PTHREAD=OFF ..
```

The library is already built in this example. To recompile the WASM, just run `make all` in the project root directory.

This example provides a pre-built copy of the [Qrack v7 API](https://github.com/vm6502q/qrack/tree/v7_api), which will be released as the main head of the Qrack repository on 1/1/2022.

The website code is based upon the following example reference, with thanks: 

## C++ To WebAssembly With React From Scratch!

The associated medium post is available at https://guptanikhil.medium.com/using-webassembly-with-react-from-scratch-fba8cdfa5118.
