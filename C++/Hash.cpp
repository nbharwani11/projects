#include "Hash.h"

// implemented for you - don't change this one
unsigned int DJB2(std::string key){
  unsigned int hash = 5381;
  // Leaving the debugging stuff commented out, in case you want to
  // play. It will output the hash at each incremental step so you can
  // see how it takes shape.
  //
  //  cout << "Hashing string \"" << key << "\"" << endl;
  //  cout << bitset<32>(hash) << " " << hash << endl;
  for (size_t i=0; i < key.length(); i++) {
    char c = key[i]; 
    hash = ((hash << 5) + hash) + c;
    // cout << bitset<32>(hash) << " " << hash << endl;
  }
  // cout << "Hashed string \"" << key << "\" to " << hash << endl;
  //
  // for light reading on why djb2 might not be the best
  // production-grade hash function, go to
  // http://dmytry.blogspot.com/2009/11/horrible-hashes.html
  return hash;
}

// implemented for you - don't change this one
unsigned int ModuloBucketFunc(unsigned int hashcode, unsigned int cap){
  unsigned int b = hashcode % cap;
  return b;
}
//********************************************************************
//                  CONSTRUCTOR & DECONSTRUCTOR          
//********************************************************************

// constructor, initialize class variables and pointers here if need.
Hash::Hash(){
  // your code here
  // initialize bool deleted
}

//deconstructor,
Hash::~Hash(){
  // no need to add anything here
}
//********************************************************************
//                  INITNODE/TABLE FUNCTIONS          
//********************************************************************

shared_ptr<hash_table> Hash::InitTable(unsigned int cap){
  // create a new shared ptr hash_table called myhashtable using the new operator
  shared_ptr<hash_table> myhashtable(new hash_table);
  // initialize capacity, current number of addressable buckets in table. kept up-to-date.
  myhashtable->capacity = cap;
  // initialize size, number of actual (not deleted) entries. kept up-to-date.
  myhashtable->size = 0;
  // initialize occupied, number of non-null buckets. (so all hash_nodes, deleted or
  // otherwise). kept up-to-date.
  myhashtable->occupied = 0;

  // htable is used in hash_table struct bellow, you can define a table dynamically for ex:
  //
  //     // dynamically create a hash_table object first
  //     shared_ptr<hash_table> myhashtable(new hash_table);
  //     // then create the htable for the size you like (say mysize)
  //     myhashtable->table = shared_ptr<htable>(new htable(mysize));

  // initialize table, points to dynamic array of hash_node pointers (read comments for htable above). it should be
  // initialize to a dynamic array of pointers to hash_node structs. Those pointers should all initially be NULL.
  myhashtable->table = shared_ptr<htable>(new htable(cap));
  // iterate through myhastable up till capacity
  // marks every node in myhastable as NULL
  for (int i = 0; i < myhashtable->capacity; i++) {
    myhashtable->table->at(i) = NULL;
  }

  // point hash_func to DJB2 function
  myhashtable->hash_func = DJB2;
  // point bucket_func to ModuloBucketFunc
  myhashtable->bucket_func = ModuloBucketFunc;
  // return completed myhashtable
  return myhashtable;
}

shared_ptr<hash_node> Hash::InitNode(std::string key, unsigned int hashcode, std::string val){
  // create a new shared ptr hash_node called ret using the new operator
  shared_ptr<hash_node> ret(new hash_node);
  // your code here
  // initialize your new node ret's key value to be key
  ret->key = key;
  // initialize your new node ret's hashcode to be hashcode
  ret->hashcode = hashcode;
  // initialize your new node ret's value to be val
  ret->value = val;
  // make sure to initialize delted as false
  // deleted indicates if the node was marked as removed from the hash
  ret->deleted = false;
  return ret;
}

//********************************************************************
//                             SETKVP          
//********************************************************************

bool Hash::SetKVP(shared_ptr<hash_table> tbl, std::string key, std::string value){
  // your code here
  // return false for key/value pair if it was not inserted into your tbl
  // this happens when your capacity is full, when your size is equal to capacity
  if (tbl->size == tbl->capacity) {
    return false;
  }

  // initialize your hashcode from your hash_func using key
  int hashcode = tbl->hash_func(key);
  // initialize your bucket_num from your bucket_func using hascode and capacity
  int bucket_num = tbl->bucket_func(hashcode, tbl->capacity);

  // check to make sure you're not appending a value into a NULL placeholder
  if ((tbl != NULL) && (tbl->table != NULL) && (tbl->table->at(bucket_num) != NULL)) {
    // if your key is equal to your key at bucket num, then update the key's value
    if (key == tbl->table->at(bucket_num)->key){
      tbl->table->at(bucket_num)->value = value;
      // return true for updaitng key's value
      return true;
   }
  }

  // if no node exists at that bucket num then create a new node
  // create the shared ptr of type hash_node called new_node with the key, hashcode and value
  shared_ptr<hash_node> new_node = InitNode(key, hashcode, value);
  // store the new_node's info at the bucket num
  tbl->table->at(bucket_num) = new_node;
  // on exit, the size and occupancy of the hash table is increased if a
  // new key/value pair was added.
  tbl->size++;
  tbl->occupied++;
  // return true for key/value pair being inserted into your tbl
  return true;
}

//********************************************************************
//                          SET LOAD         
//********************************************************************

float Hash::Load(shared_ptr<hash_table> tbl){
  // your code here
  // Load returns a load factor describing how 'full' the table
  // is. because we are using linear probing, which leaves 'deleted'
  // hash nodes sitting around, it is reasonable to use occupied divided
  // by capacity. you can alternately use size divided by capacity,
  // which is a more canonical definition of hashtable load. either will
  // work with the unit tests.
  // make sure to convert to float since size, occupied and capacoty are all int types
  float load = (static_cast<float>(tbl->size) / tbl->capacity);
  return load;
}

//********************************************************************
//                         GET VAL          
//********************************************************************

std::string Hash::GetVal(shared_ptr<hash_table> tbl, std::string key){
  // your code here
  // GetVal returns the value associated with the provided key in the
  // table, or the empty string "" if no such mapping exists (or if
  // there is a mapping but it is deleted).

  // iterate through the hash_nodes in the tbl
  for (int i = 0; i < tbl->capacity; i++) {
    // as long as you do not reach a NULL placeorder keep going
    if ((tbl != NULL) && (tbl->table != NULL) && (tbl->table->at(i) != NULL)) {
      // if the key is equal to the key at that position then return the keys value
      if (key == tbl->table->at(i)->key) {
        return tbl->table->at(i)->value;
      }
    }
  }
  // if no matching key was found, or the whole hash tbl has been traversed
  // return an empty string
  return "";
}

//********************************************************************
//                    CONTAINS FUNCTION        
//********************************************************************

bool Hash::Contains(shared_ptr<hash_table> tbl, std::string key){
  // your code here
  // Contains returns true if the table has a non-deleted node whose
  // hashcode matches the given key's hashcode.

  // iterate through the hash tbl
  for (int i = 0; i < tbl->capacity; i++) {
    // keep going unless you reach a NULL placeorder
    if ((tbl != NULL) && (tbl->table != NULL) && (tbl->table->at(i) != NULL)) {
      // if the key matches the key at that hash_node position then return true for a match
      if (key == tbl->table->at(i)->key) {
        return true;
      }
    }
  }
  // if no matching key was found and the hash tbl has been fully traversed
  // return false
  return false;
}

//********************************************************************
//                        REMOVE FUNCTION        
//********************************************************************

bool Hash::Remove(shared_ptr<hash_table> tbl, std::string key){
  // your code here
  // Remove marks as 'deleted' any existing non-deleted hash_node with
  // the same hashcode as the provided key's hashcode. if it finds such
  // a hash node, it decrements the table's size (but not occupancy) and
  // returns true. it returns false otherwise.

  // iterate through the hash tbl
  for (int i = 0; i < tbl->capacity; i++) {
    // keep going until you find a NULL placeorder
    if ((tbl != NULL) && (tbl->table != NULL) && (tbl->table->at(i) != NULL)) {
      // if the key is equal to the key at that position then
      // decrement size (NOT OCCUPIED)
      // update that node's deleted varaible to true
      // return the true
      if (key == tbl->table->at(i)->key) {
        tbl->size--;
        return tbl->table->at(i)->deleted = true;
      }
    }
  }
  // if no key matched then return false, key was not in hash tbl
  return false;
}

//********************************************************************
//                      RESIZE FUNCTION        
//********************************************************************

void Hash::Resize(shared_ptr<hash_table> tbl, unsigned int new_capacity){
  // your code here
}

// implemented for you - feel free to change this one if you like
void Hash::PrintTable(shared_ptr<hash_table> tbl){
  cout << "Hashtable:" << endl;
  cout << "  capacity: " << tbl->capacity << endl;
  cout << "  size:     " << tbl->size << endl;
  cout << "  occupied: " << tbl->occupied << endl;
  cout << "  load:     " << Load(tbl) << endl;
  if (tbl->capacity < 130) {
    for (unsigned int i=0; i < tbl->capacity; i++) {
      cout << "[" << i << "]    ";
      if (!tbl->table->at(i)) {
        cout << "<empty>" << endl;
      } else if (tbl->table->at(i)->deleted) {
        cout << "<deleted>" << endl;
      } else {
        cout << "\"" << tbl->table->at(i)->key << "\" = \"" << tbl->table->at(i)->value << "\"" << endl;
      }
    }
  } else {
    cout << "    <hashtable too big to print out>" << endl;
  }
}

