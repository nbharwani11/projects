#include "PriorityQueue.h"

PriorityQueue::PriorityQueue(){
    // YOUR CODE HERE
    // nothing to construct here
}

PriorityQueue::~PriorityQueue(){
    // You don't need to add anything here 
    // since shared ptrs automatically deallocate memory for you
}

// Implement this after defining struct in .h file
shared_ptr<pq> PriorityQueue::InitPriorityQueue(){
    // YOUR CODE HERE
    // initialize a new node called queue
    // use new to dynamically allocate new memory using the heap
    shared_ptr<pq> queue(new pq);
    // return your new node
    return queue;
}

// Implement this second
void PriorityQueue::Insert(shared_ptr<pq> queue, string text, float priority){
    // YOUR CODE HERE
    // case 1: check to see if your queue's priorityVec or textVec are empty
    // if empty then just insert text and string into the first position in each Vec
    if (queue->textVec.size() == 0) {
        // if textVec is empty then just append the text to the first position in Vec
        // if priorityVec is empty then append the priority to the first position in Vec
        // return to exit
        queue->textVec.push_back(text);
        queue->priorityVec.push_back(priority);
        return;
    }
    // if Vecs are not empty, then iterate over vecs to insert new data
    // create an int variable i for iterate through vec
    int i = 0;

    // case 2: queue is not empty
    // use while loop iterating over the priorities to find the correct position to insert new data
    while (i < queue->textVec.size() && queue->priorityVec[i] < priority) {
        i++;
    }

    // case 3: if priority goes to the end of the list then just push_back values and return
    if (i == queue->textVec.size() && i == queue->priorityVec.size()) {
        queue->textVec.push_back(text);
        queue->priorityVec.push_back(priority);
        return;
    }

    // if priority goes at the beginning of the list or the middle
    // move all elements down in textVec and priorityVec
    // push_back the last element in each vec to replicate it, so last two elements are the same in each vec
    queue->textVec.push_back(queue->textVec[queue->textVec.size() - 1]);
    queue->priorityVec.push_back(queue->priorityVec[queue->priorityVec.size() - 1]);

       
    // use for loop to iterate over textVec and priorityVec to shift all data points down by one
    // do this by starting at the end of the list minus 2, so you are at the second to last position in vec
    // continue backwards until you've reached the position to add the new data into the vectors


    for (int position = queue->textVec.size() - 2; position > i; position--) {
         queue->textVec[position] = queue->textVec[position - 1];
         queue->priorityVec[position] = queue->priorityVec[position - 1];
    } 

    // now that you have finished updating the rest of the vectors, just insert the new data into the correct position found at index i
    // then return to exit
    queue->textVec[i] = text;
    queue->priorityVec[i] = priority;
    return;
}

// Implement this third
string PriorityQueue::Remove(shared_ptr<pq> queue){
    // YOUR CODE HERE
    // find the highest priority element in the vec which will be the last element in the vec
    // store the last elements string from textVec into removed_elemnt, this is what you will be returning
    string removed_element = queue->textVec[queue->textVec.size() - 1];
    // use pop_back function for vectors to remove the last element in both textVec and priorityVec
    queue->textVec.pop_back();
    queue->priorityVec.pop_back();
    // once that element is removed, return the string of the last element removed from textVec
    // this value was stored in the string variable removed_element
    return removed_element;   // replace this line with your own code
}

// Implement this last
string PriorityQueue::Peek(shared_ptr<pq> queue){
    // if the textVec and priorityVec are empty, so size is 0 then just return an empty string
    // the empty string represents the highest priority in the list
    if (queue->textVec.size() == 0) {
        return "";
    }
    // if not empty return the highest priority item in the textVec which is the last item with the highest number
    // find this by taking the size of the vector and minus one to account for indexing then just return that string
    return queue->textVec[queue->textVec.size() - 1];    // replace this line with your own code
}
