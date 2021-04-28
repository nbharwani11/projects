#include "BST.h"

// constructor, initialize class variables and pointers here if need.
BST::BST(){
  // Your code here
  // initialize root_ptr_ to be NULL
  shared_ptr<bst_node> root_ptr_ = NULL;
}

//deconstructor,
BST::~BST(){
  // don't need to add anything here since shared ptrs automatically deallocate memory
}

shared_ptr<bst_node> BST::InitNode(int data){
  // Your code here
  // create a new shared ptr node called ret via the heap
  shared_ptr<bst_node> ret(new bst_node);  
  // store data passed to InitNode function into new node called ret's data
  ret->data = data;
  // point ret's left and right children to be NULL
  ret->left = NULL;
  ret->right = NULL;
  // return a pointer to the new node ret
  return ret;
}

void BST::InsertRecursive(shared_ptr<bst_node> new_node, shared_ptr<bst_node> cursor) {
  // check to see if cursor is NULL then just return 
  if (cursor == NULL) {
    return;
  } 
  // two cases:
  // case 1: go left if new_node's data is less than cursor's data
  else if (new_node->data < cursor->data) {
    if (cursor->left == NULL) {
      // check to see if cursor's left child is NULL
      // if NULL insert new_node here
      cursor->left = new_node;
    }
    else {
      InsertRecursive(new_node, cursor->left);
    }
  }
  // go right if new_node's data is greater than or equal to cursor's data
  else {
    if (cursor->right == NULL) {
      cursor->right = new_node;
    }
    else {
      InsertRecursive(new_node, cursor->right);
    }
  }
}

void BST::Insert(shared_ptr<bst_node> new_node){
  // Your code here
  // create a new shared ptr called cursor to point to root_ptr_
  shared_ptr<bst_node> cursor = root_ptr_;
  // first mark out your cases
  // case 1: if the tree is empty insert into the root node
  if (cursor == NULL) {
    // new_node is inserted as the root_ptr_ node
    root_ptr_ = new_node;
    return;
  }
  // case 2: tree is not empty pass to InsertRecursive helper function
  InsertRecursive(new_node, cursor);
}

void BST::InsertData(int data){
  // Your code here
  // create a new shared ptr called new_node
  shared_ptr<bst_node> new_node;
  // pass data given to InsertData function to InitNode function
  // store return value in new_node
  new_node = InitNode(data);
  // pass newly create node called new_node to the Insert helper function to insert new_node
  Insert(new_node);
}

void BST::Remove(int data){
  // Your code here
}

void BST::RemoveRecursive(int data) {
  
}

bool BST::Contains(shared_ptr<bst_node> subt, int data){
  // Your code here
  // create a new bool variable called ret
  bool ret = false;
  // create a new shared ptr called node_info to store the node's info
  shared_ptr<bst_node> node_info;
  // pass subt and data to GetNode to see if the data is within the subt
  node_info = GetNode(subt, data);
  // if node_info is NULL then the node was not found within subt
  // return ret is false
  if (node_info == NULL) {
    return ret;
  }
  // if node_info is not NULL then the data was found in the subt, return true
  else {
    ret = true;
  }
  /* 
  // second option without using GetNode
  // create a new shared ptr called cursor that points to subt
  shared_ptr<bst_node> cursor = subt;
  // case 1: if cursor is NULL, list is empty and just return false
  if (cursor == NULL) {
    // data is not included in BST ret is false
    ret = false;
    // return ret
    return ret;
  }
  // if list not empty called recursive helper function ContainsRecursive
  ContainsRecursive(cursor, data, ret);
  // return ret's value */
  return ret;
}

/* bool BST::ContainsRecursive(shared_ptr<bst_node> cursor, int data, bool& ret){
  // if cursor is NULL we have traversed the whole list, and data was not found so return false
  if (cursor == NULL) {
    return ret;
  }
  // if data is equal to cursor's data then you found the data in the BST and return ret as true
  else if (cursor->data == data) {
    ret = true;
    return ret;
  }
  else if (data < cursor->data) {
    ContainsRecursive(cursor->left, data, ret);
  }
  else { 
    ContainsRecursive(cursor->right, data, ret);
  }
} */

shared_ptr<bst_node> BST::GetNode(shared_ptr<bst_node> subt, int data){
  // Your code here
  // create a new shared ptr called ret
  shared_ptr<bst_node> ret = NULL;
  // create a new shared ptr called cursor that points to subt
  shared_ptr<bst_node> cursor = subt;
  // case 1: if cursor is NULL, list is empty and just return ret's value as NULL
  if (cursor == NULL) {
    // return ret
    return ret;
  }
  // if list not empty called recursive helper function ContainsRecursive
  GetNodeRecursive(cursor, data, ret);
  // return ret's value
  return ret;
  
}

shared_ptr<bst_node> BST::GetNodeRecursive(shared_ptr<bst_node> cursor, int data, shared_ptr<bst_node>& ret) {
  // if cursor is NULL we have traversed the whole list, and data was not found so return NULL for ret
  if (cursor == NULL) {
    return ret;
  }
  // if data is equal to cursor's data then you found the data in the BST and return ret as a pointer to the node found
  else if (cursor->data == data) {
    ret = cursor;
    return ret;
  }
  // if cursor's data was not equal to data and list not empty yet keep traversing list
  // look to the left of the BST if data is less than cursor's data
  else if (data < cursor->data) {
    GetNodeRecursive(cursor->left, data, ret);
  }
  // look to the right of the BST if data is greater than or equal to cursor's data
  else { 
    GetNodeRecursive(cursor->right, data, ret);
  }
}

int BST::Size(shared_ptr<bst_node> subt){
  // Your code here
  // initialize an int variable called size to store size of subt
  int size = 0;
  // create a new ptr called cursor that points to subt
  shared_ptr<bst_node> cursor = subt;
  // pass size and cursor to SizeRecursive helper function to traverse subt and calculate size of subt
  SizeRecursive(cursor, size);
  return size;
}

int BST::SizeRecursive(shared_ptr<bst_node> cursor, int& size) {
  if (cursor == NULL) {
    return size;
  }
  size++;
  SizeRecursive(cursor->left, size);
  SizeRecursive(cursor->right, size);
}

void BST::ToVector(shared_ptr<bst_node> subt, vector<int>& vec){
  // Your code here
  // eventually fill in vec ex. vec[0] = 1;
  // use vec.push_back(a)
  // create a shared ptr called cursor that points to subt
  shared_ptr<bst_node> cursor = subt;
  // pass cursor and vec to recursive ToVectorRecursive helper function
  ToVectorRecursive(cursor, vec);
}

void BST::ToVectorRecursive(shared_ptr<bst_node> cursor, vector<int>& vec) {
  // if cursor is NULL then we have traversed the whole list and return
  if (cursor == NULL) {
    return;
  }
  // if list has not been fully traversed, go check left subtree first
  ToVectorRecursive(cursor->left, vec);
  // store furtherest left subtree value and then make your way up the left subtree to root node
  vec.push_back(cursor->data);
  // go check right subtree and then make your way up the right subtree to root node
  ToVectorRecursive(cursor->right, vec);
}

// This function is implemented for you
// Returns the root pointer
shared_ptr<bst_node> BST::GetRoot(){
  return root_ptr_;
}

// This function is implemented for you
// sets a given pointer as the top pointer
void BST::SetRoot(shared_ptr<bst_node> root_ptr){
  root_ptr_ = root_ptr;
}