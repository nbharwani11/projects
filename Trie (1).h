// Naureen Bharwani
// CS 2270 - Final Project
// Due: May 2th, 2021
// 
// Trie Project

#ifndef TRIE_H__
#define TRIE_H__

#include <string>
#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <iostream>

// Letters in alphabet
const int LETTERS = 26;

using namespace std;

struct TrieNode { 
  // 26 children based on constant LETTERS
  shared_ptr<TrieNode> children[LETTERS];
  // string AnimeName to pass to functions
  string AnimeName;
  string char_let;
  // terminate condition checks to see if letter is last in sequence
  bool terminate; 
  int count;
};

class Trie {
public:
  // constructor
  Trie();

  // deconstructor
  ~Trie();

  // InitNode initializes a new TrieNode from the heap
  // InitNode returns a pointer to itself
  shared_ptr<TrieNode> InitNode();

  // InsertWord uses pointer to top of trie and string AnimeName 
  // It should insert the AnimeName into the trie
  // It should not return anything
  void InsertWord(shared_ptr<TrieNode> top, string AnimeName);

  // FindWord will check to see if AnimeName is within the trie
  // It should return a bool value
  bool FindWord(shared_ptr<TrieNode> top, string AnimeName);

  // RemoveWord will remove AnimeName from the trie
  // It should not return anything
  void RemoveWord(shared_ptr<TrieNode> top, string AnimeName, int curr_len);

  // RecursiveRemoveWord will remove AnimeName from trie
  // It is the helper function to RemoveWord and does the same thing
  // It should not return anything
  void RecursiveRemoveWord(shared_ptr<TrieNode>& cursor, string& AnimeName, int& curr_len);

  // NumAnimeNames will count the number of AnimeNames in the trie
  // It should return the number of individual AnimeNames
  int NumAnimeNames(shared_ptr<TrieNode> top);

  // CompareAnimeNames takes in two different tries and an AnimeName
  // checks to see if an AnimeName appears in both, one or none of the tries
  bool CompareAnimeNames(shared_ptr<TrieNode> first_trie, shared_ptr<TrieNode> second_trie, string AnimeName);

  // Returns the top pointer
  shared_ptr<TrieNode> GetTop();
    
private:
  shared_ptr<TrieNode> top;
};

#endif // TRIE_H__