#include <vector>
using namespace std;

// Only assumes the key type K is totally ordered and comparable via <
template <class T, class K>
struct HeapItem {
	//a structure to store an item and its key.
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
};

// Since there are template arguments implementing class methods in header file

/*
  Returns the pair (x, key) with the smallest key
*/
template <class T, class K>
HeapItem<T, K> BinaryHeap<T, K>::min() const{
/* This method returns the minimum element of the heap. This function takes no 
   arguments and returns a HeapItem<T,K>

   Return:
   heap[0] (HeapItem<T, K>) : the first element of the heap which is also the 
   							  smallest element of the heap
*/

  // To make sure that heap is not empty
  if (heap.size() > 0){
    // Since the first element of the array holding the heap is the root index
    // Which holds the minimum key value
    return heap[0];
  }
}

template <class T, class K>
void BinaryHeap<T, K>::fixHeapUp(int i){
/*	This method takes in the index i and continues to swap it with its parent 
	vertices (fixes it up) until it satisfies the heap property.

	Parameters:
	i(int): the vertex which doesn't satisfy the heap property

*/

    // To get the parent index in the vector
    int p_i = (i-1)/2;

    // If the vertex is at root then no need to check for heap property
    // If the new vertex validates the heap property if not swap
    if ((i != 0) && (heap[i].key < heap[p_i].key)){

        swap(heap[i], heap[p_i]);
        i = p_i;
        // Checkng if the swaped vertex obey's the heap property
        fixHeapUp(i);
    }
    // Base case:- if vertex at index i satisfies the heap property
    // or vertex is at root
    else
        return;
}

template <class T, class K>
void BinaryHeap<T, K>::insert(const T& item, const K& key){
/*This method inserts an element at the end of the heaap and then 
  fixes the heap up such that all the elements in the heap satisfy 
  the heap property.
	
  Parameters: 
  item :a const reference to the item of struct HeapItem<T,K>
  key : a const reference to the key of struct HeapItem<T,K>

  */

  HeapItem<T, K> temp_v;
  temp_v.item = item;
  temp_v.key = key;

  heap.push_back(temp_v);
  unsigned int v_i = heap.size() - 1;
  
  // Since the new vertex is added at then end checking
  // if the new vertex satisfy's the heap property
  fixHeapUp(v_i);
}

template <class T, class K>
int BinaryHeap<T, K>::size() const{
	/*This method returns a the number of elements in the heap 

	  Return:
	  heap.size() : the size of the heap which is an integer value 
	*/

	  // Since the size of Binaryheap is the number of vertexs in heap
  return heap.size();
}


template <class T, class K>
void BinaryHeap<T, K>::fixHeapDown(int i){
/*This method continues to swap the parent index i with with the 
  minimmum of both its children (right and left) until the heap
  property is satisfied.

  Parameters:
  i(int): the vertex which doesn't satisfy the heap property
*/ 

  int left = 2*i + 1;
  int right = 2*i + 2;
  int min_child = 0;
  
  // If index i has no children
  if (left >= (size())) return;
  // If index i has only left child
  else if ((left < (size())) && (right >= (size()))) min_child = left;
  // When index i has both the children
  else min_child = heap[left].key < heap[right].key ? left : right;

  // Check for the heap property at the index i and it's minimum child
  // If it doesn't satifies the heap property then swap
  if (heap[i].key > heap[min_child].key){
    swap(heap[i], heap[min_child]);
    // To check if the vertex at minimum child satisfies the heap property
    fixHeapDown(min_child);
  }
  // Basecase:- If the vertex satisfies the heap property at index i
  else{
    return;
  }
}


template <class T, class K>
void BinaryHeap<T,K>::popMin(){
	/*This method pops the minimum element off the heap and then fixes 
	  the heap property wherever in the heap it is not satisfied. 
	  
	  Parameters:
	  None

	  Return:
	  None
	*/
  
  // If there are no elements to pop in the heap
  if(size()==0)
  {
    return;
  }
  
  // Since minimum vertex is the first element in the heap
  swap(heap[0],heap[size()-1]);
  heap.pop_back();
  
  // To check for the heap property after swapping with last vertex
  fixHeapDown(0);
}
