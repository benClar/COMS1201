#ifndef libraryFunctions_h
#define libraryFunctions_h

#include "testLib.h"

/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define ERROR(PHRASE) { fprintf(stderr, \
                "Fatal Error %s occured in %s, line %d\n",  \
                PHRASE, __FILE__, __LINE__); exit(2); }


/*----------Structures----------*/



/*----------Function Prototypes-----------*/
char* increaseCharBuffer(char *buffer, int newSize);
char* createCharBuffer(char *buffer, int size);
void *checkMalloc(void *malP);
int getTokenLength(char *token);
char getFirstCharacter(char *string);

void libraryUnitTests();

#endif
