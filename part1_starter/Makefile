# R: Temporary make file to test code

all: digraph.o server.o
	g++ digraph.o server.o -o testfile -std=c++11

digraph.o: digraph.cpp digraph.h
	g++ -c digraph.cpp -o digraph.o -std=c++11

server.o: server.cpp
	g++ -c server.cpp -o server.o -std=c++11

clean:
	rm -r -f digraph.o server.o testfile