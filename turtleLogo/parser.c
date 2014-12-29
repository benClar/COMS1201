/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "grammar.h"
#include "debug.h"
#include "parser.h"

/*---------- Structures -----------*/

struct program	{

		char **tokenList;
		int tokenNum;
		int cw;
		int nw;
};

struct syntaxStack	{

		SyntaxNode start;
		SyntaxNode end;
		int numNodes;

};

struct syntaxNode	{

	char *type;
	SyntaxNode previous;
	SyntaxNode next;
};

/*---------- Functions ----------*/

void createSynStack()	{

		SyntaxStack newStack = (SyntaxStack) checkMalloc(malloc(sizeof(*newStack)));
		newStack->start = newStack->end = NULL;
		newStack->numNodes = 0;
		getSynStack(newStack);
}

SyntaxStack getSynStack(SyntaxStack nStack)	{

		static SyntaxStack cStack;
		if(nStack != NULL)	{
			cStack = nStack;	
		}

		return cStack;
}

SyntaxNode createNode(char *type)	{
	SyntaxNode newNode = (SyntaxNode) checkMalloc(malloc(sizeof(*newNode)));
	newNode->type = createCharBuffer(newNode->type,getTokenLength(type));
	strcat(newNode->type,type);
	newNode->previous = NULL;
	newNode->next = NULL;
	return newNode;
}
/*
 *Removes node of specified type that is closest to the front of the stack
 */
int removeLastSNodeOfType(char *type)	{
	SyntaxStack cStack = getSynStack(NULL);
	SyntaxNode cNode = cStack->start;
	if(!strcmp(type,cStack->start->type))	{
		removeNode(cNode);
		return 1;
	}

	while(cNode->previous != NULL)	{
		if(!strcmp(type,cNode->previous->type))	{
			removeNode(cNode);			
			return 1;
		}
	}
}

void removeNode(SyntaxNode node)	{
		SyntaxStack cStack = getSynStack(NULL);
		if(node == cStack->start)	{
			cStack->start = node->previous;
			cStack->start->next = NULL;
			free(node);	
		} else if(node == cStack->end)	{
			cStack->end = cStack->end->next;
			cStack->end->previous = NULL;
			free(node);
		}else	{
			node->next->previous = node->previous;
			node->previous->next = node->next;
			free(node);
		}
}

void addNode(SyntaxNode Node)	{

	SyntaxStack cStack = getSynStack(NULL);
	if(cStack->start == NULL)	{
		cStack->start = cStack->end = Node;
	} else	{
		Node->previous = cStack->start;
		cStack->start->next = Node;
		cStack->start = Node;
	}
}

void createProgram()	{

	Program newProgram = (Program) checkMalloc(malloc(sizeof(*newProgram)));
	newProgram->tokenList = NULL;
	newProgram->tokenNum = 0;
	newProgram->cw = 0;
	getProgram(newProgram);

}

void prog()	{

	if(compCurrCw(R_BRACE))	{
	//if(!strcmp(R_BRACE,cProg->tokenList[getCw()]))	{
		setCw(getCw()+1);
		addNode(createNode(R_BRACE));
	} else {
		ERROR("Missing { to start")
	}	
}

int code()	{
	Program cProg = getProgram(NULL);

	if(checkSynStackEmpty())	{
		if(!checkNoMoreWords())	{
			ERROR("No more input expected");
			return 0;
		}
	} else if(checkNoMoreWords()) {
			ERROR("Closing Brace missed");
			return 0;	
	} else {
		return 1;
	}

	if(compCurrCw(L_BRACE))	{
		removeLastSNodeOfType(L_BRACE);		
	}	

	setCw(getCw()+1); 	//!Moving on current Word to parse
	code();
}

int getTotalTokens()	{

	return getProgram(NULL)->tokenNum;

}

int checkNoMoreWords()	{
	if(getTotalTokens() == getCw())	{
		return 1;
	} else {
		return 0;
	}
}


int checkSynStackEmpty()	{

	SyntaxStack cStack = getSynStack(NULL);
	if(cStack->start == NULL)	{
		return 1;
	}	

	return 0;
}

int compCurrCw(char *comparison)	{

	Program cProg = getProgram(NULL);
	if(!strcmp(comparison,cProg->tokenList[getCw()]))	{
		return 1;
	} else	{
		return 0;
	}
}

/*
 *Return index of current token
 */
int getCw()	{

	return getProgram(NULL)->cw;
}

void setCw(int newValue)	{
	getProgram(NULL)->cw = newValue;
}

Program getProgram(Program cProg)	{

	static Program prog;
	if(cProg != NULL)	{
		prog = cProg;
	}

	return prog;
}

void addToken(char *token)	{

	Program cProg = getProgram(NULL);
	cProg->tokenList = increaseStringList(cProg->tokenList,++cProg->tokenNum);
	cProg->tokenList[cProg->tokenNum - 1] = createCharBuffer(cProg->tokenList[cProg->tokenNum - 1],getTokenLength(token));
	strcat(cProg->tokenList[cProg->tokenNum - 1],token);
}

void printTokenList()	{

	Program cProg = getProgram(NULL);
	int i;
	for(i = 0; i < cProg->tokenNum; i++)	{
		printf("%s\n",cProg->tokenList[i]);
	}
}

char** increaseStringList(char **stringList, int current)	{

	if(current == 1)	{	
		stringList = (char**)	checkMalloc(malloc(current*sizeof(char*)));	
	} else 	{
		stringList = (char**) checkMalloc(realloc(stringList,current*sizeof(char*)));
	}

	return stringList;

}

/*---------- Testing Functions ----------*/

void parserUnitTests()	{
	enterSuite("Token Length Checker");
	testVal(getTokenLength("TestingLength"),13,"Valid: Testing Length of dummy token returns correctly");
	leaveSuite();

}
