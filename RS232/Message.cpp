/* Implementation for getting quotes from file
* By: Justin Leach, Victor Palczewski and Jeff scherer
* Date: December 6, 2018
*/
#include "Message.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
// Function gets a random number between min and max (max is the number of quotes
in the file)
int frandNum(int min, int max) {
	return(min + (rand() % (max ‐ + 1 ‐ min)));
}
// Function returns number of quotes in the file (only need to run once)
int fnumQuotes(void) {
	FILE* fp;
	char c = 0;
	int counter = 0;
	if (fp = fopen("FortuneCookies.txt", "r")) {
		while (c != EOF) {
			c = fgetc(fp);
			if (c == '%') {
				if ((fgetc(fp)) == '%') { //if there are 2 % signs in a row
					increment the counter
						counter++;
				}
			}
		}
		fclose(fp);
		return(counter);
	}
	else {
		printf("Error: could not open file");
		fclose(fp);
		return(0);
	}
}
// Function returns an array that indicates the start of every quote in the file
(number of characters from the start of the file)
long int* fquoteIndices(int numQuotes) {
	FILE* fp;
	char c = 0;
	int i = 0;
long* indices;
indices = (long*)malloc(numQuotes * sizeof(long)); //allocate the array
if (fp = fopen("FortuneCookies.txt", "r")) { // open the file
	while (c != EOF) {
		c = fgetc(fp);
		if ((c == '%')) {
			if (fgetc(fp) == '%') {
				indices[i] = ftell(fp); //add the place in the
				file to the array if there are 2 % signs
					i++;
			}
		}
	}
	return(indices);
}
else {
	printf("Error: could not open file");
	fclose(fp);
	return(0);
}
}
// Function returns the smaller of the actual quote length or MAX_QUOTE_LENGTH
(had to do it this way because subtracting didnt give the correct number it was
	always one or two off)
	int* fquoteLength(int numQuotes, long int* quoteIndices) {
	FILE* fp;
	int* length;
	int c = 0;
	int count = ‐1;
	int i = 0;
	length = (int*)malloc(numQuotes * sizeof(int));
	if (fp = fopen("FortuneCookies.txt", "r")) {
		while (c != EOF) {
			count++;
			c = fgetc(fp);
			if ((c == '%')) {
				if (fgetc(fp) == '%') { //count how many characters in between %
					signs
						length[i] = count;
					count = ‐1;
					i++;
				}
			}
		}
		return(length);
	}
	else {
		printf("Error: could not open file");
	fclose(fp);
	return(0);
	}
}
// Function that gets q random quote from the FortuneCookies file
int GetMessageFromFile(char* buff, int iLen, int randNum, int numQuotes, long
	int* quoteIndices, int* quoteLengths) {
	FILE* fp;
	char c = 0;
	int Length;
	int i;
	if (fp = fopen("FortuneCookies.txt", "r")) {
		fseek(fp, quoteIndices[randNum], 0);
		if (quoteLengths[randNum + 1] > (iLen ‐ 1)) { // if the quote is smaller
			then 140 Lenghth is set to the length of the quote other wise it is set
				to 140 ‐ 1 for the null terminator
				Length = (iLen ‐ 1);
		}
		else {
			Length = quoteLengths[randNum + 1];
		}
		for (i = 0; i < Length; i++) { //fill the buffer with the quote
			buff[i] = fgetc(fp);
		}
		buff[Length] = '\0'; // add the null terminator to the end of
		the message
			fclose(fp);
		return(0);
	}
	else {
		printf("Error: could not open file");
		fclose(fp);
		return(1);
	}
}