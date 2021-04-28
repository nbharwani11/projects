#include "LinkedList.h"

// constructor, initialize class variables and pointers here if need.
LinkedList::LinkedList(){
  // Your code here
  // make top_ptr_ NULL
  shared_ptr<node> top_ptr_ = NULL;
}

//deconstructor,
LinkedList::~LinkedList(){
  // add nothing here since shared ptrs automatically deallocated memory for you from the heap
}


shared_ptr<node> LinkedList::InitNode(int data){
  // Your code here
  // first create a new node using shared ptrs from heap memory
  shared_ptr<node> ret(new node);
  // then store data value passed into InitNode into the newly created node called ret
  ret->data = data;
  // return pointer to the new node ret
  return ret;
}

string LinkedList::Report(){
  // Your code here
  // initialize a new string called ret to store contents from list in
  string ret = "";
  // use a helper function to recursively call the lsit until the string is filled out in ret
  ret = ReportRecursive(top_ptr_);
  // return the string ret with the contents from the list
  return ret;
}

string LinkedList::ReportRecursive(shared_ptr<node> cursor) {
  // check to see if the cursor is NULL, then you are done adding from the list
  if (cursor == NULL) {
    // return an empty string back
    return "";
  }
  // add cursor's data into the new string called ret after the previous additions to it
  // and followed by a space after the new value is added for the next in the list
  // convert cursor's data to a string as well
  // then recursively call ReportRecursive with cursor->next node
  // now cursor has to be set to the next value in the list to add to string ret or return if list is empty
  return to_string(cursor->data) + " " + ReportRecursive(cursor->next);
}

void LinkedList::AppendData(int data){
  // Your code here
  // create a new shared ptr called new_node
  shared_ptr<node> new_node;
  // pass data given to AppendData function to InitNode
  // then store the new node in new_node
  new_node = InitNode(data);
  // now pass the newly created shared ptr called new_node to Append helper function
  Append(new_node);
}

void LinkedList::Append(shared_ptr<node> new_node){
  // Your code here
  // create a new shared ptr called cursor that points to top_ptr_
  shared_ptr<node> cursor = top_ptr_;
  // check if list is empty
  if (top_ptr_ == NULL) {
    // if top_ptr_ is NULL append node to top of list and return
    top_ptr_ = new_node;
    return;
  }
  // if top_ptr_ not NULL then pass new_node and cursor into the AppendRecursive function
  AppendRecursive(new_node, cursor);
}

void LinkedList::AppendRecursive(shared_ptr<node> new_node, shared_ptr<node> cursor) {
  // check to see if the next node after cursor is NULL
  // if yes then add it to the list
  if (cursor->next == NULL) {
    // point posiiton after cursor to be new_node
    // store poisition after cursor's data to be new_node's data
    cursor->next = new_node;
    // point two positions after cursor, or position after new_node to be NULL
    cursor->next->next = NULL;
    return;
  }
  // if next position after cursor is not NULL recursively call AppendRecursive
  // send it cursor->next for the new cursor position
  AppendRecursive(new_node, cursor->next);
}

void LinkedList::InsertData(int offset, int data){
  // Your code here
  // create a new shared ptr called new_node
  shared_ptr<node> new_node;
  // pass data to InitNode to create new node, store in new_node shared ptr
  new_node = InitNode(data);
  // pass offset value and new_node to Insert helper function
  Insert(offset, new_node);
}

void LinkedList::Insert(int offset, shared_ptr<node> new_node){
  // Your code here
  // create a shared ptr called cursor to point to top_ptr_
  shared_ptr<node> cursor = top_ptr_;
  // first map out your cases
  // case 1: if offset is beyond the scope of the list, use size function
  if (offset > Size()) {
    return;
  }
  // case 2: if offset is equal to the size of the list then append to end of list using Append, use size function
  else if (offset == Size()) {
    Append(new_node);
  }
  // case 3: if offset is equal to 0
  else if (offset == 0) {
    new_node->next = cursor;
    top_ptr_ = new_node;
  }
  // case 4: if offset is in between size of list then recursively call InsertRecursive
  // next pass offset, new_node and cursor to InsertRecursive helper function to Insert new node
  // create new int variable of size to point to offset starting value
  else {
    int size = offset;
    // create new shared ptr called prev_node and make it equal to cursor
    shared_ptr<node> prev_node = cursor;
    InsertRecursive(size, new_node, prev_node, cursor);
  }
}

void LinkedList::InsertRecursive(int offset, shared_ptr<node> new_node, shared_ptr<node> prev_node, shared_ptr<node> cursor) {
  // case 4: if offset is in between size of list then recursively call InsertRecursive
  if (offset == 0) {
    prev_node->next = new_node;
    new_node->next = cursor;
    return;
  }
  offset--;
  prev_node = cursor;
  InsertRecursive(offset, new_node, prev_node, cursor->next);
}

void LinkedList::Remove(int offset){
  // Your code here
  // create a shared ptr called cursor to point to top_ptr_
  shared_ptr<node> cursor = top_ptr_;
  // case 1: if offset less than 0 or greater than Size then just return
  if (offset < 0 || offset > Size()) {
    // then just return because offset is an invalid number
    return;
  }

  // case 2: if offset is equal to 0, remove top_ptr_
  else if(offset == 0) {
    // remove top_ptr_ by pointing the next node in the list to be top_ptr_
    top_ptr_ = cursor->next;
    // then just return because you are done removing top_ptrr_
    return;
  }

  // create new int variable called size to point to offset value
  int size = offset;
  // create a new shared ptr called prev_node to point to cursor's node
  shared_ptr<node> prev_node = cursor;
  // case 4: any offset between 1 to the Size() of the list
  // use RemoveRecursive as a recursive helper function to Remove
  // pass size equal to offset value and cursor to the function
  RemoveRecursive(size, cursor, prev_node);
}

void LinkedList::RemoveRecursive(int size, shared_ptr<node> cursor, shared_ptr<node> prev_node){
  // Your code here
  // case 4: traverse list until you are at the offset to remove the node listed at that point
  // if size is equal to 0 you are at the position to repoint cursor to
  if (size == 0) {
    prev_node->next = cursor->next;
    return;
  }
  size--;
  prev_node = cursor;
  RemoveRecursive(size, cursor->next, prev_node);
}

int LinkedList::Size(){
  // Your code here
  // intialize a new int variable called ret to store size of list
  int ret = 0;
  //create a new shared ptr called cursor that points to top_ptr_
  shared_ptr<node> cursor = top_ptr_;
  // pass cursor and size to SizeRecursive to find size of a list
  // store return value in ret
  SizeRecursive(cursor, ret);
  return ret;
}

void LinkedList::SizeRecursive(shared_ptr<node> cursor, int& size) {
  // if cursor is not NULL call SizeRecursive again
  if (cursor == NULL) {
    // list has ended, return size to Size() function
    return;
  }
  // add one to size each time cursor is not NULL
  size += 1;
  // traverse list to position after cursor by calling SizeRecursive again
  SizeRecursive(cursor->next, size);
}

bool LinkedList::Contains(int data){
  bool ret = false;
  // Your code here
  // create a shared ptr called cursor to point to top_ptr_
  shared_ptr<node> cursor = top_ptr_;
  // recursively call ContainsRecursive helper function returning value of ret
  ContainsRecursive(cursor, ret, data);
  // return ret with either truew if data found or false if data not found
  return ret;
}

void LinkedList::ContainsRecursive(shared_ptr<node> cursor, bool& ret, int data) {
  // if cursor is NULL then list ended and data not found
  if (cursor == NULL) {
    return;
  }
  // if data is equal to cursor's data then data was found and return ret
  else if(cursor->data == data) {
    ret = true;
    return;
  }
  // if list not at end yet and data was not found yet then go to the next node in the list to check it
  ContainsRecursive(cursor->next, ret, data);
}

// This function is implemented for you
// It returns the top pointer
shared_ptr<node> LinkedList::GetTop(){
  return top_ptr_;
}

// This function is implemented for you
// It sets a given pointer as the top pointer
void LinkedList::SetTop(shared_ptr<node> top_ptr){
  top_ptr_ = top_ptr;
}
