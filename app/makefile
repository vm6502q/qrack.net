all: qft

qft:
	emcc -O3 -std=c++17 -msimd128 -msse -msse2 -msse3 -Wall -Werror --bind bindings/QrackBindings.cpp -Icpp/ cpp/*.cpp -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=Qrack -s ENVIRONMENT="worker" -s STACK_SIZE=8MB -s INITIAL_HEAP=8MB -s NO_DISABLE_EXCEPTION_CATCHING -Lcpp/lib -lqrack -o Qrack.js

