#ifndef testLib_h
#define testLib_h

#include "libraryFunctions.h"

/*----------TypeDefs-----------*/

typedef struct testSuite *TestSuite;
/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define TRUE        1       //!For testing.  Expecting true value.
#define FALSE       0       //!For testing.  Expecting False value.

/*----------Structures----------*/



/*----------Function Prototypes-----------*/

void createSuite();
TestSuite getSuite(TestSuite tSuite);
void testVal(int testResult,int expectedResult, char *description);
void leaveSuite();
void enterSuite(char *suiteName);

#endif
