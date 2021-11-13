# qrack.net

This is the future home of the OSS qrack.net codebase, to demonstrate simple [vm6502q/qrack](https://github.com/vm6502q/qrack) quantum computer simulator library benchmarks executed in any web browser. For now, it contains a Qrack quantum Fourier transform (QFT) bound method, wrapped in the example code provided by the below reference, with thanks. To build this, you must have CPU-only Qrack installed, built from source, with the following build options:

```sh
qrack/_build$ cmake -DENABLE_OPENCL=OFF -DENABLE_COMPLEX_X2=OFF -DENABLE_RDRAND=OFF ..
qrack/_build$ make all -j8
qrack/_build$ sudo make install
```

Below is the original WASM example reference for everything else in this project:

## C++ To WebAssembly With React From Scratch!

The associated medium post is available at https://guptanikhil.medium.com/using-webassembly-with-react-from-scratch-fba8cdfa5118.
