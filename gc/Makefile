libjblab-gc.so: gc.o malloc.o graph.o 
	gcc -shared -fpic graph.o gc.o malloc.o -o libjblab-gc.so

malloc.o: malloc.c mark.h
	gcc -c -fpic malloc.c

gc.o: gc.c gc.h graph.h mark.h
	gcc -c -fpic gc.c

graph.o: graph.c graph.h
	gcc -c -fpic graph.c

test: test.c
	gcc -o test test.c -L. -ljblab-gc 

clean:
	rm -rf *.o *.so test

