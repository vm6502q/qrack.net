all: qft

qft:
	emcc -O3 -std=c++11 -pthread -Wall -Werror --bind bindings/QrackBindings.cpp -Icpp/ cpp/*.cpp -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=Qrack -s ENVIRONMENT="worker" -s TOTAL_MEMORY=1024MB -Lcpp/lib -lqrack -lpthread -o Qrack.js

