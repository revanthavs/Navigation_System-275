# R: Temporary make file to test code

all: digraph.o server.o	dijkstra.o
	g++ digraph.o server.o dijkstra.o -o testfile -std=c++11

digraph.o: digraph.cpp digraph.h
	g++ -c digraph.cpp -o digraph.o -std=c++11

server.o: server.cpp
	g++ -c server.cpp -o server.o -std=c++11

dijkstra.o: dijkstra.cpp dijkstra.h	heap.h
	g++ -c dijkstra.cpp -o dijkstra.o -std=c++11

clean:
	rm -r -f digraph.o server.o dijkstra.o testfile