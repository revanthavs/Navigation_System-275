#include <iostream>
#include <unordered_map>
#include "heap.h"
#include "dijkstra.h"

using namespace std;

// R: need to add Dijkstra's algorithm implementation

void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree);

void dijkstra(const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& searchTree) {

  // R: New instance of BinaryHeap to keep track of events
  BinaryHeap<PII, long long int> events;

  // R: inserting the dummy element to get started
  // events.insert(startVertex, PIL(startVertex, 0));
  events.insert(PII(startVertex, startVertex), 0);

  // Loop ends after finding the shortest path
    while (events.size() > 0) {

        // The current vertex neet to be process
        auto current_item = events.min();

        // int v = current_item.item; int u = current_item.key.first;
        // long long d = current_item.key.second;

        int u = current_item.item.first, v = current_item.item.second;
        long long d = current_item.key;

        // Since we are processing min vertex and need to update the heap
        events.popmin();

        if (searchTree.find(v) == searchTree.end()) {
            searchTree[v] = PIL(u, d);

            // Exploring all the neigbors of the vertex
            for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
                int nbr = *iter;
                // events.insert(v, PIL(nbr, d+graph.getCost(v, nbr)));
                events.insert(PII(v, nbr), d+graph.getCost(v, nbr));
            }
        }
    }

    return;
}
