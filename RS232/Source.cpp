/* Filename: main.cpp
Author: Justin Leach
Date: Nov 2017
Details: Testing mainline for Windows sound API
*/
#include "sound.h"
#include "Queues.h"
#include "RS232Comm.h"
#include "BST.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
int main(int argc, char* argv[]) {
	bool huff = 0;
	bool headers = 1;
	bool encrypte = 0;
	bool correct = 0;
	bool cont;
	bool first = true;
	char in[20];
	//initiate the queues, BST and Comm port
	InitiateBST();
	InitQueue();
	initPort();
	while (cont = true) { //enter a loop while the user enters their command
		_flushall();
		if (first == false) {
			printf("\n\n");
			system("pause");
		}
		first = false;
		system("cls");
		printf("Enter a command.\n\nt ‐ Transmit\nr ‐ Receive\ne ‐ Encrypte On/Off\nc ‐ Huffman Encoding On / Off\nh ‐ Headers On / Off\no ‐ Data Corection On / Off\nq ‐ read queue\na ‐ Record Audio\np ‐ Playback Audio \ns ‐ Save Audio\nf ‐ Play audio from file\nx ‐ Exit\n\n");
			//print what flags are currently on
			if (huff)
				printf("Huffman Encoding is on\n");
			else
				printf("Huffman Encoding is off\n");
		if (correct)
			printf("Data Correction is on\n");
		else
			printf("Data Correction is off\n");
		if (encrypte)
			printf("Encryption is on\n");
		else
			printf("Encryption is off\n");
		if (headers)
			printf("Headers are on\n");
		else
			printf("Headers are off\n");
	//print the number of messages in the queue
	printf("There are %d messages in the queue\n", queuesizecall());
	printf("\nC://");
	scanf(" %[^\n]s", in);
	switch (in[0])
	{
	case 'e':
		system("cls");
		if (encrypte)
			encrypte = 0;
		else {
			encrypte = 1;
			headers = 1;
		}
		break;
	case 'o':
		system("cls");
		if (correct)
			correct = 0;
		else {
			correct = 1;
			headers = 1;
		}
		break;
	case 'c':
		system("cls");
		if (huff)
			huff = 0;
		else {
			huff = 1;
			headers = 1;
		}
		break;
	case 'h':
		system("cls");
		if (headers) {
			huff = 0;
			headers = 0;
			encrypte = 0;
		}
		else
			headers = 1;
		break;
		//Transmit Sub Menu
	case 't':
		system("cls");
	printf("Enter a command.\nt ‐ Transmit text\na ‐ Transmit Audio\nf ‐Transmit File\nq ‐ Transmit Quotes\nx ‐ Exit to last window\n");
		printf("\nC://");
	scanf(" %[^\n]s", in);
	switch (in[0]) {
	case 't':
		system("cls");
		transmit(headers, encrypte, correct);
		break;
	case 'q':
		system("cls");
		SendQuote(headers, encrypte, correct);
		break;
	case 'a':
		system("cls");
		SendAudio(huff, headers, encrypte, correct);
		break;
	case 'f':
		system("cls");
		SendFile(huff, headers, encrypte, correct);
		break;
	case 'x':
		break;
	default:
		system("cls");
		printf("Invalid Character\n");
		break;
	}
	break;
	//Receive Sub Menu
	case 'r':
		system("cls");
		printf("Enter a command.\nt ‐ Receive text\na ‐ Receive Audio\nf ‐ Receive File\nx ‐ Exit to last window\n");
			printf("\nC://");
		scanf(" %[^\n]s", in);
		switch (in[0]) {
		case 't':
			system("cls");
			receive(headers, correct);
			break;
		case 'f':
			system("cls");
			ReceiveFile(headers, correct);
			break;
		case 'a':
			system("cls");
			ReceiveAudio(headers, correct);
			break;
		case 'x':
			break;
		default:
			system("cls");
			printf("Invalid Character\n");
			break;
		}
		break;
	case 'a':
		system("cls");
		Record();
		break;
	case 'p':
		system("cls");
		Playback();
		break;
	case 's':
		system("cls");
		Save();
		break;
	case 'f':
		system("cls");
		Replay();
		break;
	case 'q':
		system("cls");
		readTree();
		readQueue();
		break;
	case 'x': //go to exit function when x is pressed
		purgePort(); // Purge the port
		closehandle(); // Closes the handle pointing to the COM port
			cont = false;
		return 0;
	default:
		system("cls");
		printf("Invalid Character\n");
		break;
	}
	}
	return 0;
}