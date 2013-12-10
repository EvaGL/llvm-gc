libjblab-gc.so: gc.o malloc.o graph.o 
	gcc -g -shared -fpic graph.o gc.o malloc.o -o libjblab-gc.so

malloc.o: malloc.c mark.h
	gcc -g -c -fpic malloc.c

gc.o: gc.c gc.h graph.h mark.h
	gcc -g -c gc.c -fpic

graph.o: graph.c graph.h
	gcc -g -c -fpic graph.c

test: test.ll
	llvm-as < test.ll | llc -load=jblab-gc.so > test.s
	gcc -o test test.s -L. -ljblab-gc
	rm test.s

clean:
	rm -rf *.o *.so test

