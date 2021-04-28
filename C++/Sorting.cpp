#include "Sorting.h"

/*
  Please refer to sorting.h for documentation on each function.

   + ------ N O T E: -- Big scary warning! -----------------------------\
   |                                                                    |
   |    We clearly can't use unit testing to ensure that you're         |
   |    actually implementing the correct algorithm. I suppose we could |
   |    write some complicated static analysis checker. But it is       |
   |    easier to just look at it with our eyeballs.                    |
   |                                                                    |
   |    After the assignment is due, we will look at the most recent    |
   |    version of your implementation that got the highest score and   |
   |    make sure you're not doing something sneaky like using a        |
   |    standard library call to sort a vector.                         |
   |                                                                    |
   |    Anyone caught doing this (even though it is kinda funny) will   |
   |    be given 0 points on the assignment.                            |
   |                                                                    |
   `--------------------------------------------------------------------/ 
*/

// constructor, initialize class variables and pointers here if you need.
Sorting::Sorting(){
  // Your code here
}

//deconstructor,
Sorting::~Sorting(){
}

void Sorting::quicksort(vector<int>& data, int low_idx, int high_idx){

}

int Sorting::quicksort_partition(vector<int>& data, int low_idx, int high_idx){

}

void Sorting::bubblesort(vector<int>& data){
  // iterate through the list comparing each value with all other values
  // use two for loops to do this
  for (int i = 0; i < data.size() - 1; i++) {
    for (int j = 0; j < data.size() - 1; j++) {
      // if item at position j is greater than item at position j + 1, swap them
      if (data[j] > data[j + 1]) {
        // create temo variable to store data[j]'s value
        int temp = data[j];
        // data of j is now equal to data[j + 1]
        data[j] = data[j + 1];
        // now store data[j + 1] to be data[j]'s previous value which is stored in temp
        data[j + 1] = temp;
      }
    }
  }
}

void Sorting::mergesort(vector<int>& data){

}

void Sorting::merge(vector<int>& left, vector<int>& right, vector<int>& result){

}

void Sorting::mystery_sort(vector<int>& data){
  
}