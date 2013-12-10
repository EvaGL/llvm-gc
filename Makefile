.PHONY: all

all: libdlmalloc.so

libdlmalloc.so: malloc.c mark.h
	gcc -O3 -shared -fpic malloc.c -o libdlmalloc.so

test: test.c mark.h
	gcc -L. -I. test.c -o test -ldlmalloc -Wl,-rpath=.

clean:
	rm libdlmalloc.so test
