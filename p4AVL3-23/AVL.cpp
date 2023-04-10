#include "AVL.h"

#include <cassert>
#include <iostream>
#include <string>
#include <queue>
#include <algorithm>



AVLNode::AVLNode(int key) :
	key_(key),
	parent_(std::weak_ptr<AVLNode>()),
	left_(nullptr),
	right_(nullptr) {}

AVLNode::AVLNode(int key, std::weak_ptr<AVLNode> parent) :
	key_(key),
	parent_(parent),
	left_(nullptr),
	right_(nullptr) {}

bool AVLNode::IsLeaf() const {
	return left_ == nullptr && right_ == nullptr;
}

bool AVLNode::HasLeftChild() const {
	return left_ != nullptr;
}

bool AVLNode::HasRightChild() const {
	return right_ != nullptr;
}

void AVLNode::DeleteChild(std::shared_ptr<AVLNode> v) {
	if (left_ == v) {
		left_ = nullptr;
	} else if (right_ == v) {
		right_ = nullptr;
	} else {
		std::cerr << "AVLNode::DeleteChild Error: non-child passed as argument\n";
		exit(EXIT_FAILURE);
	}
}

void AVLNode::ReplaceChild(std::shared_ptr<AVLNode> v, std::shared_ptr<AVLNode> u) {
	if (left_ == u || right_ == u) {
		std::cerr << "AVLNode::ReplaceChild Error: child passed as replacement\n";
	}
	if (left_ == v) {
		left_ = u;
		u->parent_ = v->parent_;
	} else if (right_ == v) {
		right_ = u;
		u->parent_ = v->parent_;
	} else {
		std::cerr << "AVLNode::ReplaceChild Error: non-child passed as argument\n";
		exit(EXIT_FAILURE);
	}
}

AVL::AVL() : root_(nullptr), size_(0) {}

void AVL::Insert(int key) {
	if (root_ == nullptr) {
		root_ = std::make_shared<AVLNode>(key);
		root_->height = 0; // if AVL is empty first node has height 0
		size_++;
		assert(root_->left_ == nullptr && root_->right_ == nullptr);	//check to see if first insert has no children
		return;
	}
	std::shared_ptr<AVLNode> currentNode = root_, lastNode = nullptr;
	
	while (currentNode != nullptr) {	// loop to walk down tree
		lastNode = currentNode;	// update lastNode to currentNode
		currentNode = (key < currentNode->key_) ?	// if key trying to insert is less than currentNode key then go left else go right
			currentNode->left_ : currentNode->right_;	//go left else go right down tree
	}
	if (key < lastNode->key_) {	// if key trying to insert is smaller than bottom most node insert as left child
		lastNode->left_ = std::make_shared<AVLNode>(key, lastNode);	// left child becomes new node
		updateAfterInsert(lastNode->left_);	// update nodes
	} else {	// else it is right child
		lastNode->right_ = std::make_shared<AVLNode>(key, lastNode);// right child becomes new node
		updateAfterInsert(lastNode->right_);// update nodes
	}	
	size_++;	//increment size
}

bool AVL::Delete(int key) {
	std::shared_ptr<AVLNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			if (currentNode->IsLeaf()) {
				DeleteLeaf(currentNode);
			} else if (currentNode->left_ == nullptr) {
				assert(currentNode->right_ != nullptr);
				std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
				parent->ReplaceChild(currentNode, currentNode->right_);
				size_--; assert(size_ >= 0);
			} else if (currentNode->right_ == nullptr) {
				assert(currentNode->left_ != nullptr);
				std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
				parent->ReplaceChild(currentNode, currentNode->left_);
				size_--; assert(size_ >= 0);
			} else {
				currentNode->key_ = DeleteMin(currentNode);
			}
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

int AVL::DeleteMin() {
	return DeleteMin(root_);
}


void AVL::DeleteLeaf(std::shared_ptr<AVLNode> currentNode) {
	std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
	if (parent == nullptr) {
		// Delete root
		root_ = nullptr;
		size_--; assert(size_ == 0);
	} else {
		if (parent->right_ == currentNode) {
			parent->right_ = nullptr;
		} else if (parent->left_ == currentNode) {
			parent->left_ = nullptr;
		} else {
			std::cerr << "AVL::DeleteLeaf Error: inconsistent state\n";
		}
		size_--; assert(size_ >= 0);
	}
}

int AVL::DeleteMin(std::shared_ptr<AVLNode> currentNode) {
	std::shared_ptr<AVLNode> lastNode = nullptr;
	while (currentNode != nullptr) {
		lastNode = currentNode;
		currentNode = currentNode->left_;
	}
	int result = lastNode->key_;
	std::shared_ptr<AVLNode> parent = lastNode->parent_.lock();
	if (parent == nullptr) {
		// lastNode is root
		if (lastNode->right_ != nullptr) {
			root_ = lastNode->right_;
			lastNode->right_->parent_.reset();
		} else {
			root_ = nullptr;
		}
	} else {
		// lastNode under the root
		if (lastNode->right_ != nullptr) {
			parent->left_ = lastNode->right_;
			lastNode->right_->parent_ = parent;
		} else {
			parent->left_ = nullptr;
		}
  }
	size_--; assert(size_ >= 0);
	return result;
}

//function to walk up tree and check each node and update/balance as necesarry
void AVL::updateAfterInsert(std::shared_ptr<AVLNode> currentNode){
	int currentHeight = 0;	//start at bottom which is height 0
	std::shared_ptr<AVLNode> previousNode = nullptr;	//pointer to keep track of previous node we stopped at
	std::shared_ptr<AVLNode> prePreNode = nullptr;	//pointer to keep track of previous previous node we stopped at
	std::shared_ptr<AVLNode> holder = currentNode;//temp pointer
	while (currentNode != nullptr){ //while we haven't gone above the root ie currentNode is a real node
		changeHeightAndBF(currentNode);	// update the nodes height and BF
		if (abs(currentNode->balanceFactor) == 2){	//if node has a balance factor of 2 then enter
			Balance(currentNode, previousNode, prePreNode);	// call balance method to fix
			currentHeight = std::max(prePreNode->height, prePreNode->height);//update current height of node we are at after balance
		}

		assert(abs(currentNode->balanceFactor) < 2);//assert to check that after balancing currentNode is balanced
		if (currentNode->IsLeaf()){	//if node is a leaf enter
			assert(currentNode->height == 0);//assert to check leaf has height 0
			assert(currentNode->balanceFactor == 0);//assert to check leaf has BF 0
		}
		else if (currentNode->HasLeftChild() && currentNode->HasRightChild()){ //if node has left and right child enter
			assert(currentNode->height == std::max(currentNode->left_->height, currentNode->right_->height) + 1);//assert to check that height of node is max of right and left
		}
		else if (currentNode->HasLeftChild()){ //if currentNode only has left 
			assert(currentNode->height == currentNode->left_->height + 1);//assert to check that height is equation
		}
		else{//else node only has right child
			assert(currentNode->height == currentNode->right_->height + 1);//assert to check that height is equation
		}
		prePreNode = previousNode;//update previous previous node
		previousNode = currentNode;//update previous node
		holder = currentNode;// update holder
		std::shared_ptr<AVLNode> parent = holder->parent_.lock(); //temp pointer to parent
		currentNode = parent;// make currentNode=parent
		holder->parent_.reset();//reset pointer
		holder->parent_ = currentNode;//add back weak pointer
		currentHeight += 1;	//increment height
	}
}

//function to update height and BF
void AVL::changeHeightAndBF(std::shared_ptr<AVLNode> currentNode){
	if (currentNode->HasLeftChild() && currentNode->HasRightChild()){//if node has both childen
	currentNode->height = std::max(currentNode->left_->height, currentNode->right_->height) + 1;//height is the max height of the children
	currentNode->balanceFactor = currentNode->right_->height - currentNode->left_->height;//BF is the equation for BF of childrens height
	}
	else if (currentNode->HasLeftChild()){//node only has left
		currentNode->height = currentNode->left_->height + 1;//height is the equation for height with 1 child
		currentNode->balanceFactor = -1 - currentNode->left_->height;//BF is the equation for BF with 1 child
	}
	else if (currentNode->HasRightChild()){//node only had right 
		currentNode->height = currentNode->right_->height + 1;//height is the equation for height with 1 child
		currentNode->balanceFactor = currentNode->right_->height + 1;//BF is the equation for BF with 1 child
	}
	else{//node had no children
		currentNode->height = 0;//height is 0 cause no children
		currentNode->balanceFactor = 0;//BF is 0 cause no children
	}
}

//function to call rotations of unbalanced tree
void AVL::Balance(std::shared_ptr<AVLNode> currentNode, std::shared_ptr<AVLNode> previousNode, std::shared_ptr<AVLNode> prePreNode){
	if(currentNode->balanceFactor == -2){//if top node has left problem
		if(previousNode->balanceFactor == -1){ //if next node has left problem
			RightRotate(currentNode, previousNode);	//LL problem so call right rotate on top node to fix
		}
		else{ // LR problem
			LeftRotate(previousNode, prePreNode);//call left rotate on middle node to turn it to RR problem
			RightRotate(currentNode, prePreNode);//call R rotate on top to fix

		}
	}
	else{ 
		if(previousNode->balanceFactor == 1){	// RR problem
			LeftRotate(currentNode, previousNode);// Left rotate on top node to fix
		}
		else{	// RL problem
			RightRotate(previousNode, prePreNode);//call R rotate on middle node to turn it to RR problem
			LeftRotate(currentNode, prePreNode);//call L rotate on top to fix
		}
	}

}

void AVL::RightRotate(std::shared_ptr<AVLNode> currentNode, std::shared_ptr<AVLNode> previousNode){
	currentNode->left_ = previousNode->right_;//make cN L = to the node below its right
	if (previousNode->HasRightChild()){	// check to see if pN had right child
		previousNode->right_->parent_ = currentNode;//if has right then make the rights parent cN
	}

	previousNode->right_ = currentNode;	//update pN right to be cN
	
	if (currentNode != root_){	//if cN is not the root enter
		previousNode->parent_ = currentNode->parent_;	//make pN parent = to cN parent
		std::shared_ptr<AVLNode> parent = previousNode->parent_.lock();//temp pointer to node above cN and now pN since they have same parent rn
		if (parent->right_ == currentNode){// if parent right is cN
			parent->right_ = previousNode;//parent right is now pN
		}
		else{	//else
			parent->left_ = previousNode;//make parent left pN
		}
		previousNode->parent_.reset();//reset temp node
		previousNode->parent_ = currentNode->parent_;	//pN parent is now cN parent
	}
	else{	// else  cN is root
		previousNode->parent_.reset();	//reset because it has no parent
		root_ = previousNode;//make root pN
	}
	
	currentNode->parent_ = previousNode;//lastly make cN parent = pN

	
	changeHeightAndBF(currentNode);//update cN
	changeHeightAndBF(previousNode);//and update pN
}
void AVL::LeftRotate(std::shared_ptr<AVLNode> currentNode, std::shared_ptr<AVLNode> previousNode){
	currentNode->right_ = previousNode->left_;//make cN R = to the node below its left
	if (previousNode->HasLeftChild()){// check to see if pN had right child
		previousNode->left_->parent_ = currentNode;//if has left then make the left parent cN
	}

	previousNode->left_ = currentNode;//update pN left to be cN
	
	if (currentNode != root_){//if cN is not the root enter
		previousNode->parent_ = currentNode->parent_;//make pN parent = to cN parent
		std::shared_ptr<AVLNode> parent = previousNode->parent_.lock();//temp pointer to node above cN and now pN since they have same parent rn
		if (parent->right_ == currentNode){// if parent right is cN
			parent->right_ = previousNode;//parent right is now pN
		}
		else{//else
			parent->left_ = previousNode;//make parent left pN
		}
		previousNode->parent_.reset();//reset temp node
		previousNode->parent_ = currentNode->parent_;//pN parent is now cN parent
	}
	else{// else  cN is root
		previousNode->parent_.reset();//reset because it has no parent
		root_ = previousNode;//make root pN
	}
	currentNode->parent_ = previousNode;//lastly make cN parent = pN
	
	changeHeightAndBF(currentNode);//update cN
	changeHeightAndBF(previousNode);//and update pN
}

size_t AVL::size() const {
	return size_;
}

bool AVL::empty() const {
	return size_ == 0;
}

bool AVL::Find(int key) const {
	std::shared_ptr<AVLNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			return true;
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

std::string AVL::JSON() const {
	nlohmann::json result;
	std::queue< std::shared_ptr<AVLNode> > nodes;
	if (root_ != nullptr) {
		result["root"] = root_->key_;
		result["height"] = root_->height;//add height to the output json
		nodes.push(root_);
		while (!nodes.empty()) {
			auto v = nodes.front();
			nodes.pop();
			std::string key = std::to_string(v->key_);
			result[key]["height"] = v->height;//add height to the output json
			result[key]["balance factor"] = v->balanceFactor;//add BF to the output json
			if (v->left_ != nullptr) {
				result[key]["left"] = v->left_->key_;
				nodes.push(v->left_);
			}
			if (v->right_ != nullptr) {
				result[key]["right"] = v->right_->key_;
				nodes.push(v->right_);
			}
			if (v->parent_.lock() != nullptr) {
				result[key]["parent"] = v->parent_.lock()->key_;
			} else {
				result[key]["root"] = true;
			}
		}
	}
	else{ // tree is empty
		result["root"] = "NONE";//no root
		result["height"] = 0;// no height so height = 0
	}
	result["size"] = size_;
	return result.dump(2) + "\n";
}
