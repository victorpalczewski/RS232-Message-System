/* Filename: sound.cpp
Author: Michael Galle
Date: Nov 2017
Details: Implementation ‐ Contains functions for Windows sound API (sound
recording & playback)
*/
#pragma comment(lib, "Ws2_32.lib") // Make sure we are linking against the Ws2_32.lib library
#pragma comment(lib, "Winmm.lib") // Make sure we are linking against the Winmm.lib library ‐ some functions / symbols from this library(Windows sound API) are used
#include <stdio.h>
#include <Windows.h> // Contains WAVEFORMATEX structure
#include <mmsystem.h>
#include <math.h>
#include "sound.h"
#include "RS232Comm.h"
#include "Queues.h"
#include "huffman.h"
	extern HANDLE hCom;
// BUFFERS
short iBigBuf[SAMPLES_SEC * RECORD_TIME];
long lBigBufSize = SAMPLES_SEC * RECORD_TIME; // total number of samples
// output and input channel parameters
static int g_nSamplesPerSec = SAMPLES_SEC;
static int g_nBitsPerSample = 16;
static HWAVEOUT HWaveOut; /* Handle of opened WAVE Out and In
device */
static HWAVEIN HWaveIn;
static WAVEFORMATEX WaveFormat; /* WAVEFORMATEX structure for reading
in the WAVE fmt chunk */
static WAVEHDR WaveHeader[NFREQUENCIES]; /* WAVEHDR structures ‐ 1 per buffer
*/
static WAVEHDR WaveHeaderSilence;
static WAVEHDR WaveHeaderIn;
FILE* f;
short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));
bool playback = false;
/* PLAYBACK FUNCTIONS */
/*
*******************************************************************************
************** */
// Prototypes needed
void SetupFormat(WAVEFORMATEX* wf); // Used by InitializePlayback()
int WaitOnHeader(WAVEHDR* wh, char cDit); // Used by Playbuffer()
int InitializePlayback(void)

{
	int rc;
	// set up the format structure, needed for playback (and recording)
	SetupFormat(&WaveFormat);
	// open the playback device
	rc = waveOutOpen(&HWaveOut, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0,
		CALLBACK_NULL);
	if (rc) {
		printf("Unable to open Output sound Device! Error %x.", rc);
		return(0);
	}
	return(1);
}
// Function needed by InitializePlayback()
/* SetupFormat() initializes a WAVEFORMATEX structure to the required parameters
(sample rate, bits per sample, etc) */
static void SetupFormat(WAVEFORMATEX* wf)
{
	wf‐ > wFormatTag = WAVE_FORMAT_PCM;
	wf‐ > nChannels = 1;
	wf‐ > nSamplesPerSec = g_nSamplesPerSec;
	wf‐ > wBitsPerSample = g_nBitsPerSample;
	wf‐ > nBlockAlign = wf‐ > nChannels * (wf‐ > wBitsPerSample / 8);
	wf‐ > nAvgBytesPerSec = wf‐ > nSamplesPerSec * wf‐ > nBlockAlign;
	wf‐ > cbSize = 0;
	return;
}
/* PlayBuffer() */
int PlayBuffer(short* piBuf, long lSamples)
{
	static WAVEFORMATEX WaveFormat; /* WAVEFORMATEX structure for reading in
	the WAVE fmt chunk */
	static WAVEHDR WaveHeader; /* WAVEHDR structure for this buffer */
	MMRESULT mmErr;
	int rc;
	// stop previous note (just in case)
	waveOutReset(HWaveOut); // is this good?
	// get the header ready for playback
	WaveHeader.lpData = (char*)piBuf;
	WaveHeader.dwBufferLength = lSamples * sizeof(short);
	rc = waveOutPrepareHeader(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
	if (rc) {
		printf("Failed preparing WAVEHDR, error 0x%x.", rc);
		return(0);
	}
	WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP | WHDR_ENDLOOP);
	// play the buffer. This is NON‐blocking.
mmErr = waveOutWrite(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
// wait for completion
rc = WaitOnHeader(&WaveHeader, 0);
// give back resources
waveOutUnprepareHeader(HWaveOut, &WaveHeader, sizeof(WAVEHDR));
return(rc);
}
// Function needed by Playbuffer()
static int WaitOnHeader(WAVEHDR* wh, char cDit)
{
	long lTime = 0;
	// wait for whatever is being played, to finish. Quit after 10 seconds.
	for (; ; ) {
		if (wh ‐> dwFlags & WHDR_DONE) return(1);
		// idle for a bit so as to free CPU
		Sleep(100L);
		lTime += 100;
		if (lTime >= 10000) return(0); // timeout period
		if (cDit) printf("%c", cDit);
	}
}
void ClosePlayback(void)
{
	int i;
	for (i = 0; i < NFREQUENCIES; ++i) {
		waveOutUnprepareHeader(HWaveOut, &WaveHeader[i], sizeof(WAVEHDR));
	}
	// close the playback device
	waveOutClose(HWaveOut);
	return;
}
/* RECORDING FUNCTIONS */
/*
*******************************************************************************
************** */
int InitializeRecording(void)
{
	MMRESULT rc;
	// set up the format structure, needed for recording.
	SetupFormat(&WaveFormat);
	// open the recording device
	rc = waveInOpen(&HWaveIn, WAVE_MAPPER, &WaveFormat, (DWORD)NULL, 0,
		CALLBACK_NULL);
	if (rc) {
		printf("Unable to open Input sound Device! Error %x.", rc);
		return(0);
	}
// prepare the buffer header for use later on
WaveHeaderIn.lpData = (char*)iBigBuf;
WaveHeaderIn.dwBufferLength = lBigBufSize * sizeof(short);
rc = waveInPrepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
if (rc) {
	printf("Failed preparing input WAVEHDR, error 0x%x.", rc);
	return(0);
}
return(1);
}
int RecordBuffer(short* piBuf, long lBufSize)
{
	static WAVEFORMATEX WaveFormat; /* WAVEFORMATEX structure for reading in
	the WAVE fmt chunk */
	static WAVEHDR WaveHeader; /* WAVEHDR structure for this buffer */
	MMRESULT mmErr;
	int rc;
	printf("Recording now.....");
	// stop previous recording (just in case)
	waveInReset(HWaveIn); // is this good?
	// get the header ready for recording. This should not be needed here AND in init.
		WaveHeader.lpData = (char*)piBuf;
	WaveHeader.dwBufferLength = lBufSize * sizeof(short);
	rc = waveInPrepareHeader(HWaveIn, &WaveHeader, sizeof(WAVEHDR));
	if (rc) {
		printf("Failed preparing WAVEHDR, error 0x%x.", rc);
		return(0);
	}
	WaveHeader.dwFlags &= ~(WHDR_BEGINLOOP | WHDR_ENDLOOP);
	// Give the buffer to the recording device to fill.
	mmErr = waveInAddBuffer(HWaveIn, &WaveHeader, sizeof(WAVEHDR));
	// Record the buffer. This is NON‐blocking.
	mmErr = waveInStart(HWaveIn);
	// wait for completion
	rc = WaitOnHeader(&WaveHeader, '.');
	// stop input
	waveInStop(HWaveIn);
	return(rc);
}
void CloseRecording(void)
{
	waveInUnprepareHeader(HWaveIn, &WaveHeaderIn, sizeof(WAVEHDR));
// close the playback device
waveInClose(HWaveIn);
return;
}
/*********************** Switch Functions **********************************/
//function records 10 seconds of audio
void Record(void) {
	InitializeRecording();
	RecordBuffer(iBigBuf, lBigBufSize);
	CloseRecording();
	playback = true;
	return;
}
//function plays recorded buffer
void Playback(void) {
	if (playback == false) {
		printf("Buffer is empty\n");
		return;
	}
	InitializePlayback();
	printf("\nPlaying recording from buffer\n");
	PlayBuffer(iBigBuf, lBigBufSize);
	ClosePlayback();
	return;
}
//function saves audio to a file
void Save(void) {
	char filename[60];
	printf("Enter a path and filename to save to Ex. (C:\\myfiles\
\recording.dat):\n");
	scanf("%s", &filename);
	f = fopen(filename, "wb");
	if (!f) {
		printf("unable to open %s\n", filename);
		return;
	}
	printf("Writing to sound file ...\n");
	fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
	fclose(f);
}
//function opens audio file and plays it
void Replay(void) {
	char filename[60];
	printf("Enter a path and filename to replay from Ex. (C:\\myfiles\
\recording.dat):\n");
	scanf("%s", &filename);
	//open file
f = fopen(filename, "rb");
if (!f) {
	printf("unable to open %s\n", "C:\\myfiles\\recording.dat");
	return;
}
printf("Reading from sound file ...\n");
fread(iBigBufNew, sizeof(short), lBigBufSize, f); // Record to new buffer iBigBufNew
fclose(f);
//play audio
InitializePlayback();
printf("\nPlaying recording from saved file ...\n");
PlayBuffer(iBigBufNew, lBigBufSize);
ClosePlayback();
return;
}
//function transmits audio
void SendAudio(bool huff, bool headers, bool encrypte, bool correct) {
	int compresslen = 0;
	node* temp = (struct node*)calloc(1, sizeof(*temp));
	DCB* hey = (DCB*)calloc(1, sizeof(DCB));
	GetCommState(hCom, hey);
	if (encrypte) { //if encrypt is on encrypt the message and set the flag
		encrypt2((char*)iBigBuf, 160000);
		temp ‐> Header.flag[2] = 1;
	}
	else
		temp‐ > Header.flag[2] = 0;
	if (huff) { //if compression is on compress the message and set the flag
		compresslen = Huffman_Compress((unsigned char*)iBigBuf, (unsigned char*)
			temp‐ > message, 160000);
		temp‐ > Header.flag[1] = 1;
	}
	else { //copy the audio buffer to the node
		memcpy(temp‐ > message, (char*)iBigBuf, 160000);
		compresslen = 160000;
		temp‐ > Header.flag[1] = 0;
	}
	printf("Baudrate: %d\n", hey‐ > BaudRate);
	printf("Transmitting...\n");
	if (headers) { //if headers are on fill the header with info
		temp‐ > Header.lDataLength = compresslen;
		temp‐ > Header.checksum = checksum(temp‐ > message, temp‐
> Header.lDataLength);
		temp‐ > Header.bReceiverAddr = RECEIVERADDRESS;
		temp‐ > Header.bSenderAddr = SENDERADDRESS;
		if (correct) { //if data corect is on transmit the header 3 tmes
			outputToPort(&temp‐ > Header, sizeof(header));
			Sleep(200);
			outputToPort(&temp‐ > Header, sizeof(header));
			Sleep(200);
		}
		outputToPort(temp, sizeof(header) + compresslen);
	}
	else
		outputToPort(temp‐ > message, compresslen);
	free(hey);
}
//Function receives audio mesage
void ReceiveAudio(bool headers, bool correct) {
	node* temp = (struct node*)calloc(1, sizeof(*temp));
	header* temp1 = (header*)calloc(1, sizeof(*temp1));
	header* temp2 = (header*)calloc(1, sizeof(*temp2));
	printf("receiving...\n");
	short message[160000];
	if (headers) { //if headers are on receive a node
		if (correct) {//if data correct is on receive 3 times
			inputFromPort(temp1, sizeof(header));
			inputFromPort(temp2, sizeof(header));
		}
		inputFromPort(temp, sizeof(header) + 160000);
		if (temp‐ > Header.bReceiverAddr == RECEIVERADDRESS) {//if the station receiver address does not match the intended adress dont do anything
				return
				if (correct) { //if data corect is on call the voteon function
					voteon(&temp‐ > Header, temp1, temp2);
				}
			if (temp‐ > Header.flag[1] == 1) { //if the comress flag is set decompress
					Huffman_Uncompress((unsigned char*)temp‐ > message, (unsigned
						char*)message,
						sizeof(temp‐ > message), 160000);
				memcpy(temp‐ > message, (char*)message, 160000);
			}
			if (temp‐ > Header.flag[2] == 1) {//if the encrypt flag is set decipher
				encrypt2(temp‐ > message, 160000);
			}
			printf("\nReceiver Address: %d\n", temp‐ > Header.bReceiverAddr);
			printf("Sender Address: %d\n", temp‐ > Header.bSenderAddr);
			printf("checksum: %d\n", temp‐ > Header.checksum);
			printf("Length of Message: %d\n", temp‐ > Header.lDataLength);
			printf("Received Calculated Checksum: %d\n\n\n", checksum(temp‐
> message, temp‐ > Header.lDataLength));
			InitializePlayback();
			printf("\nPlaying recording from buffer\n");
			PlayBuffer((short*)temp‐ > message, 80000);
		}
	}
	else {//if headers are off receive just the message and play it
		inputFromPort(temp‐ > message, sizeof(temp‐ > message));
	InitializePlayback();
	printf("\nPlaying recording from buffer\n");
	PlayBuffer((short*)temp‐ > message, 80000);
	}
	ClosePlayback();
	free(temp);
	free(temp1);
	free(temp2);
}
//function transmits a file
void SendFile(bool huff, bool headers, bool encrypte, bool correct) {
	int compresslen = 0;
	char filename[60];
	FILE* File;
	long Size;
	char* buffer;
	size_t result;
	node* temp = (struct node*)calloc(1, sizeof(*temp));
	DCB* hey = (DCB*)calloc(1, sizeof(DCB));
	printf("Enter a path and filename to open from Ex. C:\\panda.png:\n");
	scanf("%s", &filename);
	File = fopen(filename, "rb");
	if (File == NULL) {
		printf("File error");
		return;
	}
	//file size:
	fseek(File, 0, SEEK_END);
	Size = ftell(File);
	rewind(File);
	// allocate memory to hold the file:
	buffer = (char*)malloc(sizeof(char) * Size);
	if (buffer == NULL) {
		printf("Memory error");
		return;
	}
	// put file into the buffer:
	result = fread(buffer, 1, Size, File);
	if (result != Size) {
		printf("Reading error");
		return;
	}
	//set the checksum
	temp‐ > Header.checksum = checksum(buffer, Size);
	if (encrypte) {//if encrypte is on encrypte the data and set the flag
		encrypt(buffer, Size);
	temp‐ > Header.flag[2] = 1;
	}
	else
		temp‐ > Header.flag[2] = 0;
	if (huff) {//if compress is on compress the data and set the flag
		compresslen = Huffman_Compress((unsigned char*)buffer, (unsigned char*)
			temp‐ > message, Size);
		temp‐ > Header.flag[1] = 1;
	}
	else {
		memcpy(temp‐ > message, buffer, Size);
		compresslen = Size;
		temp‐ > Header.flag[1] = 0;
	}
	if (headers) {//if headers are on fill the header with info
		temp‐ > Header.lDataLength = Size;
		temp‐ > Header.bReceiverAddr = RECEIVERADDRESS;
		temp‐ > Header.bSenderAddr = SENDERADDRESS;
		//get the comm state
		GetCommState(hCom, hey);
		printf("Baudrate: %d\n", hey‐ > BaudRate);
		printf("transmitting...");
		if (correct) {
			outputToPort(&temp‐ > Header, sizeof(header));
			Sleep(200);
			outputToPort(&temp‐ > Header, sizeof(header));
			Sleep(200);
		}
		outputToPort(temp, sizeof(header) + compresslen);
	}
	else {//if headers are off just send the file
		GetCommState(hCom, hey);
		printf("Baudrate: %d\n", hey‐ > BaudRate);
		printf("transmitting...");
		outputToPort(temp‐ > message, compresslen);
	}
	fclose(File);
	free(buffer);
	free(hey);
	free(temp);
	return;
}
void ReceiveFile(bool headers, bool correct) {
	char message[1000000];
	unsigned long x;
	char filename[60];
	printf("receiving...\n");
	node* temp = (struct node*)calloc(1, sizeof(*temp));
header * temp1 = (header*)calloc(1, sizeof(*temp1));
header* temp2 = (header*)calloc(1, sizeof(*temp2));
if (headers) {//if headers are on receive a node
	if (correct) {//if data correct is on receive 3 times
		inputFromPort(temp1, sizeof(header));
		inputFromPort(temp2, sizeof(header));
	}
	x = inputFromPort(temp, sizeof(node));
	if (temp‐ > Header.bReceiverAddr != RECEIVERADDRESS)//if the station receiver address does not match the intended adress dont do anything
		return
		return;
	if (correct) {//if data correct is on call voteon function
		voteon(&temp‐ > Header, temp1, temp2);
	}
	if (temp‐ > Header.flag[1] == 1) { //if compress flag is set decompress the message
			Huffman_Uncompress((unsigned char*)temp‐ > message, (unsigned char*)
				message,
				1000000, temp‐ > Header.lDataLength);
		memcpy(temp‐ > message, (char*)message, 1000000);
	}
	if (temp‐ > Header.flag[2] == 1) { //if encrypte flag is set decipher the message
			decipher(temp‐ > message, temp‐ > Header.lDataLength);
	}
	printf("\nReceiver Address: %d\n", temp‐ > Header.bReceiverAddr);
	printf("Sender Address: %d\n", temp‐ > Header.bSenderAddr);
	printf("checksum: %d\n", temp‐ > Header.checksum);
	printf("Length of Message: %d\n", temp‐ > Header.lDataLength);
	printf("Received Calculated Checksum: %d\n\n\n", checksum(temp‐ > message,
		temp‐ > Header.lDataLength));
}
else {//if headers are off just receive a message
	x = inputFromPort(temp‐ > message, sizeof(temp‐ > message));
}
printf("Enter a path and filename to save to Ex. C:\\panda.png:\n");
scanf("%s", &filename);
//save the file
f = fopen(filename, "wb");
if (!f) {
	printf("unable to open %s\n", filename);
	return;
}
printf("Writing to file ...\n");
if (headers)
fwrite(temp‐ > message, sizeof(char), temp‐ > Header.lDataLength, f);
else
fwrite(temp‐ > message, sizeof(char), x, f);
fclose(f);
//open the file
system(filename);
free(temp);
free(temp1);
free(temp2);
}