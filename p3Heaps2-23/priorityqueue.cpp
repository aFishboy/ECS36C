#include <iostream>
#include <utility>
#include "json.hpp"

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {  
}

//method to print out all the nodes kv pairs for testing
void PriorityQueue::print(){
    for(size_t i = 1; i < max_size_+1; i++){    // loop for size of vector
        std::cout << "  Key: " << nodes_[i].first << " V1: " << nodes_[i].second.first  // print kv pairs
        << " V2: " << nodes_[i].second.second;  // print kv pairs
    }
    std::cout << std::endl; // formatting
}

void PriorityQueue::insert(Key k) { // method to insert a key with no values
	insert(std::make_pair(k, std::make_pair(0, 0)));    // make the key be paired with 0,0
}

void PriorityQueue::insert(KeyValuePair kv) {   // method to insert kv pairs
	nodes_[openSpot] = kv;  // make the open node = to kv pair
    heapifyUp(openSpot);    // heapify up to get it in position
    openSpot++; // increment the next open spot
    size_ = size(); // update size of vector
}

KeyValuePair PriorityQueue::min() { // method to get the minimun value
    return nodes_[ROOT];    // return the root
}

KeyValuePair PriorityQueue::removeMin() {   // method to remove min / root
	KeyValuePair minToReturn = nodes_[ROOT];    // make kv pair equal to root
    removeNode(ROOT);   // remove root
    return minToReturn; // return the old root
}

bool PriorityQueue::isEmpty() const {   // method to check if heap is empty
	if(nodes_[ROOT].first == 0) return true;    // if there is no root return true
    else return false;  // else return false
}

size_t PriorityQueue::size() const {    // mnethod to return size of heap
	return openSpot-1;  // size will be one less than open spot
}

nlohmann::json PriorityQueue::JSON(int operations) const {
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["maxHeapSize"] = max_size_;
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["numOperations"] = operations;
  result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {   // method to move kv pair into position up
    if(nodes_[i].first > nodes_[i/2].first || i/2 == 0){    // if child is > than parent or doesnt have parent return 
        return; //return
    }
    std::swap(nodes_[i], nodes_[i/2]);  // swap parent and child
    heapifyUp(i/2); // recursive call heapify up

}

void PriorityQueue::heapifyDown(size_t i) { //method to move kv pair into position down
    size_t smallerIndex = i;    // variable to store smallest index
    if(2*i < size_t(openSpot) && nodes_[2*i].first < nodes_[smallerIndex].first){   // if L child is in real and smaller 
        smallerIndex = 2*i; // change smallestindex to L child
    }
    if(2*i + 1 < size_t(openSpot) && nodes_[2*i + 1].first < nodes_[smallerIndex].first){ // if R child is in real and smaller 
        smallerIndex = 2*i + 1; // change smallestindex to R child
    }
    if(smallerIndex != i){  // if the smallerIndex changed
        std::swap(nodes_[i], nodes_[smallerIndex]); // swap with smaller key
        heapifyDown(smallerIndex);  // recursive call
    }
}

void PriorityQueue::removeNode(size_t i) {  // method to remove node
    if(int(i) > openSpot-1){    // if index is not valid 
        return; // return
    }
	nodes_[i] = nodes_[openSpot-1]; // replace node being deleted by bottom most node
    nodes_[openSpot-1] = nodes_[0]; // get rid of bottom most node 
    openSpot--; // decrease open spot
    size_ = size(); // update size
    heapifyDown(i); // call heapify down to adjust
    
}

Key PriorityQueue::getKey(size_t i) {   // method to get key
    return nodes_[i].first; // return key
}

