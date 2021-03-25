//
//  main.cpp
//  dijkstra
//
//  Created by revanthAVS on 20/03/21.
//

#include <iostream>
#include <list>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <queue>        // std::priority_queue


using namespace std;

class Digraph {
public:
  void addVertex(int v);

  void addEdge(int u, int v);

  bool isVertex(int v);

  bool isEdge(int u, int v);

  unordered_set<int>::const_iterator neighbours(int v) const;

  unordered_set<int>::const_iterator endIterator(int v) const;

  int numNeighbours(int v);

  int size();

  vector<int> vertices();
  
  bool isWalk(vector<int> walk);

  bool isPath(vector<int> path);

private:
  unordered_map<int, unordered_set<int>> nbrs;
};


void Digraph::addVertex(int v) {
  nbrs[v];
}

void Digraph::addEdge(int u, int v) {
  addVertex(v);
  nbrs[u].insert(v); // will also add u to nbrs if it was not there already
}


bool Digraph::isVertex(int v) {
  return nbrs.find(v) != nbrs.end();
}

bool Digraph::isEdge(int u, int v) {

    return nbrs.find(u) != nbrs.end() && nbrs[u].find(v) != nbrs[u].end();
}

unordered_set<int>::const_iterator Digraph::neighbours(int v) const {
  return nbrs.find(v)->second.begin();
}

unordered_set<int>::const_iterator Digraph::endIterator(int v) const {
  // this will crash the program if v is not a vertex
  return nbrs.find(v)->second.end();
}

int Digraph::numNeighbours(int v) {
  return nbrs.find(v)->second.size();
}

int Digraph::size() {
  return nbrs.size();
}

vector<int> Digraph::vertices() {
  vector<int> v;
  for (unordered_map<int, unordered_set<int>>::const_iterator it = nbrs.begin();
    it != nbrs.end(); it++) {
      v.push_back(it->first);
    }
  return v;
}

bool Digraph::isWalk(vector<int> walk) {
  if (walk.size() == 0)
    return false;
  if (walk.size() == 1)
    return isVertex(walk[0]);
  for (vector<int>::size_type i=0; i<walk.size()-1; i++)
    if (!isEdge(walk[i], walk[i+1]))
      return false;

  return true;
}

bool Digraph::isPath(vector<int> path) {
  set<int> s(path.begin(), path.end());
  if (s.size() < path.size())
    return false;

  return isWalk(path);
}


class WDigraph : public Digraph {
public:
  long long getCost(int u, int v) const {
    return cost.at(u).at(v);
  }

void addEdge(int u, int v, long long w){

    Digraph::addEdge(u, v);
    cost[u][v] = w;
  }

private:
  unordered_map<int, unordered_map<int, long long>> cost;
};

typedef pair<int, long long> PIL;

typedef pair<int, PIL> PIPIL;

typedef pair<int, int> PII;


//void dijkstra(const WDigraph& graph, int startVertex,
//    unordered_map<int, PIL>& searchTree) {
//
//    list<PIPIL> fires;
//
//    fires.push_back(PIPIL(startVertex, PIL(-1, 0)));
//
//    while (!fires.empty()) {
//
//        auto earliestFire = fires.begin();
//        for (auto iter = fires.begin(); iter != fires.end(); ++iter) {
//            if (iter->second.second < earliestFire->second.second) {
//                earliestFire = iter;
//            }
//        }
//
//        int v = earliestFire->first, u = earliestFire->second.first, d = earliestFire->second.second;
//
//        fires.erase(earliestFire);
//
//        if (searchTree.find(v) != searchTree.end()) {
//            continue;
//        }
//
//        searchTree[v] = PIL(u, d);
//
//        for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
//            int nbr = *iter;
//
//            long long burn = d + graph.getCost(v, nbr);
//            fires.push_back(PIPIL(nbr, PIL(v, burn)));
//        }
//    }
//}

// Only assumes the key type K is totally ordered and comparable via <
template <class T, class K>
struct HeapItem {
  T item;
  K key;
};

template <class T, class K>
class BinaryHeap {
public:
  // constructor not required because the only "initialization"
  // is through the constructor of the variable "heap" which is called by default

  // return the minimum element in the heap
  HeapItem<T, K> min() const;

  // insert an item with the given key
  // if the item is already in the heap, will still insert a new copy with this key
  void insert(const T& item, const K& key);
    
  void insert(const T& item, const K& key, vector<HeapItem<T,K>>& heapcopy, const unsigned int heapsize);

  // pop the minimum item from the heap
  void popMin();

  // returns the number of items held in the heap
  int size() const;

private:
  // the array holding the heap
  std::vector< HeapItem<T, K> > heap;

  //  will fix the heap property at index i and recurse with its parent
  void fixHeapUp(int i);

  // will fix the heap property at index i and recurse with the child
  // that received i's item (if appropriate)
  void fixHeapDown(int i);

  // R: Returns the parent index of the given index (Helper method)
  unsigned int parent_index(unsigned int i);
};

/*
  R: Returns the pair (x, key) with the smallest key
*/
template <class T, class K>
HeapItem<T, K> BinaryHeap<T, K>::min() const{
  if (heap.size() > 0){
    // R: Since the first element of the array holding the heap is the root index
    // R: Which holds the minimum key value
    return heap[0];
  }
  return heap[0];
}

template <class T, class K>
void BinaryHeap<T, K>::insert(const T& item, const K& key){
  HeapItem<T, K> temp_v;  // Need to update this approach
  temp_v.item = item;
  temp_v.key = key;
  heap.push_back(temp_v);
  // R: Should be implemented in fixheapup
  unsigned int v_i = heap.size() - 1;
  unsigned int p_i = parent_index(v_i);
  while ((v_i != 0) && (heap[v_i].key < heap[p_i].key)){
    swap(heap[v_i], heap[p_i]);
    v_i = p_i;
    p_i = parent_index(v_i);
  }
}

template <class T, class K>
void BinaryHeap<T,K>::fixHeapDown(int i)
{
    int left=2*i+1;
    int right=2*i+2;
    int min_child=heap[left].key<heap[right].key ? left : right;
  while (heap[i].key > heap[min_child].key)
  {
    swap(heap[i], heap[min_child]);
    i=min_child;
    left=2*i+1;
    right=2*i+2;

    //J: has no children
    if(left> size()-1)  break;

    //J: has left child only
    else if(left < (size()-1) && right > (size()-1))  min_child=left;

    //J: has both children
    else min_child=heap[left].key<heap[right].key? left : right;
  }
}

template <class T, class K>
void BinaryHeap<T,K>::popMin(){

    swap(heap[0],heap[size()-1]);
    heap.pop_back();
    fixHeapDown(0);
}



//template <class T, class K>
//void BinaryHeap<T, K>::insert(const T& item, const K& key, vector<HeapItem<T,K>>& heapcopy, const unsigned int heapsize){
//  HeapItem<T, K> temp_v;  // Need to update this approach
//  temp_v.item = item;
//  temp_v.key = key;
//  heapcopy.push_back(temp_v);
//  // R: Should be implemented in fixheapup
////  unsigned int v_i = heapcopy.size() - 1;
//    unsigned int v_i = heapsize;
//  unsigned int p_i = parent_index(v_i);
//  while ((v_i != 0) && (heapcopy[v_i].key < heapcopy[p_i].key)){
//    swap(heapcopy[v_i], heapcopy[p_i]);
//    v_i = p_i;
//    p_i = parent_index(v_i);
//  }
//}

template <class T, class K>
int BinaryHeap<T, K>::size() const{
  return heap.size();
}

//template <class T, class K>
//void BinaryHeap<T, K>::popMin(){
//
//    std::vector< HeapItem<T, K> > heapcopy;
//
//    for (int i = 1; i < heap.size(); i++){
//        HeapItem<T, K> temp;
//        temp.item = heap[i].item;
//        temp.key = heap[i].key;
//        insert(temp.item, temp.key, heapcopy, heapcopy.size());
//    }
//    heap = heapcopy;
//    return;
//}


template <class T, class K>
unsigned int BinaryHeap<T, K>::parent_index(unsigned int i){
  if (i > 0){
    // R: Since we are storing all the values in array
    return ((i-1)/2);
  }
  else
    return 0;
}


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
        events.popMin();

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


//template <class T, class K>
//HeapItem<T, K> BinaryHeap::min() const{
//  if (heap.size() > 0){
//    // Since the first element of the array holding the heap is the root index
//    // Which holds the minimum key value
//    return heap[0];
//  }
//}

//template <class T, class K>
//void BinaryHeap::insert(const T& item, const K& key){
//  HeapItem<T, K> temp_v;
//  temp_v.item = item;
//  temp_v.key = key;
//  heap.push_back(temp_v);
//  unsigned int v_i = heap.size() - 1;
//  unsigned int p_i = parent_index(v_i);
//  while ((v_i != 0) && (heap[v_i].key < heap[p_i].key)){
//    swap(heap[v_i], heap[p_i]);
//    v_i = p_i;
//    p_i = parent_index(v_i);
//  }
//}
//
//int BinaryHeap::size() const{
//  return heap.size();
//}
//
//template <class T, class K>
//unsigned int BinaryHeap<T, K>::parent_index(unsigned int i){
//  if (i > 0){
//    return ((i-1)/2);
//  }
//  else
//    return 0;
//}

int main() {
    WDigraph graph;

    int startVertex, endVertex;

    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        int label;
        cin >> label;
        graph.addVertex(label);
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        graph.addEdge(u, v, c);
    }
    cin >> startVertex >> endVertex;

    unordered_map<int, PIL> searchTree;
    dijkstra(graph, startVertex, searchTree);

    list<int> path;
    if (searchTree.find(endVertex) == searchTree.end()) {
      cout << "Vertex " << endVertex << " not reachable from " << startVertex << endl;
    }
    else {
      int stepping = endVertex;
      while (stepping != startVertex) {
        path.push_front(stepping);

        stepping = searchTree[stepping].first;
      }
      path.push_front(startVertex);

      cout << "Cost of cheapest path from " << startVertex << " to "
           << endVertex << " is: " << searchTree[endVertex].second << endl;
      cout << "Path:";
      for (auto it : path) {
        cout << ' ' << it;
      }
      cout << endl;
    }

    return 0;
}
