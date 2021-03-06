/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "libraryFunctions.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Main -----------*/


/*---------- Functions ----------*/

char* increaseCharBuffer(char *buffer, int newSize)	{
	if(newSize == 1)	{
		return((char*) checkMalloc(calloc(newSize,sizeof(char))));
	} else {
		return((char*) checkMalloc(realloc(buffer,newSize * sizeof(char))));
	}
}

char* createCharBuffer(char *buffer, int size)	{

	return((char*) checkMalloc(calloc(size,sizeof(char))));

}

char getFirstCharacter(char *string)	{
	return string[0];
}

int getTokenLength(char *token) {
        int i;
        for(i = 0; token[i] != '\0'; i++)   {
            /*do nothing*/
        }
        return i+1;
}

int checkForNull(void *n)	{
	if(n == NULL)	{
		return 1;
	} else {
		return 0;
	}
}

/*
 *Validation for Malloc
 */
void *checkMalloc(void *malP)   {

    if (malP == NULL)   {
        fprintf(stderr, "null address generated\n");
        exit(1);
    }
    return malP;
}

void libraryUnitTests()	{

	enterSuite("Library Function Tests");
	testVal(getTokenLength("TEST"),5,"Valid: Length of word and null character is 5",EQUALS);
	testVal(checkForNull("TEST"),0,"Invalid: string passed",EQUALS);
	testVal(checkForNull(NULL),1,"Valid: Null pointer passed",EQUALS);
	testVal(getFirstCharacter("TEST"),'T',"Valid: T is first character of TEST",EQUALS);
	leaveSuite();
}
