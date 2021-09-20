/* Interface: Header file for functions with the queue
* By : Justin Leach, Victor Palczewskiand Jeff scherer
* Date : December 6, 2018
*/
#pragma once
#define MAX_QUOTE_LENGTH 140
#define iLength 140
#include<Windows.h>
//Data structures in the Queue
struct header {
	int priority;
	unsigned int checksum;
	int bReceiverAddr;
	int bSenderAddr;
	long lDataLength;
	int flag[3]; //0=nothing/ /1=huffcompress/ /2=encrypte/
};
struct node {
	header Header;
	char message[1000000];
	struct node* pNext;
};
void InitQueue(void); //Function initializes the queue by setting pHeadand Ptail to NULL(not pointing anywhere)
int IsQueueEmpty(void); //Function returns non zero when the queue is empty
void AddToQueue(struct node*, int priority); //Function makes space in memory for a new nodeand pushes new node to the end of the queue
void DeQueue(int priority); //DeQueue pops items from the queue
void traverseF(node * h); //Function goes through the list of nodes forward
void visit(node * v); //Function goes to message in the current nodeand prints it
void SendQuote(bool headers, bool encrypte, bool correct); //function gets random quoutes and queues them to send
void readQueue(void); //Function reads the queue
int queuesize(node * h); //function finds one queue size
int queuesizecall(void); //function calls the queue size function on all FIFO queues to find the total size