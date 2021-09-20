/* Interface for getting quotes from file
* By: Justin Leach, Victor Palczewski and Jeff scherer
* Date: December 6, 2018
*/
#pragma once
// These functions will produce the inputs to the GetMessageFromFile() function
int frandNum(int min, int max); // Function gets a random number between minand max(max is the number of quotes in the file)
int fnumQuotes(void); // Function returns number of quotes in the file(only need to run once)
long int* fquoteIndices(int numQuotes); // Function returns an array that indicates the start of every quote in the file(number of characters from the start of the file)
int* fquoteLength(int numQuotes, long int* quoteIndices); // Function returns the smaller of the actual quote length or MAX_QUOTE_LENGTH
int GetMessageFromFile(char* buff, int iLen, int randNum, int numQuotes, long int*quoteIndices, int* quoteLengths); // Function that gets q random quote from the FortuneCookies file