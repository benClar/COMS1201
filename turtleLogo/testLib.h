#ifndef testLib_h
#define testLib_h

#include "libraryFunctions.h"

/*----------TypeDefs-----------*/

typedef struct testSuite *TestSuite;
/*----------Enumerated Types-----------*/

typedef enum testType	{
	EQUALS,
	GREATER,
	LESS,
	NOTEQUALS	
} testType;

/*----------Symbolic Constants-----------*/

#define TRUE        1       //!For testing.  Expecting true value.
#define FALSE       0       //!For testing.  Expecting False value.

/*----------Structures----------*/



/*----------Function Prototypes-----------*/

void createSuite();
TestSuite getSuite(TestSuite tSuite);
void testVal(double testResult,double expectedResult, char *description, testType t);
void leaveSuite();
void enterSuite(char *suiteName);
void finishTesting();
int greaterThan(double res, double comp);
int lessThan(double res, double comp);
int equals(double res, double comp);
int notEquals(double res, double comp);
#endif
