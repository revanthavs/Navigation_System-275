#include <iostream>
#include <unordered_map>
#include "heap.h"
#include "dijkstra.h"

using namespace std;

// R: need to add Dijkstra's algorithm implementation

void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree);

void dijkstra(const WDigraph& graph, int startVertex, 
    unordered_map<int, PIL>& tree) {

  BinaryHeap events;

  events.insert(PIL(startVertex, startVertex), 0);

    while (events.size() > 0) {

      auto current_item = events.min();

      int v = current_item.first.first, u = current_item.first.second, d = current_item.second;

      events.popmin();

      if (tree.find(v) == tree.end()) {
          tree[v] = u;
          for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
              int nbr = *iter;
              events.insert(PIL(v, nbr), d+getCost(v, nbr));
          }
      }
    }

    return;
}