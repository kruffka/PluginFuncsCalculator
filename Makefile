CC=gcc
CFLAGS=
PLUGINS_DIR="./plugins"

calculator: calculator.c libadd.so.1
	gcc calculator.c -L. -o calc -ldl

libadd.so.1: add.o
	gcc -shared add.o -o libadd.so.1

add.o: plugins/add.c
	gcc plugins/add.c -c -fPIC

clean:
	rm -rf *.o *.so.* calc