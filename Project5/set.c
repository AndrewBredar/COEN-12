/*Andy Bredar
COEN 12
Lab 5 Part 2
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "set.h"

#define AVG_CHAIN_LENGTH 20

//Node data structure declaration
typedef struct node{
	void *data;
	struct node *next;
	struct node *prev;
}NODE;

//List data structure declaration
typedef struct list{
	int count;
	int (*compare)();
	NODE *head;
	NODE *tail;
}LIST;

//Set data structure declaration;
typedef struct set{
	int count; //number of elements in the array
	int length; //length of the array
	int (*compare)(); //compare function
	unsigned (*hash)(); //hash function
	LIST **lists; //array of lists
}SET;


//creates a Set | O(1) time complexity
SET *createSet(int maxElts, int (*compare)(), unsigned(*hash)()){
	
	//create a set pointer (sp) and initialize its values
	int i;
	SET *sp;
	
	assert(compare != NULL && hash != NULL);
	sp = malloc(sizeof(SET));

	assert(sp != NULL);

	sp->length = maxElts / AVG_CHAIN_LENGTH;
	sp->compare = compare;
	sp->hash = hash;
	sp->count = 0;
	
	//create the array of lists
	sp->lists = malloc(sizeof(LIST *) * sp->length);
	assert(sp->lists != NULL);

	// O(1) because sp->length is constant	
	for(i = 0; i < sp->length; i++){
		sp->lists[i] = createList(compare);
	}

	return sp;
}

//destroy all lists within the set and free all allocated memory associated with the set
// O(n) time complexity because we have to call destroyList (sp->length is a constant value)
void destroySet(SET *sp){
	
	assert(sp != NULL);
	int i;
	for(i = 0; i < sp->length; i++){
		destroyList(sp->lists[i]);
	}
	free(sp->lists);
	free(sp);
	return;
}

//returns the number of elements in the set | O(1) time complexity
int numElements(SET *sp) {
	assert(sp != NULL);
	return sp->count;
}

//add an element to the set in one of the lists by calling findItem and addFirst 
//O(1) time complexity
void addElement(SET *sp, void *elt) {

	assert(sp != NULL);
	assert(elt != NULL);
	
	//find the index of the corresponding list
	int index = ((*sp->hash)(elt) % sp->length);

	//O(n) to search for a given item
	void *found = findItem(sp->lists[index], elt);
	
	//if the item is not foun then we can add it to the corresponding list
	if(found == NULL){
		//O(1) time complexity to add item to the list
		addLast(sp->lists[index], elt);
		sp->count++;
		return;
	}
	return;
}

// remove the given element from the set
// O(n) time complexity because we must search for the element
void removeElement(SET *sp, void *elt) {
	
	//check to make sure the list is not empty or NULL
	assert(sp != NULL);
	assert(sp->count > 0);
	assert(sp->compare != NULL);
	assert(elt != NULL);

	int index = (*sp->hash)(elt) % sp->length;
	
	void *found = NULL;
	found = findItem(sp->lists[index], elt);
	
	//if the item is found, call the remove function
	if(found != NULL){
		removeItem(sp->lists[index], elt);
		sp->count--;
		return;
	}
	return;
}


//find a given element in the set
// O(n) because it calls findItem
void *findElement(SET *sp, void *elt){

	assert(sp != NULL);
	assert(sp->compare != NULL);
	int index = (*sp->hash)(elt) % sp->length;

	//if it is found then return its value
	NODE *found = findItem(sp->lists[index], elt);
	if (found != NULL){
		return found->data;
	}
	return NULL;
}

//allocate and return an array with all the elements in the set
// O(n) time complexity
void *getElements(SET *sp){

	assert(sp != NULL);
	assert(sp->count > 0);
	assert(sp->compare != NULL);

	//create an empty array with a size corresponding to the sp->count
	void **elts;
	elts = malloc(sizeof(void*) * (sp->count));
	assert(elts != NULL);

	int i;
	int j = 0;
	NODE *pCur;
	
	//iterate through each list and copy its elements to the elts array
	for(i=0; i < sp->length; i++){
		pCur = sp->lists[i]->head->next;
		while(pCur != sp->lists[i]->head){
			elts[j] = pCur->data;
			pCur = pCur->next;
			j++;
		}
	}
	return elts;

}
