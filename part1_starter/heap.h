#include <vector>

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
  unsigned int p_i = parent_index(v_i);
  while ((v_i != 0) && (heap[v_i].key < heap[p_i].key)){
    swap(heap[v_i], heap[p_i]);
    v_i = p_i;
    p_i = parent_index(v_i);
  }
}

template <class T, class K>
int BinaryHeap<T, K>::size() const{
  return heap.size();
}

template <class T, class K>
unsigned int BinaryHeap<T, K>::parent_index(unsigned int i){
  if (i > 0){
    // R: Since we are storing all the values in array
    return ((i-1)/2);
  }
  else
    return 0;
}