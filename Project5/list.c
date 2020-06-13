/*Andy Bredar
COEN 12
Lab 5 Part 1
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

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

//creates a list | O(1) time complexity
LIST *createList(int (*compare)()){
	
	//create a list pointer (lp) and initialize its values
	LIST *lp;
	lp = malloc(sizeof(LIST));

	assert(lp != NULL);

	lp->count = 0;
	lp->compare = compare;

	//create dummy node
	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);
	
	//initialize pointers
	lp->head->next = lp->head;
	lp->head->prev = lp->head;
	return lp;
}

//destroys the list by freeing the memory associated with each index
//O(n) time complexity
void destroyList(LIST *lp) {
	
	assert(lp != NULL);
	NODE *pPrev;
	NODE *del = lp->head->prev;
	//iterate through the list freeing the nodes
	while(del != lp->head){
		pPrev = del->prev;
		free(del);	
		del = pPrev;
	}
	free(del);
	free(lp);
}

//returns the number of items in the list | O(1) time complexity
int numItems(LIST *lp) {
	assert(lp != NULL);
	return lp->count;
}

//add item to the front of the list | O(1) time complexity
void addFirst(LIST *lp, void *item) {

	assert(lp != NULL);
	assert(item != NULL);

	NODE *newNode = malloc(sizeof(NODE));
	assert(newNode != NULL);
	newNode->data = item;

	//insert node into the list
	newNode->next = lp->head->next;
	newNode->prev = lp->head;
	
	//now connect the list to the newNode
	lp->head->next->prev = newNode;
	lp->head->next = newNode;

	//increment the count
	lp->count++;

	return;
}

//add item to the end of a list | O(1) time complexity
void addLast(LIST *lp, void *item) {
	
	assert (lp != NULL);
	assert (item != NULL);
	NODE *newNode = malloc(sizeof(NODE));
	assert(newNode != NULL);
	newNode->data = item;

	//insert new node into the list
	newNode->next = lp->head;
	newNode->prev = lp->head->prev;

	//connect the list to the newNode
	lp->head->prev->next = newNode;
	lp->head->prev = newNode;

	//increase count
	lp->count++;
	return;
}

//remove the first item in the list and return it
// O(1) time complexity
void *removeFirst(LIST *lp) {
	
	//check to make sure the list is not empty or NULL
	assert(lp != NULL);
	assert(lp->count > 0);

	void *item;
	item = malloc(sizeof(void));
	assert(item != NULL);

	NODE *del;
	del = lp->head->next;
	item = del->data;

	lp->head->next = del->next;
	lp->head->next->prev = lp->head;

	free(del);
	lp->count--;
	
	return item;
}

//remove the last item in the list and return it
//O(1) runtime
void *removeLast(LIST *lp) {
	
	//check to make sure the list is not empty or NULL
	assert (lp != NULL);
	assert (lp->count > 0);
	
	NODE *del = lp->head->prev;
	void *item;
	item = malloc(sizeof(void));
	assert(item != NULL);
	item = del->data;
 
	lp->head->prev = del->prev;
	lp->head->prev->next = lp->head;
	free(del);
	lp->count--;
	return item;
}

//return the first item in the list without deleting it
// O(1) runtime
void *getFirst(LIST *lp) {

	assert(lp != NULL);
	assert(lp->count > 0);

	return lp->head->next->data;
}

//return the first item in the list without deleting it
// O(1) runtime
void *getLast(LIST *lp) {
	
	assert(lp != NULL);
	assert(lp->count > 0);

	return lp->head->prev->data;
}

//if item is present in the list pointed ny lp then remove it
//O(n) runtime because you must traverse the list
void removeItem(LIST *lp, void *item) {

	assert(lp != NULL);
	assert(lp->count > 0);
	assert(lp->compare != NULL);

	NODE *pCur = lp->head->next;
	while(pCur != lp->head){
		if((*lp->compare)(pCur->data, item) == 0){
			//reassign pointers if the specified item is found
			pCur->next->prev = pCur->prev;
			pCur->prev->next = pCur->next;
			
			free(pCur);
		}
		//move the pCur pointer down the list
		pCur = pCur->next;
	}
}

//if item is present in the list pointed by lp then return the matching item
// O(n) time complexity because we need to traverse through the linked list
void *findItem(LIST *lp, void *item) {
	
	assert(lp != NULL);
	assert(lp->compare != NULL);
	assert(item != NULL);

	NODE *temp = lp->head->next;
	
	void *pItem = NULL;

	while(temp != lp->head){
		if((*lp->compare)(temp->data, item) == 0){
			pItem = temp->data;
			return pItem;
		}
		temp = temp->next;
	}
	return NULL;
}

//allocate and return an array of items in the list pointed by lp
// O(n) time complexity because you must traverse the list
void *getItems(LIST *lp) {
	
	assert(lp!=NULL);
	assert(lp->count > 0);

	void **items;
	items = malloc(sizeof(void*) * (lp->count));
	assert(items != NULL);

	int i = 0;

	NODE *pCur = lp->head->next;
	
	while(pCur != lp->head){
		items[i] = pCur->data;
		pCur = pCur->next;
		i++;
	}

	return items;
}
