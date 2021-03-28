#include <iostream>
// To store the search tree and since the find takes constant time
#include <unordered_map>
#include "heap.h" 
#include "dijkstra.h"

using namespace std;

void dijkstra(const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& tree) {

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
