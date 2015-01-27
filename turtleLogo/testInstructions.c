//////
//
//testInstructions.c
//This provides a data structure to store instructions that would be interpreted and actioned 
//when the program is in normal operations.  In testing mode, before actioning instructions,
//instructions are stored in the parseArr structure, which can then be examined to see the exact
//order and values for testing purposes.
//
///////////////

/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "testInstructions.h"

/*---------- Functions ----------*/
//! Head for test parsed instruction list
struct parseArr	{

	ParseNode *nArr;  //@ Array of successfully parsed instructions
	int items;		//@Number of held instructions
};

//! node making up test instruction list
struct parseNode	{

	char *command;	//@instruction string
	double val;		//@Instruction value
};

/*
 *Creating parsed instruction array
 */
void createParseArr()	{

	ParseArr nPArr = (ParseArr) checkMalloc(malloc(sizeof(*nPArr)));
	nPArr->nArr = NULL;
	nPArr->items = 0;
	getParseArr(nPArr);
}

/*
 *Adding instruction to instruction array
 */
int addParseNode(char *cmd, int value)	{
	assert(cmd != NULL);
	ParseArr pArr = getParseArr(NULL);
	if(pArr->nArr == NULL)	{
		pArr->nArr = (ParseNode*) checkMalloc(malloc(sizeof(*pArr->nArr)));
		pArr->items++;
	} else	{
		pArr->nArr = (ParseNode*) checkMalloc(realloc(pArr->nArr,++pArr->items*sizeof(*pArr->nArr[(pArr->items - 1)])));
	}
	pArr->nArr[(pArr->items - 1)] = (ParseNode) checkMalloc(malloc(sizeof(*pArr->nArr[(pArr->items - 1)])));
	pArr->nArr[(pArr->items - 1)]->val = value;
	pArr->nArr[(pArr->items - 1)]->command = (char*) checkMalloc(calloc(getTokenLength(cmd),sizeof(char)));
	strcat(pArr->nArr[(pArr->items - 1)]->command,cmd);
	return pArr->items;
}

/*
 *Accessor to get number of instructions currently held
 */
int getParseNodeNumber()	{
	return getParseArr(NULL)->items;
}

/*
 * Returns pointer to Instruction array
 */
ParseArr getParseArr(ParseArr cp)	{

	static ParseArr currPArr;

	if(cp != NULL)	{
		currPArr = cp;
	}
	return currPArr;
}


/*
 *Accessor for instruction list node command string
 */
char* pCommand(ParseNode n)	{
	if(n != NULL)	{
		return n->command;
	}
	return NULL;
}

/*
 *Accessor for instruction list node command value
 */
int pVal(ParseNode n)	{
	if(n != NULL)	{
		return n->val;
	}

	ERROR("Attempt to access non existent symbol");
}

/*
 *Accessor for specific instruction list node
 */
ParseNode getSpecParseNode(int n)	{
	ParseArr pArr = getParseArr(NULL);
	if(n<=pArr->items)	{
		return pArr->nArr[n-1];
	}

	return NULL;
}


/*
 *Clears instruction array
 */
void clearParseArr()	{

	ParseArr pArr = getParseArr(NULL);
	int i;
	if(pArr != NULL)	{
		for(i = 0; i < pArr->items; i++)	{
			free(pArr->nArr[i]);
		}
		pArr->items = 0;
	}
	pArr->nArr = NULL;
}

/*
 *Frees parsed instruction array
 */
void freeParseArr()	{
	clearParseArr();
	ParseArr pArr = getParseArr(NULL);

	if(pArr != NULL)	{
		free(pArr);
	}
}

/*---------- Testing Functions ----------*/

void interpreterUnitTests()	{
	parseArrayTests();
}

void parseArrayTests()	{

	enterSuite("Managing Parse Array");
	testVal(addParseNode(FORWARD,10),1,"Valid: One parse node has been added",EQUALS);
	testVal(addParseNode(FORWARD,20),2,"Valid: Two parse nodes have been added",EQUALS);
	testVal(pVal(getSpecParseNode(1)),10,"Valid: Parse node one value is 10",EQUALS);
	testVal(pVal(getSpecParseNode(2)),20,"Valid: Parse node two value is 20",EQUALS);
	clearParseArr();
	testVal(addParseNode(FORWARD,20),1,"Valid: One parse node added to cleared parse array",EQUALS);
	clearParseArr();
	testVal(checkForNull(pCommand(getSpecParseNode(1))),1,"Invalid: No parse nodes in array",EQUALS);	
	leaveSuite();
}
