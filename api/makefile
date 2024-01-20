all: qft

qft:
	emcc -O3 -std=c++14 -msimd128 -msse -msse2 -msse3 -Wall -Werror --bind bindings/QrackBindings.cpp -Icpp/ cpp/*.cpp -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=Qrack -s TOTAL_MEMORY=4096MB -s NO_DISABLE_EXCEPTION_CATCHING -s ALLOW_MEMORY_GROWTH -Lcpp/lib -lqrack_wasm -lqrack -o Qrack.js

