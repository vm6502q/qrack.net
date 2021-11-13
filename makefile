all: qft

qft:
	emcc --bind bindings/SampleBindings.cpp -Icpp/ -I/usr/local/include cpp/*.cpp -s WASM=1 -s MODULARIZE=1 -Lcpp/lib -lqrack -lpthread -o Sample.js
