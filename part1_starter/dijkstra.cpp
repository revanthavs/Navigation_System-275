/*
Name: 
ID  : 

Name: Jasmine Kaur Wadhwa
ID  : 1619400

CMPUT 275 WINTER 2021
Assignment #1  Trivial Nagivation System
*/

#include <iostream>
// To store the search tree and since the find takes constant time
#include <unordered_map>
#include "heap.h" 
#include "dijkstra.h"

using namespace std;

void dijkstra(const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& tree) {
/* It takes in a reference to the instance of graph class, a reference 
   of the searchTree and a startVertex. The function then finds the all 
   the vertices that can be reached from the startVertex in the order 
   that they are reached, and updates the searchTree.  
   
   Parameters:
   graph: a const reference to the instance of the weighted graph class,
          the graph we need to explore
   startVertex: an integer value that stores the starting vertex 
   tree: a reference to the unordered map of int and PIL, to store the 
         vertices that can be reached from startVertex
*/

  // New instance of BinaryHeap to keep track of events
  BinaryHeap<PII, long long int> events;

  // inserting the start vertexto in the heap to get started
  events.insert(PII(startVertex, startVertex), 0);

  // Loop ends after finding the shortest path
    while (events.size() > 0) {

        // The current vertex neet to be process
        auto current_item = events.min();

        // v the current vertex to be explored
        // u is the predessor of the vertex v
        // d is the total cost/time that took to reach the current vertex
        int u = current_item.item.first, v = current_item.item.second;
        long long d = current_item.key;

        // Since we are processing min vertex and need to update the heap
        events.popMin();

        // Checking if we alreay explored the vertex before
        if (tree.find(v) == tree.end()) {
            tree[v] = PIL(u, d);

            // Exploring all the neigbors of the current vertex
            for (auto iter = graph.neighbours(v);
             iter != graph.endIterator(v); iter++) {
                int nbr = *iter;
                
                // inserting the neighbour vertex with ther cost/time to reach them
                events.insert(PII(v, nbr), d+graph.getCost(v, nbr));
            }
        }
    }

    return;
}
