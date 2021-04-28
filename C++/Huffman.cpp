#include "Huffman.h"

//********************************************************************************************************************
//                                        CONSTRUCTOR/DECONSTRUCTOR
//********************************************************************************************************************



// constructor, initialize class variables and pointers here if need.
Huffman::Huffman(){
  // your code here
  // no need to add anything here since there is nothing to initialize
}

//deconstructor,
Huffman::~Huffman(){
  // no need to add anything here since smart pointers automatically deallocate memory for you
}

//********************************************************************************************************************
//                                        CREATELEAF FUNCTION
//********************************************************************************************************************



// CreateLeaf initializes a freq_info from heap memory and returns a
// pointer to it. the returned object's symbol and count fields are
// set to those provided; left and right child pointers are NULL, and
// it is flagges as a leaf node.
shared_ptr<freq_info> Huffman::CreateLeaf(char symbol, int count) {
  // TODO
  // initialize a new freq info shared ptr called new_node from heap memory
  shared_ptr<freq_info> new_node(new freq_info);
  // initialize symbol as symbol paramter
  new_node->symbol = symbol;
  // initialize count as count parameter
  new_node->count = count;
  // initialize left and right children as NULL
  new_node->left = NULL;
  new_node->right = NULL;
  // initialize is_leaf to be true
  new_node->is_leaf = true;
  // return the newly made Huffman node called new_node 
  // with symbol, count, left and right children initializes
  return new_node;
}


//********************************************************************************************************************
//                                        COMBINENODES FUNCTION
//********************************************************************************************************************



// CombineNodes initializes a freq_info from the heap and returns a
// pointer to it. The new node has left and right pointers set to the
// values given. The symbol data for this node is undefined (since it
// is an internal node), and is_leaf is false.
shared_ptr<freq_info> Huffman::CombineNodes(shared_ptr<freq_info> left, shared_ptr<freq_info> right) {
  // TODO
  // initialize new freq_info shared ptr called new_node from heap memory
  shared_ptr<freq_info> comb(new freq_info);
  // initialize left child to be left passed parameter
  comb->left = left;
  // initialize right child to be right passed parameter
  comb->right = right;
  // initialize is_leaf to be false since it is an internal node
  // combo of two or more child nodes together
  comb->is_leaf = false;
  // comb left's count with right's count
  comb->count = left->count + right->count;
  // return the newly made Huffman node claled new_node
  // with left and right children initialized as the parameters passed in
  return comb;
}


//********************************************************************************************************************
//                                        INCREMENTLOOKUP FUNCTION
//********************************************************************************************************************

// IncrementLookup will add one to the value associated with the
// symbol, or if the symbol is not yet a key in the map, it will
// create a mapping lookup[symbol] = 1.
void Huffman::IncrementLookup(map<char, int>& lookup, char symbol) {
  // TODO
  // increment symbol up by 1, or this adds symbol to the lookup with value 1
  lookup[symbol] = lookup[symbol] + 1;
  return;
}


//********************************************************************************************************************
//                                        LOADQUEUE FUNCTION
//********************************************************************************************************************


// LoadQueue turns each key/value pair in the lookup table into a
// freq_info pointer, and pushes it into the provided priority queue
// (use q.push(node)).
void Huffman::LoadQueue(const map<char, int>& lookup, tree_queue& q) {
  // TODO
  for (auto it = lookup.cbegin(); it != lookup.cend(); ++it) {
    char k = (*it).first;
    int v =  (*it).second;
  
    // create a new freq_info shared ptr called node
    shared_ptr<freq_info> node = CreateLeaf(k, v);

    // store node into the priority queue using push
    q.push(node);
  }
}


//********************************************************************************************************************
//                                        BUILDTREE FUNCTION
//********************************************************************************************************************

// When accessing items from the queue, use top() to read it, and
// pop() to remove it.
//
// The first item removed should form a left node, the second item
// should form the right. That's for unit testing purposes.
//
// BuildTree creates a Huffman codec tree using the values contained
// in the provided priority queue. it returns a pointer to the root
// node.
shared_ptr<freq_info> Huffman::BuildTree(tree_queue& q) {
  // TODO
  // if queue's size is one then just record last index in queue
  if (q.size() == 1) {
    // return the top node if size is 1
    return q.top();
  }
  // create a new freq_info shared ptr called node_one to hold first element to combine
  shared_ptr<freq_info> node_one = q.top();
  // remove first element from the queue
  q.pop();
  // create a new freq_info shared ptr called node_two to hold second element to combine
  shared_ptr<freq_info> node_two = q.top();
  // remove second element from the queue
  q.pop();
  // called CombineNodes with node_one and node_two to add them together
  shared_ptr<freq_info> comb_node = CombineNodes(node_one, node_two);

  // add new comb_node back into priority queue
  q.push(comb_node);
  // recursively call BuildTree until the codec is built
  BuildTree(q);
  // return the top of priority queue
  return q.top();
}


//********************************************************************************************************************
//                                        BUILDING ENCODING TABLE FUNCTION
//********************************************************************************************************************

// BuildEncodingTable creates a lookup table for the encoding
// process. it maps each symbol in the Huffman codec tree to a string
// composed of LEFT_CHAR and RIGHT_CHAR.
//
// root is the root node of a Huffman codec tree.
map<char, string> Huffman::BuildEncodingTable(shared_ptr<freq_info> root) {
  // TODO
  // cretae a new lookup table for encoding
  map<char, string> enc_tbl;
  // create a shared_ptr called cursor to point to root
  //shared_ptr<freq_info> cursor = root;
  string e_string = ""; // enc_tbl[root->symbol]
  RecursiveBuildEncodingTable(root, enc_tbl, e_string);
  return enc_tbl;
}

void Huffman::RecursiveBuildEncodingTable(shared_ptr<freq_info> cursor, map<char, string>& enc_tbl, string e_string) {
  // if cursor is a leaf, then add symbol to enc_tbl and return
  if(cursor->is_leaf) {
    enc_tbl[cursor->symbol] = e_string;
    return;
  }

  // add left character to the left string
  string left_str = e_string + LEFT_CHAR;
  // add right character to the right string
  string right_str = e_string + RIGHT_CHAR;

  // recursive call to the left child of cursor
  RecursiveBuildEncodingTable(cursor->left, enc_tbl, left_str);
  // recursive call to the right child of cursor 
  RecursiveBuildEncodingTable(cursor->right, enc_tbl, right_str);
}


//********************************************************************************************************************
//                                       ENCODE/DECODE FUNCTIONS
//********************************************************************************************************************

// Encode returns an encoded string using the provided encoding lookup
// table on the input string.
string Huffman::Encode(map<char, string> enc_table, string input) {
  // TODO
  // create new string called e_string
  string e_string = "";
  // iterate over the size of the input string
  for (int i = 0; i < input.length(); i++) {
    // find the letter at index i in input string
    // find that letter in the enc_table and store it in e_char
    string e_char = enc_table[input[i]];
    // concatenate together the encoded string plus the encoded char 
    // we just found in the encode table
    e_string = e_string + e_char;
  }
  // return the encoded string
  return e_string;
}

// Decode returns a decoded string using the provided Huffman codec
// tree and input string.
string Huffman::Decode(shared_ptr<freq_info> root, string input) {
  // TODO
  // initialize decoded string as an empty string
  string decoded_str = "";
  // use dec to traverse the codec tree
  shared_ptr<freq_info> dec = root;
  // iterate over each character in the input string
  for ( int i = 0; i < input.length(); i++ ) { 
    // if the current input string character matches the LEFT_CHAR then go left in the codec tree
    if (input[i] == LEFT_CHAR[0] ) {
      dec = dec->left;
    }
    // if the current input string character matches the RIGHT_CHAR then go right in the codec tree
    if ( input[i] == RIGHT_CHAR[0] ) {
      dec = dec->right;
    }
    // if dec reaches a node that is_leaf == true then enter if statement
    if (dec->is_leaf == true ) { 
      // store the character at that node into decoded_str
      decoded_str = decoded_str + dec->symbol; 
      // start over again with dec pointing to the root node of codec
      dec = root;
    }
  }
  // when you have finished the whole input string's length then return the decoded string
  return decoded_str;
}