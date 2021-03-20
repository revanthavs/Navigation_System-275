#include <iostream>
#include <unordered_map>
#include "heap.h"
#include "dijkstra.h"

using namespace std;

// R: need to add Dijkstra's algorithm implementation

void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree);

// R: Need to recheck the variables with auto declaration
// after implementing heap class
typedef pair<int, long long> PIL;

void dijkstra(const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& searchTree) {

  BinaryHeap events;

  events.insert(PIL(startVertex, startVertex), 0);

    while (events.size() > 0) {

      auto current_item = events.min();

      int v = current_item.first, u = current_item.second.first, d = current_item.second.second;

      events.popmin();

      if (searchTree.find(v) == searchTree.end()) {
          searchTree[v] = u;
          for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
              int nbr = *iter;
              events.insert(PIL(v, nbr), d+getCost(v, nbr));
          }
      }
    }

    return;
}