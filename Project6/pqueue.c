/*
Andy Bredar
COEN 12
Project 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "pqueue.h"

#define INIT_LENGTH 10

//defined algorithms to determine the indices of the parent, left, and right nodes
#define parent(x) (((x)-1)/2)
#define left(x) (((x)*2)+1)
#define right(x) (((x)*2)+2)

//priority queue structure definition
typedef struct pqueue {

	int count; //number of elements in the array
	int length; //length of the array
	void **data; //allocated array of elements
	int (*compare)(); //comparison function
}PQ;

//return a pointer to a new priority queue using compare as its comparison function
// O(1) time complexity
PQ *createQueue(int (*compare)()){
	
	PQ *pq;

	assert (compare != NULL);

	pq = malloc(sizeof(PQ));
	assert(pq !=  NULL);

	//initialize the compare, length, and count values
	pq->compare = compare;
	pq->length = INIT_LENGTH; //array startin size is 10
	pq->count = 0; //initialize the umber of elements to 0

	//allocate memory
	pq->data = malloc(sizeof(void*) * pq->length);
	assert(pq->data != NULL);

	return pq;
}

//deallocate memory associated with the priority queue pointed to by pq
// O(n)
void destroyQueue(PQ *pq){

	assert(pq != NULL);
	
	/*
	I realized that this is not necessary...
	free memory associated with the priority queue
	for(int i = 0; i<pq->count ; i++){
		free(pq->data[i]); //free each double pointer
	}
	*/

	free(pq->data); //free the pointer to the array
	free(pq); //free the queue
}

//return the number of entries in the priority queue pointed to by pq
// O(1)
int numEntries(PQ *pq){

	assert(pq != NULL);
	return(pq->count);
}

//add entry to the priority queue pointed to by pq
// O(log n) time complexity
void addEntry(PQ *pq, void *entry){

	assert(pq != NULL);
	
	//if the priority queue is full, create a new queue with double the length and copy over the data
	if(pq->count >= pq->length){

		pq->length *= 2;
		pq->data = realloc(pq->data, sizeof(void*) * pq->length);
	}	
	
	//add entry to the queue
	pq->data[pq->count] = entry;

	//now if the entry value is greater than its parent, we must swap the values
	int idx = pq->count; //index of the new value
	while(pq->count != 0 && (*pq->compare)(pq->data[parent(idx)], pq->data[idx]) > 0){

		void* temp = pq->data[idx]; //copy the child node's data
		pq->data[idx] = pq->data[parent(idx)];
		pq->data[parent(idx)] = temp;
		idx = parent(idx); //the new index is the parent index
	}
	pq->count++; //number of elements is incremented
	return;
}

//remove and return the smallest entry from the priority queue pointed to by pq
// O(log n) time complexity
void *removeEntry(PQ *pq){

	assert(pq != NULL);
	void* returnValue = pq->data[0];
	pq->data[0] = pq->data[pq->count -1]; //last item in the heap is moved to the root
	
	int idx = 0;
	
	//need to move the root value to its proper location
	while(left(idx) <= pq->count -1){

		int smallestIdx = left(idx);
		if(right(idx) <= pq->count -1){
			//compare the left and right children of the root, the smallestIdx is the index of the smaller child
			if((*pq->compare)(pq->data[right(idx)], pq->data[smallestIdx]) < 0) {
				smallestIdx = right(idx);
			}
		}
		//if the data at idx is larger than the data at smallestIdx, there is no need to swap
		if((*pq->compare)(pq->data[idx], pq->data[smallestIdx]) < 0){
			break;
		}
		//swap if the child's value is larger than the parent's value
		else{
			void* temp = pq->data[idx];
			pq->data[idx] = pq->data[smallestIdx];
			pq->data[smallestIdx] = temp;
		}
		idx = smallestIdx;
	}
	pq->count--;
	//return the largest value which was originally at the root
	return returnValue;
}
