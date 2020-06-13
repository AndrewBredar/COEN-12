#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

//declare a data structure called SET with the following data types within
typedef struct set {
	char **words;
	int length;
	int count;
}SET;

//creates a set | O(1) time complexity
SET *createSet(int maxElts){
	
	//create a set pointer (sp) and initialize its values
	SET *sp = malloc(sizeof(SET));

	//allocate memory for an array of length maxElts to store char* values
	sp->words = malloc(sizeof(char*)*maxElts);

	//the maximum lenth should be the maximum elements that the user inputs
	sp->length = maxElts;

	//initialize the count of unique words to 0
	sp->count = 0;

	//return the a pointer to the set
	return sp;
}

//receives a SET pointer, a word to search for, and a boolean to indicate if the word is found
//uses linear search to find the given word in the array | O(n) time complexity
static int search(SET *sp, char* searchWord, bool *found){
	
	//make sure the set pointer is not NULL (this line appears in most functions but I won't comment each time)
	assert (sp != NULL);

	//initialize the "found" boolean to false
	*found = false;

	int i = 0;
	int count = sp->count;

	//iterates through the list of words within the SET structure, sets found to true and returns the index of the word (if it is found)
	for(i = 0; i < count; i++){
		if(strcmp(sp->words[i], searchWord) == 0){
			*found = true;
			return i;
		}
	}
	//if the word is not found, return -1
	return -1;
}

//frees each element within the SET that is pointed to, including each word that is pointed to within the char* array
// O(n) time complexity
void destroySet(SET *sp){
	
	assert(sp != NULL);
	
	//iterate throught the list of words within the set freeing the memory where each word is located
	for(int i = 0; i < sp->count; i++){
		free(sp->words[i]);
	}
	
	//free the pointer to the array of char*
	free (sp->words);
	
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
//O(n) time complexity because it calls the seach function
void addElement(SET *sp, char *elt){
	
	assert(sp != NULL);

	//we cannot add another word to the set if the number of unique words if greater than or equal to the max number allowed
	assert(sp->count < sp->length);
	
	//search for the word and make sure that it does not already exist in the set
	bool found;
	search(sp, elt, &found);
	
	//if the word we are trying to add is not already in the set then we can add it to the set
	if(found == false){

		sp->words[sp->count] = malloc(sizeof(char) * (strlen(elt)+1));
		strcpy(sp->words[sp->count], elt);
		sp->count++;

	}
	return;
}

//seaches through the set for a particular word and removes it if it is found
//O(n) time complexity because it calls the search function
void removeElement(SET *sp, char *elt){	

	assert(sp != NULL);
	
	//search for the given element
	bool found;
	int idx = search(sp, elt, &found);
	
	//if it is not found then there is nothing to remove, so just return
	if(found == false)
		return;
	
	//if it is found, then free the memory associated with that word, then put the pointer in the last
	//index in the array in the index where the pointer to the given word was located
	else {
		free(sp->words[idx]);
		sp->words[idx] = sp->words[sp->count-1];
		sp->count--;
	}
}

//looks for a given word in the set
//O(n) time complexity because it calls the search function
char *findElement(SET *sp, char *elt){

	assert(sp != NULL);
	
	//search for the given word, if it is found then return the pointer to that word
	bool found;
	int idx = search(sp, elt, &found);
	if(found == true){
		return(sp->words[idx]);
	}
	return NULL;
}

//creates an array of elements in the set pointed by sp
//O(1) runtime
char **getElements(SET *sp){
	
	assert(sp != NULL);
	
	//create an array of char* with a length according to the number of words(count)
	char** elementArray = malloc(sizeof(char*) * (sp->count));
	assert(elementArray != NULL);
	
	//copy the list of words from the SET structure to the new elementArray using memcpy
	//(Akash said to use this function, and I learned how to use it online)
	return	memcpy(elementArray, sp->words, (sizeof(char*) * (sp->count)));
}
