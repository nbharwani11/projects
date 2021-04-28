#include "Vector10.h"

Vector10::Vector10(){
    // YOUR CODE HERE
   for (int i = 0; i < 10; i++) {
       arr[i] = 0;
   }
   last_spot = 0;
}

Vector10::~Vector10(){
    // You don't need to add anything here
}

// Implement this function first, since all tests use it.
int Vector10::ValueAt(int index){
    // YOUR CODE HERE
    for (int i = 0; i < 10; i++) {
        if (i == index) {
            return arr[i];
        }
    }
    return -1;
}

// Implement this function second, since most tests use it.
bool Vector10::PushBack(int value){
    // YOUR CODE HERE
    for (int i = 0; i < 10; i++) {
        if (i == last_spot) {
            arr[last_spot] = value;
            last_spot++;
            return true;
        }
    }
    return false;
}

int Vector10::CountEmpty(){
    // YOUR CODE HERE
    int remaining_spots = 0;
    remaining_spots = (10 - last_spot);
    return remaining_spots;
}

bool Vector10::Search(int value){
    // YOUR CODE HERE
    for (int i = 0; i < 10; i++) {
        if (arr[i] == value) {
            return true;
        }
    }
    return false;
}


bool Vector10::Remove(int index){
    // YOUR CODE HERE
    if (index > 0 && index < 10) {
        for (int i = index; i < 10; i++) {
            arr[i] = arr[i + 1];
        }
        return true;
    }
    return false;
}
