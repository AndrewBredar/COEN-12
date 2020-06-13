#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>


#define FILLED 'a'
#define EMPTY 'b'
#define DELETED 'c'


//declare a data structure called SET with the following data types within
typedef struct set {
	char **words; //array of allocated elements
	char *flags; //state of each slot in array
	int length; //length of the allocated array
	int count; //number of elements in the array
	int (*compare)(); //comparison function
	unsigned (*hash)(); //hash funciton
}SET;

//creates a set | O(1) time complexity (although there is a 'for' loop, maxElts is a constant value and therefore
//initializing the flag array takes constant time (however, if maxElts is not constant then the time complexity is O(n)
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)()){
	
	//create a set pointer (sp) and initialize its values
	SET *sp = malloc(sizeof(SET));
	assert(sp != NULL);
	
	//allocate memory for an array of length maxElts to store char* values
	sp->words = malloc(sizeof(char*)*maxElts);
	assert(sp->words != NULL);

	sp->flags = malloc(sizeof(char)*maxElts);
	assert(sp->flags != NULL);
	
	//the maximum lenth should be the maximum elements that the user inputs
	sp->length = maxElts;
	
	//initialize the count of unique words to 0
	sp->count = 0;
	
	//initialize flag array
	for(int i=0; i < maxElts; i++)
		sp->flags[i] = EMPTY;
	
	sp->compare = compare;
	sp->hash = hash;

	//return the a pointer to the set
	return sp;
}

//receives a SET pointer, a word to search for, and a boolean to indicate if the word is found
//returns an index of the found word if found or index where the new word should go
//uses the hash function to find the given word in the array
// O(1) time complexity in the ideal case | O(n) in the worst case
static int search(SET *sp, void* searchWord, bool *found){
	
	//make sure the set pointer is not NULL (this line appears in most functions but I won't comment each time)
	assert (sp != NULL);
	
	//initialize the "found" boolean to false
	*found = false;
	
	//utilize hash function with linear probing to search for the given word
	int i = 0; 
	int hash;
	int openSlot = -1;

	for(i = 0; i < sp->length ; i++){
		
		//initialize index
		int idx = (((*sp->hash)(searchWord) + i) % sp->length);

		if(sp->flags[idx] == EMPTY){
			//if openSlot is not -1 then we have already found a deleted index where a new word can be inserted
			if(openSlot != -1)
				return openSlot;
			//otherwise we can return the index of this EMPTY slot where a new word can go
			return idx;
		}
		else if(sp->flags[idx] == DELETED){
			//if this is the first deleted spot we have seen than we can mark this as the openSlot for a new word
			if(openSlot == -1)
				openSlot = idx;
		}
		if((*sp->compare)(sp->words[idx], searchWord) == 0){
			//if we find the word we are looking for then we can return this index and mark 'found' as true
			*found = true;
			return idx;
		}
	}
	//return the first available index if the given word is not found
	return openSlot;
	
}


//frees each element within the SET that is pointed to, including each word that is pointed to within the array of char*
// O(n) time complexity becasue of the for loop
void destroySet(SET *sp){
	
	assert(sp != NULL);
	
	//free the array storing all of the flags
	free(sp->flags);
	
	//free the pointer to the array of char*
	free(sp->words);
	
	//free the memory where the SET structure is located
	free(sp);
	return;
}

//returns the number of unique words in the SET 
//O(1) time complexity
int numElements(SET *sp){
	
	assert(sp != NULL);
	
	//the SET structure has a value called count which keeps track of the number of unique words, so just return that value
	return(sp->count);
}

//adds an element to the list by first checking if this element is within the list and then adding it to the list if it is not found
//O(1) time complexity in the ideal case and O(n) time complexity in the worst case because it calls the search function
void addElement(SET *sp, void *elt){
	
	assert(sp != NULL);

	//we cannot add another word to the set if the number of unique words if greater than or equal to the max number allowed
	assert(sp->count  <  sp->length);
	
	//search for the word and make sure that it does not already exist in the set
	bool found;
	int idx = search(sp, elt, &found);
	
	//if the word we are trying to add is not already in the set then we can add it to the set | O(1) time complexity
	if(found == false && idx != -1){
		
		//allocate memory and copy the word to this location	
		sp->words[idx] = elt;
		assert(sp->words[idx] != NULL);
		
		//change the flag array and increment the count
		sp->flags[idx] = FILLED;
		sp->count++;
	}
	return;
}

//seaches through the set for a particular word and removes it if it is found
//O(1) time in the best case and O(n) time in the worst case
void removeElement(SET *sp, void *elt){	

	assert(sp != NULL);
	
	bool found;
	int idx = search(sp, elt, &found);
	
	//if the word is not found just return
	if(found){
	//if the word is found then free the memory at the word's location and decrement the found and modify the flag array
	//not supposed to free up space free(sp->words[idx]);
		sp->flags[idx] = DELETED;
		sp->count--;
	}
}


//looks for a given word in the set
//O(1) in the best case and O(n) in the worst case time complexity because it calls the search function
char *findElement(SET *sp, void *elt){

	assert(sp != NULL);
	
	bool found;
	int idx = search(sp, elt, &found);
	
	if(idx != -1 && found == true){
		return(sp->words[idx]);
	}
	return NULL;
}

//creates an array of elements in the set pointed by sp
//O(n) runtime because of the for loop to traverse the array
char **getElements(SET *sp){
	
	assert(sp != NULL);
	int i, j;

	//create an array of char* with a length according to the number of words(count)
	char** elementArray = malloc(sizeof(char*) * (sp->count));
	assert(elementArray != NULL);
	
	//copy the list of words from the SET structure to the new elementArray using a for loop
	//memcpy wouldnt work well here because the array may have empty slots
	for(i=0, j=0; i < sp->length ; i++){
		if(sp->flags[i] == FILLED){
			elementArray[j] = sp->words[i];
			j++;
		}
	}
	return elementArray;
}
