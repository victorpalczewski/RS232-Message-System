/* Implementation: Header file for functions with the queue
* By: Justin Leach, Victor Palczewski and Jeff scherer
* Date: December 6, 2018
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "Queues.h"
#include "Message.h"
#include "RS232Comm.h"
#include "BST.h"
#include <time.h>
//Initailizing the heads and tails of the priority FIFO queue
node * pHead0;
node* pHead1;
node* pHead2;
node* pHead3;
node* pHead4;
node* pHead5;
node* pHead6;
node* pHead7;
node* pTail0;
node* pTail1;
node* pTail2;
node* pTail3;
node* pTail4;
node* pTail5;
node* pTail6;
node* pTail7;
node* temp;
int counter = 0;
//Initialize the queue by setting pHead and Ptail to NULL (not pointing anywhere)
void InitQueue(void) {
	pHead0 = pTail0 = NULL;
	pHead1 = pTail1 = NULL;
	pHead2 = pTail2 = NULL;
	pHead3 = pTail3 = NULL;
	pHead4 = pTail4 = NULL;
	pHead5 = pTail5 = NULL;
	pHead6 = pTail6 = NULL;
	pHead7 = pTail7 = NULL;
	return;
}
//Return non zero when the queue is empty
int IsQueueEmpty(void) {
	return (pHead0 == NULL);
}

//Function makes space in memory for a new node and pushes new node to the end of the queue
void AddToQueue(node * pn, int priority) {
	temp = (node*)malloc(sizeof(*temp)); //malloc here to simplify main
	switch (priority) {
	case 0:
		if (pHead0 == NULL) { //if first item make phead the
			new node
				pHead0 = pTail0 = temp;
		}
		else {
			pTail0‐ > pNext = temp; //if not first go to what tail
			points toand make node there
		}
		*temp = *pn;
		temp‐ > pNext = NULL;
		pTail0 = temp;
		return;
	case 1:
		if (pHead1 == NULL) { //if first item make phead the
			new node
				pHead1 = pTail1 = temp;
		}
		else {
			pTail1‐ > pNext = temp; //if not first go to what tail
			points toand make node there
		}
		*temp = *pn;
		temp‐ > pNext = NULL;
		pTail1 = temp;
		return;
	case 2:
		if (pHead2 == NULL) { //if first item make phead the
			new node
				pHead2 = pTail2 = temp;
		}
		else {
			pTail2‐ > pNext = temp; //if not first go to what tail points toand make node there
		}
		*temp = *pn;
		temp‐ > pNext = NULL;
		pTail2 = temp;
		return;
	case 3:
		if (pHead3 == NULL) { //if first item make phead the new node
				pHead3 = pTail3 = temp;
		}
		else {
			pTail3‐ > pNext = temp; //if not first go to what tail points to and make node there
		}
		*temp = *pn;
		temp‐ > pNext = NULL;
		pTail3 = temp;
		return;
	case 4:
		if (pHead4 == NULL) { //if first item make phead the new node
				pHead4 = pTail4 = temp;
		}
		else {
			pTail4‐ > pNext = temp; //if not first go to what tail points toand make node there
		}
		*temp = *pn;
		temp‐ > pNext = NULL;
		pTail4 = temp;
		return;
	case 5:
		if (pHead5 == NULL) { //if first item make phead the new node
				pHead5 = pTail5 = temp;
		}
		else {
			pTail5‐ > pNext = temp; //if not first go to what tail points toand make node there
		}
		*temp = *pn;
		temp‐ > pNext = NULL;
		pTail5 = temp;
		return;
	case 6:
		if (pHead6 == NULL) { //if first item make phead the new node
				pHead6 = pTail6 = temp;
		}
		else {
			pTail6‐ > pNext = temp; //if not first go to what tail points toand make node there
		}
		*temp = *pn;
		temp‐ > pNext = NULL;
		pTail6 = temp;
		return;
	case 7:
		if (pHead7 == NULL) { //if first item make phead the new node
				pHead7 = pTail7 = temp;
		}
		else {
			pTail7‐ > pNext = temp; //if not first go to what tail points to and make node there
		}
		*temp = *pn;
		temp‐ > pNext = NULL;
		pTail7 = temp;
		return;
	}
}
//DeQueue pops items from the queue
void DeQueue(int priority) {
	switch (priority) {
	case 0: //if empty return otherwise set the head to what head point and free the original head
			if (pHead0 == NULL) return;
		temp = pHead0;
		pHead0 = pHead0‐ > pNext;
		free(temp);
	case 1:
		if (pHead1 == NULL) return;
		temp = pHead1;
		pHead1 = pHead1‐ > pNext;
		free(temp);
	case 2:
		if (pHead2 == NULL) return;
		temp = pHead2;
		pHead2 = pHead2‐ > pNext;
		free(temp);
	case 3:
		if (pHead3 == NULL) return;
		temp = pHead3;
		pHead3 = pHead3‐ > pNext;
		free(temp);
	case 4:
		if (pHead4 == NULL) return;
		temp = pHead4;
		pHead4 = pHead4‐ > pNext;
		free(temp);
	case 5:
		if (pHead5 == NULL) return;
		temp = pHead5;
		pHead5 = pHead5‐ > pNext;
		free(temp);
	case 6:
		if (pHead6 == NULL) return;
		temp = pHead6;
		pHead6 = pHead6‐ > pNext;
		free(temp);
	case 7:
		if (pHead7 == NULL) return;
		temp = pHead7;
		pHead7 = pHead7‐ > pNext;
	free(temp);
	}
}
//goes through the list of nodes forward
void traverseF(node* n) {
	if (n == NULL) return;
	counter = counter + 1;
	visit(n); // calls ‘visit’ before recursive call
	traverseF(n‐ > pNext);
}
//goes to message in the current node and prints it
void visit(node* h) {
	printf("\nMessage Priority: %d\n", h‐ > Header.priority);
	printf("Receiver Address: %d\n", h‐ > Header.bReceiverAddr);
	printf("Sender Address: %d\n", h‐ > Header.bSenderAddr);
	printf("checksum: %d\n", h‐ > Header.checksum);
	printf("Length of Message: %d\n", h‐ > Header.lDataLength);
	printf("Message Received: %s\n\n", h‐ > message);
	return;
}
void SendQuote(bool headers, bool encrypte, bool correct) {
	int numQuotes; // Number of quotes in the file
	long int* quoteIndices; // Array of quote locations in the file
	(index correspondes to quote number)
		int* quoteLengths; // Array of quote lengths (index correspondes to quote number)
	int Num;
	int x;
	int pri;
	printf("How many quotes do you want to print?\n");
	scanf_s("%d", &Num);
	// Count the number of quotes in the file and index it for fast access (only need to do this once)
	numQuotes = fnumQuotes(); // Number of quotes
	quoteIndices = fquoteIndices(numQuotes);// Index locations of the quotes
	quoteLengths = fquoteLength(numQuotes, quoteIndices); // Length of each quote(up to MAX_QUOTE_LENGTH) ‐ cut off after
		srand(time(NULL)); // Seed the random number generator
	node* p;
	p = (node*)malloc(sizeof(*p));
	//set the priority of the message
	if (headers) {
		printf("Enter the messages priority from 0 to 7\n");
		while (1) {
			getc(stdin);
		pri = 0;
		scanf(" %d", &pri);
		if ((pri == 7) || (pri == 6) || (pri == 5) || (pri == 4) || (pri ==
			3) || (pri == 2) || (pri == 1) || (pri == 0)) {
			p‐ > Header.priority = pri;
			break;
		}
		else
			printf("\nInvalid Character\n");
		}
	}
	//Adds the messages to queue
	for (x = 0; x < Num; x++) {
		//fill node with information
		GetMessageFromFile(p‐ > message, MAX_QUOTE_LENGTH, frandNum(1, numQuotes),
			numQuotes, quoteIndices, quoteLengths);
		p‐ > Header.lDataLength = strlen(p‐ > message);
		p‐ > Header.checksum = checksum(p‐ > message, p‐ > Header.lDataLength);
		p‐ > Header.bReceiverAddr = RECEIVERADDRESS;
		p‐ > Header.bSenderAddr = SENDERADDRESS;
		p‐ > Header.flag[0, 1, 2] = NULL;
		if (headers) { //if headers are on send headers
			if (encrypte) { //if encrypte is on encrypte the message and set the flag
					printf("\nMessage: %s\n\n", p‐ > message);
				encrypt(p‐ > message, p‐ > Header.lDataLength);
				p‐ > Header.flag[2] = 1;
			}
			else {
				p‐ > Header.flag[2] = 0;
			}
			if (correct) { //if data correct is on transmit the header 3 times
				outputToPort(&temp‐ > Header, sizeof(header));
				Sleep(200);
				outputToPort(&temp‐ > Header, sizeof(header));
				Sleep(200);
			}
			outputToPort(p, sizeof(header) + p‐ > Header.lDataLength);
			printf("\n\nMessage Sent: %s", p‐ > message);
		}
		else { //if headers are off just send the message
			outputToPort(p‐ > message, p‐ > Header.lDataLength);
			printf("\n\nMessage Sent: %s", p‐ > message);
		}
		Sleep(200);
	}
	return;
}
//Function reads all the queues in priority order
void readQueue(void) {
	//call the traverse functions forward
	traverseF(pHead7);
	for (int i = 0; i < counter; i++) {
		DeQueue(7);
	}
	counter = 0;
	traverseF(pHead6);
	for (int i = 0; i < counter; i++) {
		DeQueue(6);
	}
	counter = 0;
	traverseF(pHead5);
	for (int i = 0; i < counter; i++) {
		DeQueue(5);
	}
	counter = 0;
	traverseF(pHead4);
	for (int i = 0; i < counter; i++) {
		DeQueue(4);
	}
	counter = 0;
	traverseF(pHead3);
	for (int i = 0; i < counter; i++) {
		DeQueue(3);
	}
	counter = 0;
	traverseF(pHead2);
	for (int i = 0; i < counter; i++) {
		DeQueue(2);
	}
	counter = 0;
	traverseF(pHead1);
	for (int i = 0; i < counter; i++) {
		DeQueue(1);
	}
	counter = 0;
	traverseF(pHead0);
	for (int i = 0; i < counter; i++) {
		DeQueue(0);
	}
}
//Function finds the size of one queue
int queuesize(node* h) {
	if (h == NULL)
		return (0);
	return(1 + queuesize(h‐ > pNext));
}
//Function finds the total size of all the queues
int queuesizecall(void) {
	return (queuesize(pHead0) + queuesize(pHead1) + queuesize(pHead2) + queuesize
	(pHead3) +queuesize(pHead4) + queuesize(pHead5) + queuesize(pHead6) +
		queuesize(pHead7));
}