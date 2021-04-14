build:
	g++ -Wall -fPIC -std=c++14 -c translator.cpp -o translator.o `pkg-config --cflags freetype2 cairo` `pkg-config --libs freetype2 cairo`
	g++ -Wall -fPIC -shared -o translator.so translator.o `pkg-config --cflags freetype2 cairo` `pkg-config --libs freetype2 cairo`
	rm translator.o
	gcc -Wall -Wextra -rdynamic -g main.c -o runePrint `pkg-config --libs --cflags gtk+-3.0` -ldl
	@echo "Done"