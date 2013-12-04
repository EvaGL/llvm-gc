test: test.o visit.o graph.o
	gcc -Wall test.o visit.o graph.o -o test

test.o: test.c
	gcc -Wall -c test.c

visit.o: visit.c
	gcc -Wall -c visit.c

graph.o: graph.c
	gcc -Wall -c graph.c

clean:
	rm -rf *.o test

