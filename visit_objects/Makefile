test: test.o visit.o
	g++ -Wall test.o visit.o -o test

test.o: test.cpp
	g++ -Wall -c test.cpp

visit.o: visit.cpp
	g++ -Wall -c visit.cpp

clean:
	rm -rf *.o test
