/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "testLib.h"

/*---------- Structures -----------*/

struct testSuite	{

	char *currentSuite;
	int sNumber;
	int passNumber;
	int failNumber;
};

/*---------- Functions ----------*/

void createSuite()	{

	TestSuite tSuite = (TestSuite) checkMalloc(malloc(sizeof(*tSuite)));
	tSuite->sNumber = 0;
	tSuite->passNumber = 0;
	tSuite->failNumber = 0;
	getSuite(tSuite);
}

TestSuite getSuite(TestSuite tSuite)	{

		static TestSuite currSuite;

		if(tSuite != NULL)	{
			currSuite = tSuite;
		}

	return currSuite;
}

/*
 * Tests return value of test functions against expected value
 */
void testVal(double testResult,double expectedResult, char *description)  {
	TestSuite tSuite = getSuite(NULL);
    if(testResult != expectedResult)    {
        fprintf(stderr,"###FAIL###: \"%s\" has failed \n",description);
		tSuite->failNumber++;
    } else {
        printf("\"%s\" : pass \n",description);
		tSuite->passNumber++;
    }
}

void enterSuite(char *suiteName)	{
	int i;

	TestSuite tSuite = getSuite(NULL);
	tSuite->sNumber++;
	for(i = 0; suiteName[i] != '\0'; i++)	{
		tSuite->currentSuite = increaseCharBuffer(tSuite->currentSuite,i+1);
		tSuite->currentSuite[i] = suiteName[i];
	}

	tSuite->currentSuite[i] = '\0';

	printf("###Entering suite [%d] : %s ###\n",tSuite->sNumber, tSuite->currentSuite);
}

void leaveSuite()	{
	TestSuite tSuite = getSuite(NULL);
	printf("### Leaving Suite [%d] : %s ###\n",tSuite->sNumber,tSuite->currentSuite);
	if(tSuite->failNumber == 0)	{
		printf("SUCCESS: %d tests passed\n",tSuite->passNumber);
	} else {
		printf("FAILURE: %d tests failed\n",tSuite->failNumber);
	}
	tSuite->failNumber = tSuite->passNumber = 0;
	free(tSuite->currentSuite);
	tSuite->currentSuite = NULL;
}
