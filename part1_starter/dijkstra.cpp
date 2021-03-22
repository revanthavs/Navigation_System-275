#include <iostream>
#include <unordered_map>
#include "heap.h"
#include "dijkstra.h"

using namespace std;

// R: need to add Dijkstra's algorithm implementation

void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree);

template < class T, class K>
void dijkstra(const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& searchTree) {

  // R: New instance of BinaryHeap to keep track of events
  BinaryHeap<T, k> events;

  // R: inserting the dummy element to get started
  // events.insert(PIPIL(startVertex, PIL(startVertex, 0)));
  events.insert(startVertex, PIL(startVertex, 0));

  // Loop ends after finding the shortest path
    while (events.size() > 0) {

        // The current vertex neet to be process
        auto current_item = events.min();

        // int v = current_item.first, u = current_item.second.first;
        // long long int d = current_item.second.second;

        int v = current_item.item; int u = current_item.key.first;
        long long d = current_item.key.second;

        // Since we are processing min vertex and need to update the heap
        events.popmin();

        if (searchTree.find(v) == searchTree.end()) {
            searchTree[v] = PIL(u, d);

            // Exploring all the neigbors of the vertex
            for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
                int nbr = *iter;
                // events.insert(PIPIL(v, PIL(nbr, d+graph.getCost(v, nbr))));
                eveents.insert(v, PIL(nbr, d+graph.getCost(v, nbr)));
            }
        }
    }

    return;
}


// #include <iostream>
// #include <unordered_map>
// #include "heap.h"
// #include "dijkstra.h"

// using namespace std;

// // R: need to add Dijkstra's algorithm implementation

// void dijkstra(const WDigraph& graph, int startVertex,
//               unordered_map<int, PIL>& tree);

// void dijkstra(const WDigraph& graph, int startVertex, 
//     unordered_map<int, PIL>& tree) {

//   BinaryHeap events;

//   events.insert(PIL(startVertex, startVertex), 0);

//     while (events.size() > 0) {

//       auto current_item = events.min();

//       int v = current_item.first.first, u = current_item.first.second, d = current_item.second;

//       events.popmin();

//       if (tree.find(v) == tree.end()) {
//           tree[v] = u;
//           for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
//               int nbr = *iter;
//               events.insert(PIL(v, nbr), d+getCost(v, nbr));
//           }
//       }
//     }

//     return;
// }