/* Filename: sound.h
* Author: Michael Galle, Justin Leach, Victor Palczewski and Jeff scherer
* Date: December 6, 2018
* Details: Interface ‐ Contains prototypes for Windows sound API (sound
recording and playback functions)
*/
#pragma once
// CONSTANTS ‐ SET RECORDING TIME AND RATE
#define DEFAULT_NSAMPLES 4000
#define MIN_BUFSIZE 1000
#define RECORD_TIME 10 // seconds to record from mic
#define SAMPLES_SEC 8000 // number of samples per second
#define NFREQUENCIES 96 // number of frequencies used ‐ leave this alone
// FUNCTION PROTOTYPES
/* ************************ */
// Playback
int InitializePlayback(void);
int PlayBuffer(short* piBuf, long lSamples);
void ClosePlayback(void);
// Recording
int InitializeRecording(void);
int RecordBuffer(short* piBuf, long lBufSize);
void CloseRecording(void);
// Justin Jeff and Victors functions
/* ************************ */
void Record(void); //function records 10 seconds of audio
void Playback(void); //function plays recorded buffer
void Save(void); //function saves audio to a file
void Replay(void); //function opens audio file and plays it
void SendAudio(bool huff, bool headers, bool encrypte, bool correct); //function transmits audio
void ReceiveAudio(bool headers, bool correct); //function receives audio
void SendFile(bool huff, bool headers, bool encrypte, bool correct); //function opensand sends a file
void ReceiveFile(bool headers, bool correct); //function receivesand saves a file