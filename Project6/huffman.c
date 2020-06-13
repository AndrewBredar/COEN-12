/*
Andy Bredar
COEN 12
Project 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "pack.h"
#include "pqueue.h"

typedef struct node NODE;

//function declarations
static void readFile(char* filename);
static NODE* mknode(int data, NODE *left, NODE* right);
static int nodeCompare(NODE *np1, NODE *np2);
static int nodeDepth(NODE *np);

//declare array of character counts
int characterArray[257]; //index corresponds to each ASCII character and the value at each index is it's frequency
//declare the array of NODEs
NODE* nodes[257];

//main function - takes in a file with text and an empty output file
int main(int argc, char* argv[]){

	//if there are only two arguments, the user probably forgot to include an output file
	if(argc == 2){
		printf("Missing filename! You probably forgot and output file.");
		return 0;
	}
	else{
		readFile(argv[1]);
	}
	//create the priority queue
	PQ *pq = createQueue(nodeCompare);

	//create a counter for the total number of bits used
	int totalBits = 0;

	//make a node for each character that has a value greater than 0 in the characterArray
	for(int i = 0; i < 256; i++){
		if(characterArray[i] > 0){
			nodes[i] = mknode(characterArray[i], NULL, NULL);
			//add the node to the priority queue			
			addEntry(pq, nodes[i]); 
		}
	}
	
	//make the last node in the character array have a quantity of 0 for the EOF marker
	nodes[256] = mknode(0, NULL, NULL);
	//add this node to the priority queue
	addEntry(pq, nodes[256]);

	//while there are more than one nodes in the priority queue, keep constructing a huffman tree
	while(numEntries(pq) > 1){

		NODE *left = removeEntry(pq);
		NODE *right = removeEntry(pq);

		//the parent node will have a value equal to the sum of the counts of its two children
		NODE *parent = mknode(left->count + right->count, left, right);
		addEntry(pq, parent);
	}

	//iterate through the nodes array and print each character, its frequency, the bits required, and the total bits for each character
	for(int j = 0; j < 256; j++){
		if(nodes[j] != NULL){
			//if the letter is printable, then print the letter with the following line to express how mnay bits are needed
			printf(isprint(j) ? "%c" : "%03o", j);
			printf(": %d x %d bits =  %d bits\n", characterArray[j], nodeDepth(nodes[j]), characterArray[j] * nodeDepth(nodes[j]));
			totalBits += (characterArray[j] * nodeDepth(nodes[j]));
		}
	}
	
	//used the next line to double check the total bits number
	//printf("Total bits required: %d\n", totalBits);

	//call Professor Lonny's pack function
	pack(argv[1], argv[2], nodes);

	//destroy the queue
	destroyQueue(pq);
	return 0;
}


//open the file and incremenet the count for each letter that appears
// O(n) where n is the length of the file
static void readFile(char *fileName){

	//open the file
	FILE *fp = fopen(fileName, "r");
	if(fp == NULL){
		printf("There was an error opening the file\n");
		return;
	}
	//count the occurences of each character and increment the corresponding value in the characterArray
	int C  = getc(fp);
	while(C != EOF){
		characterArray[C]++;
		C = getc(fp);
	}

	//close the file and return
	fclose(fp);
	return;
}

//create a node and use the given data as its value
// O(1) time complexity
static NODE* mknode(int data, NODE *left, NODE *right){

	//allocate memory for the node
	NODE *root = malloc(sizeof(NODE));
	assert( root != NULL);

	//initialize its data and parent
	root->count = data;
	root->parent = NULL;

	//make the new node tha parent of the two given nodes (if they are not NULL)
	if(left != NULL){
		left->parent = root;
	}
	if(right != NULL){
		right->parent = root;
	}
	//return the pointer to the new node
	return root;
}

//calculate the depth of a given node using a recursive algorithm
// O(h) where h is the height of the tree
static int nodeDepth(NODE *np){

	assert(np != NULL);
	//base case
	if(np->parent == NULL){
		return 0;
	}
	//recursive function call
	return(nodeDepth(np->parent) + 1);
}


//comparison function
// O(1) time complexity
static int nodeCompare(NODE *node1, NODE *node2){

	assert(node1 != NULL && node2 != NULL);
	//if the value of the first node is less than the value of the second node, return -1
	if(node1->count < node2->count)
		return -1;
	//if the value of the first node is greater than the second node, return 1
	else if(node1->count > node2->count)
		return 1;
	//otherwise the nodes are equal and return 0
	else
		return 0;
}
