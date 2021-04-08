##########################################
#  Name: Atmakuri Venkata Sai Revanth
#  ID  : 1684293
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
OBJS= server/digraph.o server/server.o server/dijkstra.o server/server client/client.o client/client

# executable targets
all: server client

server: digraph.o server.o dijkstra.o 
	$(CC) server/digraph.o server/server.o server/dijkstra.o -o server/server

client: client.o
	$(CC) client/client.o -o client/client

# object targets
digraph.o: server/digraph.cpp server/digraph.h
	$(CC) server/digraph.cpp -o server/digraph.o $(CFLAGS)

server.o: server/server.cpp
	$(CC) server/server.cpp -o server/server.o $(CFLAGS)

dijkstra.o: server/dijkstra.cpp server/dijkstra.h server/heap.h
	$(CC) server/dijkstra.cpp -o server/dijkstra.o $(CFLAGS)

client.o: client/client.cpp
	$(CC) client/client.cpp -o client/client.o $(CFLAGS)

clean:
	rm -r -f $(OBJS) 
