libjblab-gc.so: gc.o malloc.o graph.o 
	gcc -g -shared -fpic graph.o gc.o malloc.o -o libjblab-gc.so

malloc.o: malloc.c mark.h
	gcc -g -c -fpic malloc.c

gc.o: gc.c gc.h graph.h mark.h
	gcc -g -c gc.c -fpic

graph.o: graph.c graph.h
	gcc -g -c -fpic graph.c

tests: test test_cycle test_sweep test_autocall

test_sweep: test_sweep.c
	gcc -g -o test_sweep test_sweep.c -L. -ljblab-gc

test_autocall: test_autocall.ll
	llvm-as < test_autocall.ll | llc -load=/home/liana/progs/llvm/build/lib/jblab-gc.so > test_autocall.s
	gcc -o test_autocall test_autocall.s -L. -ljblab-gc
	rm test_autocall.s

test: test.ll
	llvm-as < test.ll | llc -load=/home/liana/progs/llvm/build/lib/jblab-gc.so > test.s
	gcc -o test test.s -L. -ljblab-gc
	rm test.s

test_cycle: test_cycle.ll
	llvm-as < test_cycle.ll | llc -load=jblab-gc.so > test.s
	gcc -o test_cycle test.s -L. -ljblab-gc
	rm test.s

clean:
	rm -f *.o *.so test test_cycle test_sweep test_autocall 
	
