all: qft

qft:
	emcc -O3 -std=c++11 -Wall -Werror -s USE_BOOST_HEADERS=1 --bind bindings/QrackBindings.cpp -Icpp/ cpp/*.cpp -s WASM=1 -s MODULARIZE=1 -s EXPORT_NAME=Qrack -s ENVIRONMENT="worker" -s TOTAL_MEMORY=4095MB -s USE_BOOST_HEADERS=1 -Lcpp/lib -lqrack -o Qrack.js

