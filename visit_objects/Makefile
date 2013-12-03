test: test.o visit.o graph.o
	g++ -Wall test.o visit.o graph.o -o test

test.o: test.cpp
	g++ -Wall -c test.cpp

visit.o: visit.cpp
	g++ -Wall -c visit.cpp

graph.o: graph.cpp
	g++ -Wall -c graph.cpp

clean:
	rm -rf *.o test
