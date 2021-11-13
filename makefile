all: qft

qft:
	emcc -O3 -std=c++11 -pthread -Wall -Werror qrack_benchmarks.cpp -s WASM=1 --shell-file html_template/shell_minimal.html -Llib -lqrack -lpthread -o index.html

