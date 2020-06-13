/*
Andy Bredar
COEN 12
5-26-20
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

//Node struct definition
typedef struct node {
	int slots; //how many indices there are in this node
	int full; //how many indices are full in this node
	struct node *next; //points to the next node
	struct node *prev; //points to the previous node
	int first; //index of the first element in the queue
	int last; //index of the last element in the queue
	void **data; //points to the array of generic data pointers
}NODE;

//List struct definition
typedef struct list {
	int totalItems; //total number of elements in all of the nodes
	int count; //number of nodes in the list
	NODE *head; //points to the head node
	NODE *tail; //points to the tail node
}LIST;

/*
As nodes are added the nodes should increase
in size so that less operations have to be done to find an element

I will start looking for a particular item from the front
*/

//function that creates a node
// O(1) time complexity
/*
static NODE* createNode(int slots){
	NODE *np = malloc(sizeof(NODE));
	assert(np != NULL);

	np->next = NULL;
	np->prev = NULL;
	
	np->slots = slots;
	np->first = 0;
	np->last = 0;
	
	np->full = 0;
	np->data = malloc(sizeof(void*) * slots);
	assert(np->data != NULL);

	return np;
}
*/

//return a pointer to a new list
// O(1) time complexity
LIST *createList(void){

	LIST *lp;
	lp = malloc(sizeof(LIST));

	assert(lp != NULL);

	lp->totalItems = 0; //initialize the number of elements in the list to 0
	lp->count = 0; //initialize the number of nodes in the list to 0

	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);

	//initialize next and prev pointers
	lp->head->next = NULL;
	lp->head->prev = NULL;
	
	//intialize the indices of the front and the back of the queue as 0
	lp->head->first = 0;
	lp->head->last = 0;

	//intitialize the number of full slots as 0 out of a total of 64 for the first node
	lp->head->full = 0;
	lp->head->slots = 64;

	//allocate a void* array with as many indices as their are "slots"
	lp->head->data = malloc(sizeof(void*) * 64);

	lp->tail = lp->head;
	return lp;
}

// deallocate memory associated with the list pointed by lp
// O(log n) time comlexity because the number of nodes grows with logarithmic complexity
//so we will only need to do proportionately less operations to free the whole list as the list gets larger
void destroyList(LIST *lp){

	assert(lp != NULL);
	NODE *pCur = lp->head;
	NODE *pDel;	

	//iterate through the linked list and deallocate nodes
	while(pCur != NULL){
	
		pDel = pCur->next;
		free(pCur->data);
		free(pCur);
		pCur = pDel;
		//pDel = pCur;
		//pCur = pCur->next;
		//free(pDel);
	}

	free(lp);
}

//return the number of items in the list pointed by lp
// O(1) time complexity
int numItems(LIST *lp){

	assert (lp != NULL);
	return lp->totalItems;	
}

//add item as the first item in the list pointed by lp
// O(1) time complexity
void addFirst(LIST *lp, void *item){

	assert(lp != NULL);

	//if the head node is full then allocate a new node to accomodate new data
	if((lp->head->full) >= (lp->head->slots)){

		NODE *newNode = malloc(sizeof(NODE));

		//put the new node in the linked list by reassigning pointers
		newNode->next = lp->head;
		newNode->prev = NULL;
		lp->head->prev = newNode;

		//initialize the number of slots for the new node
		newNode->full = 0;
		int newSlots = 2 * (lp->head->slots);
	
		//set the newNode as the head of the linked list
		lp->head = lp->head->prev;

		//update slots
		lp->head->slots = newSlots;

		//allocate an array of void* twice the size of the array in the node to the right
		lp->head->data = malloc(sizeof(void*) * (newSlots));
		assert(newNode-> data != NULL);

		//increment the counter of the number of nodes
		lp->count += 1;
	}

	//make sure there is still room in the queue to add an item (count<length)
	assert((lp->head->full) < (lp->head->slots));

	//decrease the index of the "first index"
	lp->head->first--;
	if(lp->head->first < 0){
		lp->head->first = lp->head->slots - 1;
	}

	//put the item in the front of the queue in the current node
	//first represents the first index of the queue (not the array)
	//new first index is to the left of the current first index
	//lp->head->first = ((lp->head->first) - 1) % lp->head->slots;
	lp->head->data[lp->head->first] = item;
	
	//update counters etc.
	lp->head->full += 1;
	lp->totalItems += 1;

	return;
}

//add item as the last item in the list pointed by lp
// O(1) time complexity
void addLast(LIST *lp, void *item){

	assert(lp != NULL);
	
	int index;

	//if the tail node is full then allocate a new node to accomodate new data
	if((lp->tail->full) >= (lp->tail->slots)){

		NODE *newNode = malloc(sizeof(NODE));

		//put the new node in the linked list by reassigning pointers
		newNode->next = NULL;
		newNode->prev = lp->tail;
		lp->tail->next = newNode;

		//initialize the number of slots for the new node
		newNode->full = 0;
		int newSlots = 2 * (lp->tail->slots);

		//set the newNode as the tail of the linked list
		lp->tail = lp->tail->next;

		//update slots
		lp->tail->slots = newSlots;
	
		//allocate an array of void* twice the size of the array in the node to the left
		newNode->data = malloc(sizeof(void*) * (newSlots));
		assert(newNode->data != NULL);

		//increment the counter of the number of nodes
		lp->count += 1;
	}

	//make sure there is still room in the queue to add an item (count<length)
	assert((lp->tail->full) < (lp->tail->slots));

	//find the index where the last element should go
	index = lp->tail->first + lp->tail->full;
	if(index >= lp->tail->slots){
		index = index - lp->tail->slots;
	}

	//insert the item
	lp->tail->data[index] = item;
	
	//update counters etc.
	lp->head->full += 1;
	lp->totalItems += 1;

	//lp->tail->data[((lp->tail->full) + (lp->tail->first)) % lp->tail->slots] = item;
	//update the "last" index number	
	lp->tail->last = ((lp->head->first) + (lp->head->full)) % lp->head->slots;

	return;
}

//remove and return the first item pointed to by lp (the list must not be empty)
// O(1) time complexity
void *removeFirst(LIST *lp){

	assert(lp != NULL);

	//check that there is an item to remove and return
	assert(lp->totalItems > 0);

	//remove the head node if it is empty	
	if(lp->head->full == 0){

		NODE *delNode = lp->head;
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(delNode->data);
		free(delNode);
	}

	//store the first value in a temporary variable
	void* temp = lp->head->data[lp->head->first];
	
	//free(lp->head->data[lp->head->first]);
	
	//update the "first" index
	if(++lp->head->first >= lp->head->slots)
		lp->head->first = 0;

	//lp->head->first = ((lp->head->first) + 1) % lp->head->slots;
	lp->totalItems -= 1;
	lp->head->full -= 1;

	return temp;
}

//remove and retur the last item in the list pointed by lp (the list must not be empty)
// O(1) time complexity
void *removeLast(LIST *lp){

	assert(lp != NULL);
	
	//check that there is an item to remove and return
	assert(lp->totalItems > 0);

	//remove the tail node if it is no longer needed
	if(lp->tail->full == 0){
		
		NODE *delNode = lp->tail;
		lp->tail = lp->tail->prev;
		lp->tail->next = NULL;
		free(delNode->data);
		free(delNode);
	}

	//create a temporary value to store the last element
	void* temp = lp->tail->data[(lp->tail->full + lp->tail->first -1) % lp->tail->slots];

	//free(lp->tail->data[lp->tail->last]);

	if(--lp->tail->last < 0){
		lp->tail->last = lp->tail->slots - 1;
	}

	lp->totalItems -= 1;
	lp->tail->full -= 1;

	return temp;		
}

//return the item at position index in the list pointed to by lp (the index must be within range)
// worst case O(log n) runtime if the index we are looking for is at the end of the list we
// will have to iterate through the entire list but the nodes have varying size
void *getItem(LIST *lp, int index){

	assert(lp != NULL);
	assert(index >= 0);
	assert(lp->totalItems > 0);
	
	//make sure the index is less than the total number of elements
	assert(index < lp->totalItems);

	//traverse linked list looking for the node that has the
	//array with the index that we are looking for
	NODE *pCur = lp->head;
	while(index >= pCur->full){

		index = index - pCur->full;
		pCur = pCur->next;
	}
	
	return pCur->data[(pCur->first + index) % pCur->slots];
}

//update the item at position index in the list pointed by lp (the index must be within range)
// worst case O(log n) runtime because we may have to iterate through the whole list but
//as the list grows the number of nodes increases logarithmically
void setItem(LIST *lp, int index, void *item){

	assert(lp != NULL);	

	//make sure the index is less than the total number of elements
	assert(index >= 0 && index < lp->totalItems);

	//like above, traverse the linked list looking for the node that has
	//the array with the index we are looking for
	NODE *pCur = lp->head;
	while(index >= pCur->full){

		index = index - (pCur->full);
		pCur = pCur->next;
	}

	//update the item at the given index
	pCur->data[(pCur->first + index) % pCur->slots] = item;
}
