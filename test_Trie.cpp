// Naureen Bharwani
// CS 2270 - Final Project
// Due: May 2th, 2021
// 
// Trie Project

// If you change anything in this file, your changes will be ignored 
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include <gtest/gtest.h>
#include "../code/Trie.h"

#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

class test_Trie : public ::testing::Test {
protected:
	// This function runs only once before any TEST_F function
	static void SetUpTestCase(){
	}

	// This function runs after all TEST_F functions have been executed
	static void TearDownTestCase(){
	}

	// this function runs before every TEST_F function
	void SetUp() override {}

	// this function runs after every TEST_F function
	void TearDown() override {
	}
};

/* // manually create Node for Trie
// return top of Trie
shared_ptr<TrieNode> HandBuildNode(string letter) {
  shared_ptr<TrieNode> top(new TrieNode);
  top->letter = letter;
  top->left = NULL;
  top->right = NULL;
  return top;
}

// return root of Trie
// manually create Trie
shared_ptr<TrieNode> HandBuildTree() {
	/* Hand build a trie with 3 anime names in it
	   erased, boruto and bfish in it. Looks
	   like this:
	  
	     				" "
	     				/ \
	       			   e   b
	   	  			  /   / \ 
	     			 r   o   f   
	    			/	/     \
	   			   a   r 	   i
	  			  /	  / 		\
	 			s	 u  		 s
			   /	/ 		      \
   			  e	   t			   h
  			 / 	  /					 
 			d 	 o					 
	*/	
	/* shared_ptr<TrieNode> top = HandBuildNode(" ");
	top->left = HandBuildNode("E");
	top->left->left = HandBuildNode("R");
	top->left->left->left = HandBuildNode("A");
	top->left->left->left->left = HandBuildNode("S");
	top->left->left->left->left->left = HandBuildNode("E");
	top->left->left->left->left->left->left = HandBuildNode("D");

	top->right = HandBuildNode("B");
	top->right->left = HandBuildNode("O");
	top->right->left->left = HandBuildNode("R");
	top->right->left->left->left = HandBuildNode("U");
	top->right->left->left->left->left = HandBuildNode("T");
	top->right->left->left->left->left->left = HandBuildNode("O");
	
	top->right->right = HandBuildNode("F");
	top->right->right->right = HandBuildNode("I");
	top->right->right->right->right = HandBuildNode("S");
	top->right->right->right->right->right = HandBuildNode("H");
	
	return top;
} */


TEST_F(test_Trie, TestInitNode){
	Trie mytrie;
	// expect top to be NULL
	ASSERT_FALSE(mytrie.GetTop());
	// create a new shared ptr called new_node via InitNode
	shared_ptr<TrieNode> new_node = mytrie.InitNode(); 
	// new_node terminate condition should be false      
    ASSERT_FALSE(new_node->terminate);    
	// check to confirm all letter children of new_node are NULL
    for (int i = 0; i < LETTERS; i++) {
        ASSERT_FALSE(new_node->children[i]);
    }
}


TEST_F(test_Trie, TestInsertWord){
  	Trie mytrie;
	// expect top to be NULL
	ASSERT_FALSE(mytrie.GetTop());
	// create a new shared ptr called first_insert via InitNode
	shared_ptr<TrieNode> first_insert = mytrie.InitNode(); 
	// first_insert terminate condition should be false      
    ASSERT_FALSE(first_insert->terminate);    
	// check to confirm all letter children of first_insert are NULL
    for (int i = 0; i < LETTERS; i++) {
        ASSERT_FALSE(first_insert->children[i]);
    }

	// now insert a new AnimeName into the Trie
	// pass the first_insert created and "erased" to function InsertWord
	mytrie.InsertWord(first_insert, "erased");
	// check to see if erased was inserted into tree
	bool found_erased = mytrie.FindWord(first_insert, "erased");
	ASSERT_TRUE(found_erased);
	// check to see if onepiece was inserted into tree
	bool found_onepiece = mytrie.FindWord(first_insert, "onepiece");
	ASSERT_FALSE(found_onepiece);
}


TEST_F(test_Trie, TestFindWord){
	Trie mytrie;
	// expect top to be NULL
	ASSERT_FALSE(mytrie.GetTop());
	// create a new shared ptr called second_insert via InitNode
	shared_ptr<TrieNode> second_insert = mytrie.InitNode(); 
	// now insert new AnimeName into the Trie
	mytrie.InsertWord(second_insert, "bananafish");
	// now insert another new AnimeName into the Trie
	mytrie.InsertWord(second_insert, "boruto");

	// test to see if FindWord function works
	// store answer in bool called found_bfish
	bool found_bfish = mytrie.FindWord(second_insert, "bananafish");
	// check to see if FindWord came back true, meaning bananafish
	// was inserted into tree
	ASSERT_TRUE(found_bfish);
}


TEST_F(test_Trie, TestRemoveWord){
	Trie mytrie;
	// create a shared ptr called remove_word via InitNode
	shared_ptr<TrieNode> remove_word = mytrie.InitNode();
	// check to see remove_word terminate condition is false
	ASSERT_FALSE(remove_word->terminate);
	// check to confirm all letter children of remove_word are NULL
	for (int i = 0; i < LETTERS; i++) {
		ASSERT_FALSE(remove_word->children[i]);
	}

	// insert AnimeNames into trie called remove_word
	mytrie.InsertWord(remove_word, "bananafish");
	mytrie.InsertWord(remove_word, "boruto");
	mytrie.InsertWord(remove_word, "erased");
	mytrie.InsertWord(remove_word, "onepiece");
	mytrie.InsertWord(remove_word, "onepunchman");
	mytrie.InsertWord(remove_word, "border");

	// use FindWord to assert true for all inserted AnimeNames
	bool found_bfish = mytrie.FindWord(remove_word, "bananafish");
	bool found_boruto = mytrie.FindWord(remove_word, "boruto");
	bool found_erased = mytrie.FindWord(remove_word, "erased");
	bool found_onepiece = mytrie.FindWord(remove_word, "onepiece");
	bool found_onepunchman = mytrie.FindWord(remove_word, "onepunchman");
	bool found_border = mytrie.FindWord(remove_word, "border");

	ASSERT_TRUE(found_bfish);
	ASSERT_TRUE(found_boruto);
	ASSERT_TRUE(found_erased);
	ASSERT_TRUE(found_onepiece);
	ASSERT_TRUE(found_onepunchman);
	ASSERT_TRUE(found_border);

	// check to see if RemoveWord function works
	// initialize curr_len to be 0 to pass to RemoveWord
	int curr_len = 0;
	mytrie.RemoveWord(remove_word, "border", curr_len);
	
	// check to see if border AnimeName was removed
	found_border = mytrie.FindWord(remove_word, "border");
	ASSERT_FALSE(found_border);
}


TEST_F(test_Trie, TestSize){
	Trie mytrie;
  	// expect top to be NULL
	ASSERT_FALSE(mytrie.GetTop());
	// create a new shared ptr called check_size via InitNode
	shared_ptr<TrieNode> check_size = mytrie.InitNode(); 
	// check_size terminate condition should be false      
    ASSERT_FALSE(check_size->terminate);    
	// check to confirm all letter children of check_size are NULL
    for (int i = 0; i < LETTERS; i++) {
        ASSERT_FALSE(check_size->children[i]);
    }

	// now insert new AnimeName into the Trie
	mytrie.InsertWord(check_size, "bananafish");
	// store answer in bool called found_bfish
	bool found_bfish = mytrie.FindWord(check_size, "bananafish");
	// check to see if found_bfish came back true, meaning bananafish
	// was inserted into tree
	ASSERT_TRUE(found_bfish);

	// bfish is inserted into check_size, size should be equal to 1
	int size = mytrie.NumAnimeNames(check_size);            
    ASSERT_EQ(1, size);    

	// next insert another AnimeName into the Trie called check_size
	mytrie.InsertWord(check_size, "boruto");
	// store answer in bool called found_boruto
	bool found_boruto = mytrie.FindWord(check_size, "boruto");
	// check to see if found_boruto came back true, meaning boruto
	// was inserted into trie
	ASSERT_TRUE(found_boruto);

	// next check to see if NumAnimeNames works
	// boruto & bananfish are inserted into check_size, size should be equal to 2
	size = mytrie.NumAnimeNames(check_size);            
    ASSERT_EQ(2, size);  

	// insert a couple more AnimeNames into trie called check_size
	mytrie.InsertWord(check_size, "erased");
	mytrie.InsertWord(check_size, "onepiece");
	mytrie.InsertWord(check_size, "onepunchman");
	mytrie.InsertWord(check_size, "border");

	// check to see if each new inserted AnimeName was found in check_size trie
	bool found_erased = mytrie.FindWord(check_size, "erased");
	bool found_onepiece = mytrie.FindWord(check_size, "onepiece");
	bool found_onepunchman = mytrie.FindWord(check_size, "onepunchman");
	bool found_border = mytrie.FindWord(check_size, "border");

	ASSERT_TRUE(found_erased);
	ASSERT_TRUE(found_onepiece);
	ASSERT_TRUE(found_onepunchman);
	ASSERT_TRUE(found_border);

	// finally check size of check_size trie
	// it should be equal to 6
	size = mytrie.NumAnimeNames(check_size);   
	ASSERT_EQ(6, size);
}


TEST_F(test_Trie, TestCompare){
	// create an initialize trie called first_trie
	Trie mytrie;
	// create a shared ptr called first_trie via InitNode
	shared_ptr<TrieNode> first_trie = mytrie.InitNode();
	// check to see first_trie terminate condition is false
	ASSERT_FALSE(first_trie->terminate);
	// check to confirm all letter children of first_trie are NULL
	for (int i = 0; i < LETTERS; i++) {
		ASSERT_FALSE(first_trie->children[i]);
	}

	// create a shared ptr called second_trie via InitNode
	shared_ptr<TrieNode> second_trie = mytrie.InitNode();
	// check to see second_trie terminate condition is false
	ASSERT_FALSE(second_trie->terminate);
	// check to confirm all letter children of second_trie are NULL
	for (int i = 0; i < LETTERS; i++) {
		ASSERT_FALSE(second_trie->children[i]);
	}	

	// insert AnimeNames into trie called first_trie
	mytrie.InsertWord(first_trie, "erased");
	mytrie.InsertWord(first_trie, "onepiece");

	// insert AnimeNames into trie called second_trie 
	mytrie.InsertWord(second_trie, "erased");
	mytrie.InsertWord(second_trie, "border");

	// check to see if each new inserted AnimeName was found in first_trie
	bool found_erased = mytrie.FindWord(first_trie, "erased");
	bool found_onepiece = mytrie.FindWord(first_trie, "onepiece");

	// check to see if each new inserted AnimeName was found in first_trie
	bool found_onepunchman = mytrie.FindWord(second_trie, "erased");
	bool found_border = mytrie.FindWord(second_trie, "border");
	
	// pass first_trie, second_trie and AnimeName to CompareAnimeNames
	bool found_name = mytrie.CompareAnimeNames(first_trie, second_trie, "erased");
	// check to see if erased is found in both first_trie and second_trie
	ASSERT_TRUE(found_name);

	// pass first_trie, second_trie and AnimeName to CompareAnimeNames
	found_name = mytrie.CompareAnimeNames(first_trie, second_trie, "bfish");
	// check to see if bfish was NOT found in either tries
	ASSERT_FALSE(found_name);

	// pass first_trie, second_trie and AnimeName to CompareAnimeNames
	found_name = mytrie.CompareAnimeNames(first_trie, second_trie, "border");
	// check to see if bfish was NOT found in either tries
	ASSERT_FALSE(found_name);

	// pass current length of AnimeName as 0 to RemoveWord
	int curr_len = 0;
	// RemoveWord will remove erased from first_trie
	mytrie.RemoveWord(first_trie, "erased", curr_len);
	// compare tries again to see if erased has been removed from first_trie
	found_name = mytrie.CompareAnimeNames(first_trie, second_trie, "erased");
	// erased should not appear in both tries now, found_name should be false
	ASSERT_FALSE(found_name);
}
