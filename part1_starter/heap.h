#include <vector>
using namespace std; // need to remove this

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

// need to remove
  void insert(const T& item, const K& key, vector<HeapItem<T,K>>& heapcopy, const unsigned int heapsize);

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


// template <class T, class K>
// unsigned int BinaryHeap<T, K>::parent_index(unsigned int i){
//   if (i > 0){
//     // R: Since we are storing all the values in array
//     return ((i-1)/2);
//   }
//   else
//     return 0;
// }

template <class T, class K>
void BinaryHeap<T, K>::insert(const T& item, const K& key){
  HeapItem<T, K> temp_v;  // Need to update this approach
  temp_v.item = item;
  temp_v.key = key;
  heap.push_back(temp_v);
  // R: Should be implemented in fixheapup
  unsigned int v_i = heap.size() - 1;
  fixHeapUp(v_i);
  // unsigned int p_i = parent_index(v_i);
  // while ((v_i != 0) && (heap[v_i].key < heap[p_i].key)){
  //   swap(heap[v_i], heap[p_i]);
  //   v_i = p_i;
  //   p_i = parent_index(v_i);
  // }
}

template <class T, class K>
int BinaryHeap<T, K>::size() const{
  return heap.size();
}

// template <class T, class K>
// int BinaryHeap<T, K>::left(int parent){
//   int left=2*parent+1;

//   //J: if the left child has a valid index
//   if (left < size())
//     return left;
//   //J: if the index of left child is out of bounds
//   else
//     return -1;
// }
// template <class T, class K>
// int BinaryHeap<T, K>::right(int parent){
//   int right=2*parent+1;

//   //J: if the right child has a valid index
//   if (right < size())
//     return right;
//   //J: if the index of right child is out of bounds
//   else
//     return -1;
// }
// template <class T, class K>
// void BinaryHeap<T,K>::fixHeapDown(int i)
// {
//   int child1=left(i);
//   int child2=right(i);
  
//   //J: if both the children are valid indices and the right child has the minimum value
//   if(child1 >= 0 && child2 >= 0 && heap[child1].key > heap[child2].key)
//   {
//     child1=child2;
//   }
//   //J: if the minimum child is a valid index and the does not obey the heap property,swap
//   if (child1 > 0 && heap[i].key > heap[child1].key)
//   {
//     swap(heap[i], heap[child1]);
//     //J: checking if the swapped vertex obeys the heap property
//     fixHeapDown(child1);
//   }
// }

  /*while (heap[i].key > heap[min_child].key)
  {

  //J: has no children
  if(left> size()-1)  break;

    swap(heap[i], heap[min_child]);
    i=min_child;
    left=2*i+1;
    right=2*i+2;

  // //J: has no children
  // if(left> size()-1)  break;

  //J: has left child only
  if(left < (size()-1) && right > (size()-1))  min_child=left;

  //J: has both children
  else min_child= heap[left].key < heap[right].key ? left : right;
  }
}*/

// Need to remove
template <class T, class K>
void BinaryHeap<T, K>::insert(const T& item, const K& key, vector<HeapItem<T,K>>& heapcopy, const unsigned int heapsize){
  HeapItem<T, K> temp_v;  // Need to update this approach
  temp_v.item = item;
  temp_v.key = key;
  heapcopy.push_back(temp_v);
  // R: Should be implemented in fixheapup
//  unsigned int v_i = heapcopy.size() - 1;
    unsigned int v_i = heapsize;
  unsigned int p_i = parent_index(v_i);
  while ((v_i != 0) && (heapcopy[v_i].key < heapcopy[p_i].key)){
    swap(heapcopy[v_i], heapcopy[p_i]);
    v_i = p_i;
    p_i = parent_index(v_i);
  }
}

template <class T, class K>
void BinaryHeap<T, K>::popMin(){

    std::vector< HeapItem<T, K> > heapcopy;

    for (int i = 1; i < heap.size(); i++){
        HeapItem<T, K> temp;
        temp.item = heap[i].item;
        temp.key = heap[i].key;
        insert(temp.item, temp.key, heapcopy, heapcopy.size());
    }
    heap = heapcopy;
    return;
}


// template <class T, class K>
// void BinaryHeap<T,K>::popMin(){
//   if(size()==0)
//   {
//     return;
//   }
  
//   swap(heap[0],heap[size()-1]);
//   heap.pop_back();
//   fixHeapDown(0);
// }


template <class T, class K>
unsigned int BinaryHeap<T, K>::parent_index(unsigned int i){
  if (i > 0){
    // R: Since we are storing all the values in array
    return ((i-1)/2);
  }
  else
    return 0;
}



// The origianl implementation
// #include <vector>

// // Only assumes the key type K is totally ordered and comparable via <
// template <class T, class K>
// struct HeapItem {
//   T item;
//   K key;
// };

// template <class T, class K>
// class BinaryHeap {
// public:
//   // constructor not required because the only "initialization"
//   // is through the constructor of the variable "heap" which is called by default

//   // return the minimum element in the heap
//   HeapItem<T, K> min() const;

//   // insert an item with the given key
//   // if the item is already in the heap, will still insert a new copy with this key
//   void insert(const T& item, const K& key);

//   // pop the minimum item from the heap
//   void popMin();

//   // returns the number of items held in the heap
//   int size() const;

// private:
//   // the array holding the heap
//   std::vector< HeapItem<T, K> > heap;

//   // will return the left child of the vertex
//   int left(int parent);

//   //will return the right child of the vertex
//   int right(int parent);

//   //  will fix the heap property at index i and recurse with its parent
//   void fixHeapUp(int i);

//   // will fix the heap property at index i and recurse with the child
//   // that received i's item (if appropriate)
//   void fixHeapDown(int i);

//   // R: Returns the parent index of the given index (Helper method)
//   // unsigned int parent_index(unsigned int i);
// };

// /*
//   R: Returns the pair (x, key) with the smallest key
// */
// template <class T, class K>
// HeapItem<T, K> BinaryHeap<T, K>::min() const{
//   if (heap.size() > 0){
//     // R: Since the first element of the array holding the heap is the root index
//     // R: Which holds the minimum key value
//     return heap[0];
//   }
//   //return ;
// }

// template <class T, class K>
// void BinaryHeap<T, K>::fixHeapUp(int i){

//     int p_i = (i-1)/2;
//     // R: If the vertex is at root then no need to check for heap property
//     // R; If the new vertex validates the heap property if not swap
//     if ((i != 0) && (heap[i].key < heap[p_i].key)){
//         swap(heap[i], heap[p_i]);
//         i = p_i;
//         // R: Checkng if the swaped vertex obey's the heap property
//         fixHeapUp(i);
//     }
//     // R: Base case if it's satisfies the heap property or vertex is at root
//     else
//         return;
// }

// template <class T, class K>
// void BinaryHeap<T, K>::insert(const T& item, const K& key){
//   HeapItem<T, K> temp_v;  // Need to update this approach
//   temp_v.item = item;
//   temp_v.key = key;
//   heap.push_back(temp_v);
//   // R: Should be implemented in fixheapup
//   unsigned int v_i = heap.size() - 1;
//   fixHeapUp(v_i);
//   // unsigned int p_i = parent_index(v_i);
//   // while ((v_i != 0) && (heap[v_i].key < heap[p_i].key)){
//   //   swap(heap[v_i], heap[p_i]);
//   //   v_i = p_i;
//   //   p_i = parent_index(v_i);
//   // }
// }

// template <class T, class K>
// int BinaryHeap<T, K>::size() const{
//   return heap.size();
// }

// // template <class T, class K>
// // unsigned int BinaryHeap<T, K>::parent_index(unsigned int i){
// //   if (i > 0){
// //     // R: Since we are storing all the values in array
// //     return ((i-1)/2);
// //   }
// //   else
// //     return 0;
// // }

// template <class T, class K>
// int BinaryHeap<T, K>::left(int parent){
// 	int left=2*parent+1;

// 	//J: if the left child has a valid index
// 	if (left < size())
// 		return left;
// 	//J: if the index of left child is out of bounds
// 	else
// 		return -1;
// }
// template <class T, class K>
// int BinaryHeap<T, K>::right(int parent){
// 	int right=2*parent+1;

// 	//J: if the right child has a valid index
// 	if (right < size())
// 		return right;
// 	//J: if the index of right child is out of bounds
// 	else
// 		return -1;
// }
// template <class T, class K>
// void BinaryHeap<T,K>::fixHeapDown(int i)
// {
// 	int child1=left(i);
// 	int child2=right(i);
	
// 	//J: if both the children are valid indices and the right child has the minimum value
// 	if(child1 >= 0 && child2 >= 0 && heap[child1].key > heap[child2].key)
// 	{
// 		child1=child2;
// 	}
// 	//J: if the minimum child is a valid index and the does not obey the heap property,swap
// 	if (child1 > 0 && heap[i].key > heap[child1].key)
// 	{
// 		swap(heap[i], heap[child1]);
// 		//J: checking if the swapped vertex obeys the heap property
// 		fixHeapDown(child1);
// 	}
// }

//   /*while (heap[i].key > heap[min_child].key)
//   {

//   //J: has no children
//   if(left> size()-1)  break;

//     swap(heap[i], heap[min_child]);
//     i=min_child;
//     left=2*i+1;
//   	right=2*i+2;

// 	// //J: has no children
// 	// if(left> size()-1)  break;

// 	//J: has left child only
// 	if(left < (size()-1) && right > (size()-1))  min_child=left;

// 	//J: has both children
// 	else min_child= heap[left].key < heap[right].key ? left : right;
//   }
// }*/

// template <class T, class K>
// void BinaryHeap<T,K>::popMin(){
// 	if(size()==0)
// 	{
// 		return;
// 	}
	
// 	swap(heap[0],heap[size()-1]);
// 	heap.pop_back();
// 	fixHeapDown(0);
// }


