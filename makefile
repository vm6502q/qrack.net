all: qft

qft:
	emcc -O3 -std=c++11 -Wall -Werror --bind bindings/SampleBindings.cpp -Icpp/ cpp/*.cpp -s ASSERTIONS=1 -s WASM=1 -s MODULARIZE=1 -L${EMSDK}/upstream/emscripten/cache/sysroot/lib -lqrack -lpthread -o Sample.js
