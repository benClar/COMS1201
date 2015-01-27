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
	int tFailure;
};

/*---------- Functions ----------*/

void createSuite()	{

	TestSuite tSuite = (TestSuite) checkMalloc(malloc(sizeof(*tSuite)));
	tSuite->sNumber = 0;
	tSuite->passNumber = 0;
	tSuite->failNumber = 0;
	tSuite->tFailure = 0;
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
void testVal(double testResult,double expectedResult, char *description, testType t)  {
	TestSuite tSuite = getSuite(NULL);
	int success = 0;
	switch(t)	{
		case EQUALS:
			success = equals(testResult,expectedResult);
			break;
		case GREATER:
			success = greaterThan(testResult,expectedResult);
			break;
		case LESS:
			success = lessThan(testResult,expectedResult);
			break;
		case NOTEQUALS:
			success = notEquals(testResult,expectedResult);
			break;
		default:
			fprintf(stderr,"Unrecognised test operator\n");
			exit(1);
			break;
	}
    if(!success)    {
        fprintf(stderr,"###FAIL###: \"%s\" has failed \n",description);
		tSuite->failNumber++;
		tSuite->tFailure++;
    } else {
        printf("\"%s\" : pass \n",description);
		tSuite->passNumber++;
    }
}

int greaterThan(double res, double comp)	{

	if(res>comp)	{
		return 1;
	}
	return 0;
}

int lessThan(double res, double comp)	{
	if(res<comp)	{
		return 1;
	}
	return 0;
}

int equals(double res, double comp)	{
	if(res == comp)	{
		return 1;
	} 

	return 0;
}

int notEquals(double res, double comp)	{
	if(res != comp)	{
		return 1;
	}	
	return 0;
}

void enterSuite(char *suiteName)	{
	int i;

	TestSuite tSuite = getSuite(NULL);
	tSuite->sNumber++;
	for(i = 0; suiteName[i] != '\0'; i++)	{
		tSuite->currentSuite = increaseCharBuffer(tSuite->currentSuite,i+1);
		tSuite->currentSuite[i] = suiteName[i];
	}
	tSuite->currentSuite = increaseCharBuffer(tSuite->currentSuite,i+1);
	tSuite->currentSuite[i] = '\0';

	printf("###Entering suite [%d] : %s ###\n",tSuite->sNumber, tSuite->currentSuite);
}

void leaveSuite()	{
	TestSuite tSuite = getSuite(NULL);
	printf("### Leaving Suite [%d] : %s ###\n",tSuite->sNumber,tSuite->currentSuite);
	if(tSuite->failNumber == 0)	{
		printf("SUCCESS: %d tests passed\n\n",tSuite->passNumber);
	} else {
		printf("FAILURE: %d tests failed\n\n",tSuite->failNumber);
	}
	tSuite->failNumber = tSuite->passNumber = 0;
	free(tSuite->currentSuite);
	tSuite->currentSuite = NULL;
}

void finishTesting()	{
	TestSuite tSuite = getSuite(NULL);
	if(!tSuite->tFailure)	{
		printf("####################\n# All Tests Passed #\n####################\n");
		exit(0);
	} else	{
		printf("##################\n# %d Tests Failed #\n##################\n",tSuite->tFailure);
		exit(1);	
	}
}
