all:
	g++ src/*.cpp -g -Wall -Wextra -std=c++17 -o Compiler.out

clean:
	rm Compiler
