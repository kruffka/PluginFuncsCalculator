

calculator: calculator.c libmath.so.1
	# gcc calculator.c libmath.so.1 -L. -o calc
	gcc calculator.c -L. -o calc -ldl

libmath.so.1: add.o
	gcc -shared add.o -o libmath.so.1

add.o: plugins/add.c
	gcc plugins/add.c -c -fPIC

clean:
	rm -rf *.o *.so.* calc