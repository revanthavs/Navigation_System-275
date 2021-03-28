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

  // pop the minimum item from the heap
  void popMin();

  // returns the number of items held in the heap
  int size() const;

private:
  // the array holding the heap
  std::vector< HeapItem<T, K> > heap;

  // will return the left child of the vertex
  int left(int parent);

  //will return the right child of the vertex
  int right(int parent);

  //  will fix the heap property at index i and recurse with its parent
  void fixHeapUp(int i);

  // will fix the heap property at index i and recurse with the child
  // that received i's item (if appropriate)
  void fixHeapDown(int i);

  // R: Returns the parent index of the given index (Helper method)
  // unsigned int parent_index(unsigned int i);
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
  //return ;
}

template <class T, class K>
void BinaryHeap<T, K>::fixHeapUp(int i){

    int p_i = (i-1)/2;
    // R: If the vertex is at root then no need to check for heap property
    // R; If the new vertex validates the heap property if not swap
    if ((i != 0) && (heap[i].key < heap[p_i].key)){
        swap(heap[i], heap[p_i]);
        i = p_i;
        // R: Checkng if the swaped vertex obey's the heap property
        fixHeapUp(i);
    }
    // R: Base case if it's satisfies the heap property or vertex is at root
    else
        return;
}

template <class T, class K>
void BinaryHeap<T, K>::insert(const T& item, const K& key){
  HeapItem<T, K> temp_v;  // Need to update this approach
  temp_v.item = item;
  temp_v.key = key;
  heap.push_back(temp_v);
  // R: Should be implemented in fixheapup
  unsigned int v_i = heap.size() - 1;
  fixHeapUp(v_i);
  
}

template <class T, class K>
int BinaryHeap<T, K>::size() const{
  return heap.size();
}



template <class T, class K>
int BinaryHeap<T, K>::left(int parent){
  int left=2*parent+1;

  //J: if the left child has a valid index
  if (left < size())
    return left;
  //J: if the index of left child is out of bounds
  else
    return -1;
}
template <class T, class K>
int BinaryHeap<T, K>::right(int parent){
  int right=2*parent+1;

  //J: if the right child has a valid index
  if (right < size())
    return right;
  //J: if the index of right child is out of bounds
  else
    return -1;
}
template <class T, class K>
void BinaryHeap<T,K>::fixHeapDown(int i)
{
  int child1=left(i);
  int child2=right(i);
  
  //J: if both the children are valid indices and the right child has the minimum value
  if(child1 >= 0 && child2 >= 0 && heap[child1].key > heap[child2].key)
  {
    child1=child2;
  }
  //J: if the minimum child is a valid index and the does not obey the heap property,swap
  if (child1 > 0 && heap[i].key > heap[child1].key)
  {
    swap(heap[i], heap[child1]);
    //J: checking if the swapped vertex obeys the heap property
    fixHeapDown(child1);
  }
}

template <class T, class K>
void BinaryHeap<T,K>::popMin(){
  if(size()==0){
    return;
  }
  
  swap(heap[0],heap[size()-1]);
  heap.pop_back();
  fixHeapDown(0);
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

int main()
{
  WDigraph wgraph;
  string filename="edmonton-roads-2.0.1.txt";
  //Point locations;
  unordered_map<int, Point> points;
  readGraph(filename, wgraph, points);

  Point start_point, end_point;
  string temp;
  cin >> temp;
  if (temp[0] == 'R'){
    cin >> temp;
    start_point.lat = stol(temp);
    cin >> temp;
    start_point.lon = stol(temp);
    cin >> temp;
    end_point.lat = stol(temp);
    cin >> temp;
    end_point.lon = stol(temp);
  }
  int startVertex = 0, endVertex = 0;

  for (auto it: points){
    if (it.second.lat == start_point.lat){
      startVertex = it.first;
    }
  }

  for (auto it: points){
    if (it.second.lat == end_point.lat){
      endVertex = it.first;
    }
  }

  unordered_map<int, PIL> searchTree;
  dijkstra(wgraph, startVertex, searchTree);

  list<int> path;
  if (searchTree.find(endVertex) == searchTree.end()){
    cout << "N " << 0 << endl;
  }
  else {
    int stepping = endVertex;
    while (stepping != startVertex) {
      path.push_front(stepping);

      stepping = searchTree[stepping].first;
    }
    path.push_front(startVertex);
  }
  cout << "N " << path.size();
  for (auto it: path){
    cin >> temp;
    if (temp[0] == 'A'){
      cout << "W " << points[it].lat << " " << points[it].lon;
    }
  }

  cout << "E" << endl;
  return 0;
}
