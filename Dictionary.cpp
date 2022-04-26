/****************************************************************************************
*  Darong Li, dli97
*  2022 Winter CSE101 PA8
*  Dictionary.cpp
*  Dictionary ADT (RED BLACK TREE)
*****************************************************************************************/
#include <iostream>
#include <string>
#include <stdexcept>
#include "Dictionary.h"

using namespace std;

Dictionary::Node::Node(keyType k, valType v){
	key = k;
	val = v;
	parent = nullptr;
	left = nullptr;
	right = nullptr;
	color = 0; // 0 = red, 1 = black
}

Dictionary::Dictionary(){
	nil = new Node("\0",-1);
	nil->color = 1;
	nil->left = nil;
	nil->right = nil;
	root = nil;

	current = nil;
	num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary& D){
	Node* R = D.root;
	Node* N = D.nil;
	this->nil = new Node("\0",-1);
	this->nil->color = 1;
	this->nil->left = this->nil;
	this->nil->right = this->nil;
	this->root = this->nil;
	this->current = this->nil;
	num_pairs = 0;
	this->preOrderCopy(R,N);
}

Dictionary::~Dictionary(){
	this->postOrderDelete(root);
	delete nil;
}

int Dictionary::size() const{
	return this->num_pairs;
}

bool Dictionary::contains(keyType k) const{
	Node* r = root;
	Node* b = search(r,k);
	if(b!=nil){
		return true;
	}
	else{
		return false;
	}
}

valType& Dictionary::getValue(keyType k) const{
	Node* r = root;
	Node* c = search(r,k);
	if(c == nil){
		throw std::logic_error("Dictionary: getValue(): key does not exist");
	}
	else{
		return c->val;
	}
}

bool Dictionary::hasCurrent() const{
	if(current == nil){
		return false;
	}
	else{
		return true;
	}
}

keyType Dictionary::currentKey() const{
	if(hasCurrent() == false){
		throw std::logic_error("Dictioary: currentKey(); current does not exist");
	}
	else{
		return current->key;
	}
}

valType& Dictionary::currentVal() const{
	if(hasCurrent() == false){
		throw std::logic_error("Dictioary: currentVal(); current does not exist");
	}
	else{
		return current->val;
	}
}

void Dictionary::clear(){
	this->postOrderDelete(root);
	this->root = nil;
	this->current = nil;
}

void Dictionary::setValue(keyType k, valType v){
	Node* y = this->nil;
	Node* x = this->root;
	while(x != nil){
		y = x;
		if(k < x->key){
			x = x->left;
		}
		else if( k > x->key){
			x = x->right;
		}
		else{
			y->val = v;
			return;
		}
	}
	Node* z = new Node(k,v);

	z->parent = y;
	if(y == nil){
		this->root = z;
	}
	else if(k < y->key){
		y->left = z;
	}
	else{
		y->right = z;
	}

	z->left = nil;
	z->right = nil;
	z->color = 0;
	num_pairs++;

	RB_InsertFixUp(z);
}

void Dictionary::RB_InsertFixUp(Node* N){
	while(N->parent->color == 0){
		if(N->parent == N->parent->parent->left){
			Node* y = N->parent->parent->right;
		
			if(y->color == 0){
				N->parent->color = 1;
				y->color = 1;
				N->parent->parent->color = 0;
				N = N->parent->parent;
			}
			else{
				if(N == N->parent->right){
					N = N->parent;
					LeftRotate(N);
				}
				N->parent->color = 1;
				N->parent->parent->color = 0;
				RightRotate(N->parent->parent);
			}
		}
		else{
			Node* y = N->parent->parent->left;
			if(y->color == 0){
				N->parent->color = 1;
				y->color = 1;
				N->parent->parent->color = 0;
				N = N->parent->parent;
			}
			else{
				if(N == N->parent->left){
					N = N->parent;
					RightRotate(N);
				}
				N->parent->color = 1;
				N->parent->parent->color = 0;
				LeftRotate(N->parent->parent);
			}
		}
	}
	root->color = 1;
}

void Dictionary::LeftRotate(Node* N){
	Node* y = N->right;

	N->right = y->left;
	
	if(y->left != nil){
		y->left->parent = N;
	}

	y->parent = N->parent;

	if(N->parent == nil){
		root = y;
	}
	else if( N == N->parent->left){
		N->parent->left = y;
	}
	else{
		N->parent->right = y;
	}
	y->left = N;
	N->parent = y;
}

void Dictionary::RightRotate(Node* N){

	Node* y = N->left;

	N->left = y->right;
	if(y->right != nil){
		y->right->parent = N;
	}

	y->parent = N->parent;
	if(N->parent == nil){
		root = y;
	}
	else if( N == N->parent->right){
		N->parent->right = y;
	}
	else{
		N->parent->left = y;
	}
	y->right = N;
	N->parent = y;
}

void Dictionary::RB_Transplant(Node* u, Node* v){
	if(u->parent == nil){
		root = v;
	}
	else if(u == u->parent->left){
		u->parent->left = v;
	}
	else{
		u->parent->right = v;
	}

	v->parent = u->parent;
}

void Dictionary::RB_Delete(Node* N){
	Node* y = N;
	Node* x;

	int y_color = y->color;

	if(N->left == nil){
		x = N->right;
		RB_Transplant(N,N->right);
		delete N;
		num_pairs--;
	}
	else if(N->right == nil){
		x = N->left;
		RB_Transplant(N,N->left);
		delete N;
		num_pairs--;
	}
	else{		
		y = findMin(N->right);
		y_color = y->color;
		x = y->right;
		if(y->parent == N){
			x->parent = y;
		}
		else{
			RB_Transplant(y,y->right);
			y->right = N->right;
			y->right->parent = y;
		}
		RB_Transplant(N,y);
		y->left = N->left;
		y->left->parent = y;
		y->color = N->color;
		delete N;
		num_pairs--;
	}
	
	if(y_color == 1){
		RB_DeleteFixUp(x);
	}
}

void Dictionary::RB_DeleteFixUp(Node* N){
	Node* x = N;
	while (x != root && x->color == 1){
		if (x == x->parent->left){
			Node* w = x->parent->right;
			if (w->color == 0){
				w->color = 1;                        // case 1
				x->parent->color = 0;                   // case 1
				LeftRotate(x->parent);                // case 1
				w = x->parent->right;                    // case 1
			}

			if (w->left->color == 1 && w->right->color == 1){
				w->color = 0;                          // case 2
            			x = x->parent;				// case 2
			}
         		else{
				if (w->right->color == 1){
					w->left->color = 1;                // case 3
               				w->color = 0;                       // case 3
               				RightRotate(w);                   // case 3
               				w = x->parent->right;                  // case 3
				}
            			w->color = x->parent->color;               // case 4
            			x->parent->color = 1;                 // case 4
            			w->right->color = 1;                  // case 4
            			LeftRotate(x->parent);                // case 4
            			x = root;                             // case 4
			}
		}
      		else{
         		Node* w = x->parent->left;
         		if( w->color == 0 ){
            			w->color = 1;                        // case 5
            			x->parent->color = 0;                   // case 5
           	 		RightRotate(x->parent);               // case 5
            			w = x->parent->left;                      // case 5
			}
         		if( w->right->color == 1 && w->left->color == 1){
            			w->color = 0;                          // case 6
            			x = x->parent;                           // case 6
			}
         		else{ 
            			if (w->left->color == 1){
               				w->right->color = 1;               // case 7
               				w->color = 0;                       // case 7
               				LeftRotate(w);                    // case 7
               				w = x->parent->left;                   // case 7
				}
            			w->color = x->parent->color;               // case 8
            			x->parent->color = 1;                 // case 8
            			w->left->color = 1;                   // case 8
            			RightRotate(x->parent);               // case 8
            			x = root;                             // case 8
			}
		}
	}
	x->color = 1;
}

void Dictionary::remove(keyType k){
	Node* N = search(root,k);
	if(N == nil){
		throw std::logic_error("Dictionary: remove(): no Node contains keyValue k");
	}
	else{
		if(current == N){
			current = nil;
		}
		RB_Delete(N);
	}
}

void Dictionary::begin(){
	if(size()!=0){
		current = findMin(root);
	}
}

void Dictionary::end(){
	if(size() != 0){
		current = findMax(root);
	}
}

void Dictionary::next(){
	if(hasCurrent() == true){
		if(current != findMax(root)){
			current = findNext(current);
		}
		else{
			current = nil;
		}
	}
	else{
		throw std::logic_error("Dictionary: next(): current is max");
	}
}

void Dictionary::prev(){
	if(hasCurrent() == true){
		if(current != findMin(root)){
			current = findPrev(current);
		}
		else{
			current = nil;
		}
	}
	else{
		throw std::logic_error("Dictionary: prev(): current is min");
	}
}

std::string Dictionary::to_string() const{
	Node* N = root;
	string s;
	inOrderString(s,N);
	return s;
}

std::string Dictionary::pre_string() const{
	Node* N = root;
	string s;
	preOrderString(s,N);
	return s;
}

bool Dictionary::equals(const Dictionary& D) const{
	if(size() == 0 && D.size() == 0){
		return true;
	}
	else if(size() != D.size()){
		return false;
	}
	else{
		bool c;
		if(to_string() == D.to_string()){
			c = true;
		}
		else{
			c = false;
		}
		return c;
	}
}

// Helper Functions

void Dictionary::inOrderString(std::string& s, Node* R) const{
	if(R != this->nil){
		inOrderString( s, R->left );

		s += R->key;
		s += " : ";
		s += std::to_string(R->val);
		s += '\n';

		inOrderString( s, R->right );
	}
}

void Dictionary::preOrderString(std::string& s, Node* R) const{
	if(R != this->nil){
		s += R->key;
		s += '\n';

		preOrderString(s, R->left);
		preOrderString(s, R->right);
	}
}

void Dictionary::preOrderCopy(Node* R, Node* N){
	if(R != N){
		keyType R_key = R->key;
		valType R_val = R->val;
		setValue(R_key, R_val);

		preOrderCopy(R->left,N);
		preOrderCopy(R->right,N);
	}
}

void Dictionary::postOrderDelete(Node* R){
	if(R != nil){
		postOrderDelete(R->left);
		postOrderDelete(R->right);
		delete R;
		num_pairs--;
	}
}

Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
	Node* N = R;
	if(N==nil || k == N->key){
		return N;
	}
	else if(k < N->key){
		return search(N->left,k);
	}
	else{
		return search(N->right,k);
	}
}

Dictionary::Node* Dictionary::findMin(Node* R){
	if(R == nil){
		throw std::logic_error("Dictionary: findMin(): R is nil");
	}
	Node* N = R;
	while(N->left != nil){
		N = N->left;
	}
	return N;
}

Dictionary::Node* Dictionary::findMax(Node* R){
	if(R == nil){
		throw std::logic_error("Dictionary: findMax(): R is nil");
	}
	Node* N = R;
	while(N->right != nil){
		N = N->right;
	}
	return N;
}

Dictionary::Node* Dictionary::findNext(Node* R){
	if(R->right != nil){
		return findMin(R->right);
	}
	Node* y = R->parent;
	while(y != nil && R == y->right){
		R = y;
		y = y->parent;
	}
	return y;
}

Dictionary::Node* Dictionary::findPrev(Node* R){
	if(R->left != nil){
		return findMax(R->left);
	}
	Node* y = R->parent;
	while(y != nil && R == y->left){
		R = y;
		y = y->parent;
	}
	return y;
}

std::ostream& operator<<( std::ostream& stream, Dictionary& D){
	return stream << D.to_string();
}

bool operator==(const Dictionary&A, const Dictionary& B){
	return A.equals(B);
}

Dictionary& Dictionary::operator=(const Dictionary& D){
	if(this != &D){
		Dictionary temp = D;
		std::swap(nil,temp.nil);
		std::swap(root,temp.root);
		std::swap(current,temp.current);
		std::swap(num_pairs,temp.num_pairs);
	}
	return *this;
}

