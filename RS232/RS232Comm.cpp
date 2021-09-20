/* Interface: Header file for the RS232 Functions
* By: Justin Leach, Victor Palczewski and Jeff scherer
* Date: December 6, 2018
*/
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RS232Comm.h"
#include "Queues.h"
#include "BST.h"
#define EX_FATAL 1
// Ensure that default character set is not Unicode
// Communication variables and parameters
HANDLE hCom; // Pointer to a COM port
int nComRate = 300; // Baud (Bit) rate in bits/second
int nComBits = 8; // Number of bits per frame
COMMTIMEOUTS timeout; // A commtimout struct variable
// Initializes the port and sets the communication parameters
void initPort() {
	createPortFile(); // Initializes hCom to point to PORT4(port 4 is used by USB‐Serial adapter on my laptop)
		purgePort(); // Purges the COM port
	SetComParms(); // Uses the DCB structure to set up the COM port
		purgePort();
}
// Purge any outstanding requests on the serial port (initialize)
void purgePort() {
	PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT |
		PURGE_TXCLEAR);
}
// Output message to port (COM5)
void outputToPort(LPCVOID buf, DWORD szBuf) {
	int i = 0;
	DWORD NumberofBytesTransmitted;
	LPDWORD lpErrors = 0;
	LPCOMSTAT lpStat = 0;
	i = WriteFile(
		hCom, // Write handle pointing to COM port
		buf, // Buffer size
		szBuf, // Size of buffer
		&NumberofBytesTransmitted, // Written number of bytes
		NULL
		);
	// Handle the timeout error
	if (i == 0) {
		printf("\nWrite Error: 0x%x\n", GetLastError());
		ClearCommError(hCom, lpErrors, lpStat); // Clears the device error flag to enable additional inputand output operations.Retrieves information ofthe communications error.
	}
	else
		printf("\nSuccessful transmission, there were %ld bytes transmitted\n",
			NumberofBytesTransmitted);
}
unsigned long inputFromPort(LPVOID buf, DWORD szBuf) {
	int i = 0;
	DWORD NumberofBytesRead;
	LPDWORD lpErrors = 0;
	LPCOMSTAT lpStat = 0;
	i = ReadFile(
		hCom, // Read handle pointing to COM port
		buf, // Buffer size
		szBuf, // Size of buffer ‐ Maximum number of bytes to read
		& NumberofBytesRead,
		NULL
	);
	// Handle the timeout error
	if (i == 0) {
		printf("\nRead Error: 0x%x\n", GetLastError());
		ClearCommError(hCom, lpErrors, lpStat); // Clears the device error flag to enable additional inputand output operations.Retrieves information ofthe communications error.
			return 0;
	}
	else
		printf("\nSuccessful reception!, There were %ld bytes read\n",
			NumberofBytesRead);
	return (NumberofBytesRead);
}
// Sub functions called by above functions


// Set the hCom HANDLE to point to a COM port, initialize for reading and writing, open the portand set securities

void createPortFile() {
	// Call the CreateFile() function
	hCom = CreateFile(
		"COM7", // COM port number ‐‐> If COM# is larger than 9 then use the following syntax‐‐ > "\\\\.\\COM10"
		GENERIC_READ | GENERIC_WRITE, // Open for read and write
		NULL, // No sharing allowed
		NULL, // No security
		OPEN_EXISTING, // Opens the existing com port
		FILE_ATTRIBUTE_NORMAL, // Do not set any file attributes ‐‐ > Use synchronous operation
		NULL // No template
	);
	if (hCom == INVALID_HANDLE_VALUE) {
		printf("\nFatal Error 0x%x: Unable to open\n", GetLastError());
	}
	else {
		printf("\nCOM is now open\n");
	}
}
static int SetComParms() {
	DCB dcb; // Windows device control block
		// Clear DCB to start out clean, then get current settings
		memset(&dcb, 0, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);
	if (!GetCommState(hCom, &dcb))
		return(0);
	// Set our own parameters from Globals
	//dcb.fRtsControl = RTS_CONTROL_TOGGLE;
	dcb.BaudRate = nComRate; // Baud (bit) rate
	dcb.ByteSize = (BYTE)nComBits; // Number of bits(8)
	dcb.Parity = 0; // No parity
	dcb.StopBits = ONESTOPBIT; // One stop bit
	if (!SetCommState(hCom, &dcb))
		return(0);
	// Set communication timeouts (SEE COMMTIMEOUTS structure in MSDN) ‐ want a fairly long timeout
		memset((void*)&timeout, 0, sizeof(timeout));
	timeout.ReadIntervalTimeout = 100; // Maximum time allowed to elapse before arival of next byte in milliseconds.If the interval between
										//the arrival of any two bytes exceeds this amount, the ReadFile operation is
										//completedand buffered data is returned
		timeout.ReadTotalTimeoutMultiplier = 1; // The multiplier used to calculate the total time‐out period for read operations in milliseconds.
												//For each read operation this value is multiplied by the requested number of bytes to be read
		timeout.ReadTotalTimeoutConstant = 100000; // A constant added to the calculation of the total time‐out period.This constant is added to the
													//resulting product of the ReadTotalTimeoutMultiplierand the number of bytes (above).
		SetCommTimeouts(hCom, &timeout);
	return(1);
}
//Funtion transmits a single user entered text message
void transmit(bool headers, bool encrypte, bool correct) {
	int pri = 1;
	DCB* portrate;
	portrate = (DCB*)calloc(1, sizeof(DCB));
	node* temp = (struct node*)calloc(1, sizeof(*temp));
	getc(stdin);
	printf("Enter a message to send\n");
	fgets(temp‐ > message, 140, stdin);
	//add a null terminator
	if ((strlen(temp‐ > message) > 0) && (temp‐ > message[strlen(temp‐ > message) ‐ 1]
		== '\n'))
		temp‐ > message[strlen(temp‐ > message) ‐ 1] = '\0';
	//clear the user input if message is over 140 characters
	if (strlen(temp‐ > message) == 139) {
		while (fgetc(stdin) != '\n') {}
	}
	if (headers) { //if headers are on get priority
		printf("Enter the message priority from 0 to 7\n");
		while (1) {
			pri = 0;
			scanf(" %d", &pri);
			if ((pri == 7) || (pri == 6) || (pri == 5) || (pri == 4) || (pri == 3)
				|| (pri == 2) || (pri == 1) || (pri == 0)) {
				temp‐ > Header.priority = pri;
				break;
			}
			else
				printf("\nInvalid Character\n");
		}
	}
	//fill header
	temp‐ > Header.lDataLength = strlen(temp‐ > message);
	temp‐ > Header.checksum = checksum(temp‐ > message, temp‐ > Header.lDataLength);
	temp‐ > Header.bReceiverAddr = RECEIVERADDRESS;
	temp‐ > Header.bSenderAddr = SENDERADDRESS;
	//show comm status
	GetCommState(hCom, portrate);
	printf("Baudrate: %d\n", portrate‐ > BaudRate);
	printf("\nTransmitting...\n");
	if (encrypte) { //if encrypte is on encrypte the message and set flag
		printf("\nMessage: %s\n\n", temp‐ > message);
	encrypt(temp‐ > message, strlen(temp‐ > message));
	temp‐ > Header.flag[2] = 1;
	}
	else
		temp‐ > Header.flag[2] = 0;
	if (headers) {
		if (correct) { //if data correct is on send header 3 times
			outputToPort(&temp‐ > Header, sizeof(header));
			Sleep(200); //allow receiver side to timeout and distinguise between transmissions
				outputToPort(&temp‐ > Header, sizeof(header));
			Sleep(200);
		}
		outputToPort(temp, sizeof(header) + strlen(temp‐ > message));
	}
	else
		outputToPort(temp‐ > message, strlen(temp‐ > message));
	printf("\n\nMessage sent: %s\n\n", temp‐ > message);
	free(temp);
}
//Function receives any number of texts or quotes
void receive(bool headers, bool correct) {
	int j, i;
	printf("How many messages would you like to receive?\n:");
	scanf("%d", &i);
	for (j = 0; j < i; j++) {
		node* temp = (struct node*)calloc(1, sizeof(*temp));
		header* temp1 = (header*)calloc(1, sizeof(*temp1));
		header* temp2 = (header*)calloc(1, sizeof(*temp2));
		printf("receiving...");
		if (headers) { //if headers are on receive a node
			if (correct) { //if data correct is on receive the header 3 times
				inputFromPort(temp1, sizeof(header));
				inputFromPort(temp2, sizeof(header));
				printf("\nMessage Priority: %d\n", temp1‐ > priority);
				printf("Receiver Address: %d\n", temp1‐ > bReceiverAddr);
				printf("Sender Address: %d\n", temp1‐ > bSenderAddr);
				printf("checksum: %d\n", temp1‐ > checksum);
				printf("Length of Message: %d\n", temp1‐ > lDataLength);
				printf("flags: %d, %d, %d\n\n", temp1‐ > flag[0], temp1‐ > flag[1],
					temp1‐ > flag[2]);
				printf("\nMessage Priority: %d\n", temp2‐ > priority);
				printf("Receiver Address: %d\n", temp2‐ > bReceiverAddr);
				printf("Sender Address: %d\n", temp2‐ > bSenderAddr);
				printf("checksum: %d\n", temp2‐ > checksum);
				printf("Length of Message: %d\n", temp2‐ > lDataLength);
				printf("flags: %d, %d, %d\n\n", temp2‐ > flag[0], temp2‐ > flag[1],
					temp2‐ > flag[2]);
			}
			inputFromPort(temp, sizeof(node)); // Receive string from port
				if (temp‐ > Header.bReceiverAddr == RECEIVERADDRESS) {
					if (correct) {//if data correct is on call voteon function
						voteon(&temp‐ > Header, temp1, temp2);
					}
					printf("\nMessage Priority: %d\n", temp‐ > Header.priority);
					printf("Receiver Address: %d\n", temp‐ > Header.bReceiverAddr);
					printf("Sender Address: %d\n", temp‐ > Header.bSenderAddr);
					printf("checksum: %d\n", temp‐ > Header.checksum);
					printf("Length of Message: %d\n\n", temp‐ > Header.lDataLength);
					if (temp‐ > Header.flag[2] == 1) { //if encrypted flag is set dechipher message
							decipher(temp‐ > message, temp‐ > Header.lDataLength);
					}
					printf("Received Calculated Checksum: %d\n\n", checksum(temp‐> message, temp‐ > Header.lDataLength));
					printf("Message Received: %s\n\n\n\n", temp‐ > message); //
					Display message from port
						AddToQueue(temp, temp‐ > Header.priority); //add message to queue
						Insertt(temp‐ > Header.bSenderAddr); //add sender ID to adress book(BST)
				}
		}
		else { //if headers are off set everything to null and add to queue
			inputFromPort(temp‐ > message, 140);
			temp‐ > Header.bReceiverAddr = NULL;
			temp‐ > Header.bSenderAddr = NULL;
			temp‐ > Header.checksum = NULL;
			temp‐ > Header.flag[0, 1, 2] = NULL;
			temp‐ > Header.lDataLength = NULL;
			printf("Message Received: %s\n\n", temp‐ > message); // Display
			message from port
				AddToQueue(temp, 0);
		}
		free(temp);
		//free(temp1);
		//free(temp1);
	}
}
// Closes the handle pointing to the COM port
void closehandle(void) {
	CloseHandle(hCom);
}
//function finds the check sum of the message by adding and moding with 0x100
unsigned int checksum(char* temp, long length) {
	unsigned char* newBuf8; //this pointer used if 8bits are needed

unsigned int sum = 0;
unsigned int checksum = 0;
int i = 0;
newBuf8 = (unsigned char*)temp; //Cast the input buffer as an unsigned char to group 1 byte at a time
for (i = 0; i < length; i++) {
	sum += *newBuf8;
	newBuf8++;
}
checksum = sum % 0x100;
return(checksum);
}
//function encrypts the data by shifting the ascii by 12
void encrypt(char* data, int datalen) {
	for (int i = 0; i < datalen; i++) {
		data[i] = (data[i] + 12);
	}
}
//function deciphers the data by shifting the ascii back by 12
void decipher(char* data, int datalen) {
	for (int i = 0; i < datalen; i++) {
		data[i] = (data[i] ‐ 12);
	}
}
//function encrypts the audio message by swapping bits
void encrypt2(char* data, int datalen) {
	char* newData = (char*)calloc(datalen, sizeof(char));
	int j = 0;
	char fC;
	char sC;
	for (int i = 0; i < datalen ‐ 1; i = i + 2) {
		fC = data[i];
		sC = data[i + 1];
		newData[i] = sC;
		newData[i + 1] = fC;
	}
	memcpy(data, newData, datalen);
	free(newData);
	return;
}
//function compares three headers and picks what happens most often and writes to the first header 'a'
void voteon(header * a, header * b, header * c) {
	//if a is different and b = a then set a to equal b and c otherwise dont do anything
		if (b‐ > bReceiverAddr == c‐ > bReceiverAddr && a‐ > bReceiverAddr != b‐> bReceiverAddr)a‐ > bReceiverAddr = b‐ > bReceiverAddr;
	if (b‐ > bSenderAddr == c‐ > bSenderAddr && a‐ > bSenderAddr != b‐ > bSenderAddr)
		a‐ > bSenderAddr = b‐ > bSenderAddr;
	if (b‐ > checksum == c‐ > checksum && a‐ > checksum != b‐ > checksum)
		a‐ > checksum = b‐ > checksum;
	if (b‐ > lDataLength == c‐ > lDataLength && a‐ > lDataLength != b‐ > lDataLength)
		a‐ > lDataLength = b‐ > lDataLength;
	if (b‐ > priority == c‐ > priority && a‐ > priority != b‐ > priority)
		a‐ > priority = b‐ > priority;
	for (int i = 0; i < 3; i++) {
		if (b‐ > flag[i] == c‐ > flag[i] && a‐ > flag[i] != b‐ > flag[i])
			a‐ > flag[i] = b‐ > flag[i];
	}
}