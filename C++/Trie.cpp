// Naureen Bharwani
// CS 2270 - Final Project
// Due: May 2th, 2021
// 
// Trie Project


#include "Trie.h"

Trie::Trie(){
    shared_ptr<TrieNode> top(NULL);
}


Trie::~Trie(){
    // nothing to add here, smart ptrs automatically deallocate memory for you
}



// ***************************************************************
//
//                      INITNODE FUNCTION
//
// ***************************************************************

// InitNode initializes a new TrieNode from the heap
// InitNode returns a pointer to itself
shared_ptr<TrieNode> Trie::InitNode() {
    // create a new TrieNode smart ptr called trie_node from heap memory
    shared_ptr<TrieNode> trie_node(new TrieNode);
    
    // Initialize NULL children
    for (int i = 0; i < LETTERS; i++) {
        trie_node->children[i] = NULL;
    } 

    // if letter is the last in the sequence then terminate is true
    // else letter is not the last in the sequence then terminate is false
    trie_node->terminate = false; 

    // return pointer to trie_node
    return trie_node;
}



// ***************************************************************
//
//                     INSERT NODE FUNCTION
//
// ***************************************************************


void Trie::InsertWord(shared_ptr<TrieNode> top, string AnimeName) {
    // create a new shared ptr called cursor to point to top
    shared_ptr<TrieNode> cursor = top;

    // iterate over each letter within AnimeName
    for (int i = 0; i < AnimeName.length(); i++) {
        // make sure each letter is between 0 to 25 
        // this corresponds to LETTERS Value
        int letter = AnimeName[i] - 97; // 97 is ASCII value for a
        // could also have used 65 to use capital letters

        // if letter is not a child node in the Trie
        // then create new node for the letter
        if (cursor->children[letter] == NULL) {
            // create a new shared ptr called new_node to store new letter
            shared_ptr<TrieNode> new_node = InitNode();
            // store new child letter into new_node
            cursor->children[letter] = new_node;
            // store letter from AnimeName in new_node
            new_node->char_let = AnimeName[i];
            // increment count for new node in trie
            cursor->count++;
        }
        // increment count for new node in trie
        cursor->count++;
        // update cursor to continue iterating through each letter in AnimeName
        cursor = cursor->children[letter];
    }
    // after iteration is complete, update last letter stored in cursor to be the terminate node
    cursor->terminate = true;
    return;
}



// ***************************************************************
//
//                     FIND NODE FUNCTION
//
// ***************************************************************


bool Trie::FindWord(shared_ptr<TrieNode> top, string AnimeName) {
    // create a new shared ptr called cursor to point to top
    shared_ptr<TrieNode> cursor = top;
    // check to see if trie is already empty before moving on
    if (cursor == NULL) {
        // return false if trie is empty, AnimeName won't be found
        return false;
    }
    // if trie not empty, then iterate over the length of AnimeName
    for (int i = 0; i < AnimeName.length(); i++) {
        // make sure each letter is between 0 to 25 
        // this corresponds to LETTERS Value
        int letter = AnimeName[i] - 97; // 97 is ASCII value for a
        // if our current letter is not a child of cursor (it is NULL)
        // then our AnimeName is not in the trie
        if (cursor->children[letter] == NULL) {
            return false;
        }

        // if child letter not NULL, then keep traversing the pathways
        cursor = cursor->children[letter];
    }

    // once iteration is done and all letters match
    // check to make sure cursor is the last in the list to ensure the Name ends ther
    if (cursor->terminate == true) {
        return true;
    }

    // otherwise the Name continues on and is not the AnimeName we are searching for
    // then return false
    return false;
}



// ***************************************************************
//
//                     REMOVE NODE FUNCTION
//
// ***************************************************************


void Trie::RemoveWord(shared_ptr<TrieNode> top, string AnimeName, int curr_len) {
    // create a shared ptr called cursor to point to top
    shared_ptr<TrieNode> cursor = top;
    // case 1: check to see if cursor is NULL then trie is empty
    // case 2: if Anime Name is not in trie
    if (false == FindWord(cursor, AnimeName)) {
        // exit RemoveWord
        return;
    }

    // create a new int variable to hold length of each word, initialize to 0
    curr_len = 0;
    RecursiveRemoveWord(cursor, AnimeName, curr_len);
}



// ***************************************************************
//
//                RECURSIVE REMOVE NODE HELPER FUNCTION
//
// ***************************************************************
void Trie::RecursiveRemoveWord(shared_ptr<TrieNode>& cursor, string& AnimeName, int& curr_len) {


    // case 3: AnimeName is found and is it's own string
    // no other words rely on it's inidivudal nodes
    

    // case 4: the AnimeName is a prefix for another word
    // i.e. boruto needs to be removed but border is present


    // case 5: the AnimeName contains a prefix in it of another word
    // i.e. bfish needs to be removed but boruto is present


    if (curr_len == AnimeName.length()) {
        int size = NumAnimeNames(cursor);        
        if (size > 1) {
            // if reached the end of AnimeName
            // terminate is set to false
            cursor->terminate = false;
        }

        // if size not greater than 1 then delete AnimeName
        // set cursor to NULL
        cursor = NULL;   
        // return once done deleting cursor
        return;
    }

    // decrement count of each node
    cursor->count--;
    // find last index of AnimeName via cursor's children
    int curr_let = AnimeName[curr_len] - 97; // 97 is ASCII num for a
    // traverse cursor's child with curr_let, AnimeName and curr_len of AnimeName
    // add one to curr_len to show another letter has been traversed in AnimeName
    curr_len += 1;
    RecursiveRemoveWord(cursor->children[curr_let], AnimeName, curr_len);
    if (cursor->count == 1) {
        cursor = NULL;
    }
}



// ***************************************************************
//
//                     SIZE FUNCTION
//
// ***************************************************************


int Trie::NumAnimeNames(shared_ptr<TrieNode> top) {
    // create a shared ptr called cursor to point to top
    shared_ptr<TrieNode> cursor = top;
    // initialize an int variable to hold the count of distinct AnimeNames in trie
    int num_names = 0;
    // case 1: if terminate is true, then we reached the end of the AnimeName
    if (cursor->terminate == true) {
        // incrememnt num_names
        num_names++;
    }
    // case 2: if terminate is not true then keep traversing nodes for each word
    for (int i = 0; i < LETTERS; i++) {
        // if cursor's chidlren is not NULL then follow that children until the word ends
        // recursively call NumAnimeNames with new cursor's child and update num_names
        //  if the child node is NULL then we that node/letter does not have any other children
        // exit the loop for that cursor's child and continue on until done
        if (cursor->children[i] != NULL) {
            num_names += NumAnimeNames(cursor->children[i]);
        }
    }
    // once all children have been searched return num_names
    return num_names;
}



// ***************************************************************
//
//                     COMPARE FUNCTION
//
// ***************************************************************

bool Trie::CompareAnimeNames(shared_ptr<TrieNode> first_trie, shared_ptr<TrieNode> second_trie, string AnimeName) {
    // create a new bool variable called found_name_first
    bool found_name_first = FindWord(first_trie, AnimeName);
    // create a new bool varaible called found_name_second
    bool found_name_second = FindWord(second_trie, AnimeName);
    if (found_name_first == true && found_name_second == true) {
        return true;
    }
    return false;
}


// Returns the top pointer
shared_ptr<TrieNode> Trie::GetTop(){
  return top;
}