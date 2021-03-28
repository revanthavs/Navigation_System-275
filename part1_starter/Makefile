##########################################
#  Name: 
#  ID  : 
#
#  Name: Jasmine Kaur Wadhwa
#  ID  : 1619400
#
#  CMPUT 275 WINTER 2021
#  Assignment #1  Trivial Nagivation System
#
##########################################


CC=g++
CFLAGS=-c -Wall -O2 -std=c++11
OBJS= digraph.o server.o dijkstra.o

# executable targets
all: server

server: digraph.o server.o dijkstra.o 
	$(CC) digraph.o server.o dijkstra.o -o server

# object targets
digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

server.o: server.cpp
	$(CC) server.cpp -o server.o $(CFLAGS)

dijkstra.o: dijkstra.cpp dijkstra.h heap.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

clean:
	@rm $(OBJS) server
