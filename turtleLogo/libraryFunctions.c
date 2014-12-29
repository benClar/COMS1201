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

int getTokenLength(char *token) {
        int i;
        for(i = 0; token[i] != '\0'; i++)   {
            /*do nothing*/
        }
        return i;
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

