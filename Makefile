CC=gcc
CFLAGS=
PLUGINS_DIR="./plugins"


calc: calculator.c
	gcc calculator.c -L. -o calc -ldl -g

plugins: libadd.so.1 libdivide.so.1 libmult.so.1
	rm *.o

libadd.so.1: add.o
	gcc -shared add.o -o $(PLUGINS_DIR)/libadd.so.1

libdivide.so.1: divide.o
	gcc -shared divide.o -o $(PLUGINS_DIR)/libdivide.so.1

libmult.so.1: mult.o
	gcc -shared mult.o -o $(PLUGINS_DIR)/libmult.so.1

add.o: lib_src/add.c
	gcc lib_src/add.c -c -fPIC

divide.o: lib_src/divide.c
	gcc lib_src/divide.c -c -fPIC

mult.o: lib_src/mult.c
	gcc lib_src/mult.c -c -fPIC

clean:
	rm -rf *.o calc