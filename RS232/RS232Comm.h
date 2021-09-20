




/* Interface: Header file for the RS232 Functions
* By: Justin Leach, Victor Palczewski and Jeff scherer
* Date: December 6, 2018
*/
#pragma once
// Prototype the functions to be used
#include <Windows.h>
#define BUFSIZE 140
#define RECEIVERADDRESS 1011
#define SENDERADDRESS 1010
#include"Queues.h"
void initPort(); //initialize comm‐port
void purgePort(); //closes the comm port
void outputToPort(LPCVOID buf, DWORD szBuf); //transmits to the comm port
unsigned long inputFromPort(LPVOID buf, DWORD szBuf); //receives from the comm port
void voteon(header* a, header* b, header* c); //data correction for the header
void createPortFile(); //Sets comm port settings
static int SetComParms(); //sets comm port parameters
void transmit(bool headers, bool encrypte, bool correct); //transmits a text
void receive(bool headers, bool correct); //receives a text
void closehandle(void); //closes the handle
unsigned int checksum(char* temp, long length); //gets checksum of the data
void encrypt(char* data, int datalen); //encryption function
void decipher(char* data, int datalen); //decrypts the encrypted transmition
void encrypt2(char* data, int datalen); //encryption for audio