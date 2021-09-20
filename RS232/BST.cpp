/* Implementation for Binary Search Trees
* By: Justin Leach, Victor Palczewski and Jeff scherer
* Date: December 6, 2018
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "BST.h"
BSTNode * BSTHead; //head of the tree
//Function initailizes the tree by setting the head to NULL
void InitiateBST(void) {
	BSTHead = NULL;
}
//Function creates a new node
link NEW(int ID, link left, link right) {
	link pNew = (link)malloc(sizeof(*pNew));
	pNew‐ > SenderID = ID;
	pNew‐ > Count = 1;
	pNew‐ > pLeft = left;
	pNew‐ > pRight = right;
	return(pNew);
}
//Function calls insert if the ID is not in the tree
void Insertt(int ID) {
	if (search(BSTHead, ID) == 1) return;
	BSTHead = Insert(BSTHead, ID);
}
//Function makes space in memory for a new node and pushes new node to the end of the queue
link Insert(link h, int ID) {
	//if first item ‐ nothing else
	int rc;
	if (h == NULL) {
		h = NEW(ID, NULL, NULL);
		return h;
	}
	else {
		rc = ID; //go left or right
		if (rc < h‐>SenderID) {
			h‐ > pLeft = Insert(h‐ > pLeft, ID);
		}
		else {
			h‐ > pRight = Insert(h‐ > pRight, ID);
		}
		return(h); //pointer to child passed to parent
	
	}
}
//Function to call traverse from main without needing the head pointer in main
void readTree() {
	traverseIN(BSTHead);
}
//Function searches the tree for a sender ID
int search(BSTnode* h, int ID) {
	if (h == NULL) {
		return 0;
	}
	if (h‐ > SenderID == ID) {
		h‐ > Count++;
		return 1;
	}
	if (h‐ > SenderID < ID)
		return(search(h‐ > pLeft, ID));
	else
		return(search(h‐ > pRight, ID));
}
//goes through the tree printing left child then parent then right
void traverseIN(BSTnode* h) {
	if (h == NULL)
		return;
	traverseIN(h‐ > pLeft);
	printf("You have %d new messages from Sender ID %d\n", h‐ > Count, h‐ > SenderID);
	traverseIN(h‐ > pRight);
	return;
}
//Function counts the number of nodes in the tree
int Count(link h) {
	if (h == NULL)
		return (0);
	return(1 + Count(h‐ > pLeft) + Count(h‐ > pRight));
}
//Function counts how tall the tree is
int Height(link h) {
	int HeightL, HeightR;
	if (h == NULL)
		return(‐1); //return ‐1 if reached a NULL
	HeightL = Height(h‐ > pLeft); //Recursively call left and right links
	HeightR = Height(h‐ > pRight);
	if (HeightL > HeightR)
		return(HeightL + 1);
	return(HeightR + 1);
}