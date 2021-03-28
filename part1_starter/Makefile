
# R: Temporary make file to test code

all: server

server: digraph.o server.o dijkstra.o
	g++ digraph.o server.o dijkstra.o -o server -std=c++11

digraph.o: digraph.cpp digraph.h
	g++ -c digraph.cpp -o digraph.o -std=c++11

server.o: server.cpp
	g++ -c server.cpp -o server.o -std=c++11

dijkstra.o: dijkstra.cpp dijkstra.h	heap.h
	g++ -c dijkstra.cpp -o dijkstra.o -std=c++11

clean:
	rm -r -f digraph.o server.o dijkstra.o testfile

# CC=g++
# CFLAGS=-c -Wall -O2 -std=c++11
# PROGS= server
# OBJS= digraph.o server.o dijkstra.o

# # executable targets
# all: server

# sever: digraph.o server.o dijkstra.o 
# 	$(CC) digraph.o server.o dijkstra.o -o $(PROGS)

# # object targets
# digraph.o: digraph.cpp digraph.h
# 	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

# server.o: server.cpp
# 	$(CC) server.cpp -o server.o $(CFLAGS)

# dijkstra.o: dijkstra.cpp dijkstra.h heap.h
# 	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

# clean:
# 	@rm $(OBJS) $(PROGS)
