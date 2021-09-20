/* Interface for Binary Search Trees
* By: Justin Leach, Victor Palczewski and Jeff scherer
* Date: December 6, 2018
*/
#pragma once
//typedefs for tree
typedef struct BSTnode BSTNode;
typedef BSTNode* link;
//data structure for the nodes
struct BSTnode {
	link pLeft;
	link pRight;
	int SenderID;
	int Count;
};
//BST functions
void InitiateBST(void); //Function initializes the tree by setting pHead to NULL
link NEW(int ID, link left, link right); //creates a new node
link Insert(link h, int ID); //inserts a new node into the tree
void Insertt(int ID); //function to call insert function
int Count(link h); //counts how many nodes ar in the tree
int Height(link h); //finds the height of the tree
void traverseIN(BSTnode* h); //traverse the tree to print
void readTree(); //function called in main to call traverse function
int search(BSTnode* h, int ID); //searches BST for a specific node