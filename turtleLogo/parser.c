/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*---------- Custom Headers	-----------*/

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
		int instructionLength;
};

struct syntaxNode	{

	char *type;
	synType sType;
	SyntaxNode previous;
	SyntaxNode next;
};

struct calcStack	{
	CalcNode start;
	int numNodes;
};

struct calcNode	{

	CalcNode previous;
	synType type;
	int value;
	char *op;	
};

struct varTable	{

	int **vTable;	//! Stores variables
	int vNum; 	//!Tracks number of variables
};



/*---------- Functions ----------*/

void createVarTable()	{

	int i;	
	VarTable vt = (VarTable) checkMalloc(malloc(sizeof(*vt)));
	vt->vTable = (int**) checkMalloc(malloc(MAXVAR * sizeof(int*)));
	for( i = 0; i < MAXVAR; i++)	{
		vt->vTable[i] = (int *)	checkMalloc(calloc(VAR_T_COLUMNS,sizeof(int)));
	}
	vt->vNum = 0;
	getVarTable(vt);
}

VarTable getVarTable(VarTable nVT)	{

	static VarTable cVT;
	if(nVT != NULL)	{
		cVT = nVT;
	}

	return cVT;

}

char addVariable(char var, int val)	{

	VarTable vt = getVarTable(NULL);
	if(checkVarUnique(var))	{
		if(vt->vNum < MAXVAR)	{
			vt->vTable[vt->vNum][V_NAME_COL] = var;
			vt->vTable[vt->vNum][V_VAL_COL] = val;
		} else	{
			ERROR("Out of Variable memory: Only 26 variables allowed");
		}
		vt->vNum++;
	} else { 
		updateVariable(var,val);
	}
	return var;
}

int getVariable(char var)	{

	VarTable vt = getVarTable(NULL);
	int i;
	for(i = 0; i < MAXVAR; i++)	{
		if(vt->vTable[i][V_NAME_COL] == var)	{
			return vt->vTable[i][V_VAL_COL];
		}
	}
	ERROR("Undefined Symbol");
}

/*
 *Checks if a variable is present in var table
 */
char checkVarDeclared(char var)	{
	int i;
	VarTable vt = getVarTable(NULL);

	for(i = 0; i < MAXVAR; i++)	{
		if(vt->vTable[i][V_NAME_COL] == var)	{
			return var;
		}
	}
	ERROR("Undefined Symbol");

}

int checkVarUnique(char var)	{
	int i;
	VarTable vt = getVarTable(NULL);

	for(i = 0; i < MAXVAR; i++)	{
		if(vt->vTable[i][V_NAME_COL] == var)	{
			return 0;
		}
	}

	return 1;
}

int* getVarAddress(char var)	{
	int i;
	VarTable vt = getVarTable(NULL);

	for(i = 0; i < MAXVAR; i++)	{
		if(vt->vTable[i][V_NAME_COL] == var)	{
			return &vt->vTable[i][V_VAL_COL];
		}
	}

	ERROR("Undefined Symbol");

}

int updateVariable(char var,int val)	{
	
	int i;
	VarTable vt = getVarTable(NULL);

	for(i = 0; i < MAXVAR; i++)	{
		if(vt->vTable[i][V_NAME_COL] == var)	{
			vt->vTable[i][V_VAL_COL] = val;
			return 1;
		}
	}
	ERROR("Variable not declared");

}



CalcNode popCalcStack()	{

	CalcStack cStack = getCalcStack(NULL);
	CalcNode temp = NULL;
	if(cStack->start != NULL)	{
		temp = cStack->start;
		cStack->start = cStack->start->previous;
	} else	{
		addCalcNode(createCalcValNode(0)); //!Adding a 0 value node
		temp = popCalcStack();
	}

	return temp;	
}

int calcStackEmpty()	{

	if(getCalcStack(NULL)->start == NULL)	{
		return 1;
	}	else {
		return 0;
	}
}

void createCalcStack()	{
	CalcStack newCStack = (CalcStack) checkMalloc(malloc(sizeof(*newCStack)));
	newCStack->start = NULL;
	newCStack->numNodes = 0;
	getCalcStack(newCStack);
}

CalcNode createCalcNode()	{
	CalcNode newCNode = (CalcNode) checkMalloc(malloc(sizeof(*newCNode)));
	newCNode->previous = NULL;
	return newCNode;
}

CalcNode createCalcOpNode(char *op)	{

	CalcNode newCNode = createCalcNode();
	newCNode->op = (char*) checkMalloc(calloc(getTokenLength(op),sizeof(char)));
	strcat(newCNode->op,op);
	newCNode->type = OP;
	return newCNode;
}

CalcNode createCalcValNode(int value)	{
	CalcNode newCNode = createCalcNode();
	newCNode->value = value;
	newCNode->type = NUM;
	return newCNode;
}

void addCalcNode(CalcNode newNode)	{
	CalcStack cStack = getCalcStack(NULL);
	newNode->previous = cStack->start;
	cStack->start = newNode;
}

CalcStack getCalcStack(CalcStack cStack)	{

	static CalcStack currCalcStack;
	if(cStack != NULL)	{
		currCalcStack = cStack;
	}
	return currCalcStack;

}

void createSynStack()	{

	SyntaxStack newStack = (SyntaxStack) checkMalloc(malloc(sizeof(*newStack)));
	newStack->start = newStack->end = NULL;
	newStack->numNodes = 0;
	newStack->instructionLength = 0;
	getSynStack(newStack);
}

SyntaxStack getSynStack(SyntaxStack nStack)	{

	static SyntaxStack cStack;
	if(nStack != NULL)	{
		cStack = nStack;	
	}

	return cStack;
}

SyntaxNode createNode(char *type, synType sType)	{
	SyntaxNode newNode = (SyntaxNode) checkMalloc(malloc(sizeof(*newNode)));
	newNode->type = createCharBuffer(newNode->type,getTokenLength(type));
	strcat(newNode->type,type);
	newNode->previous = NULL;
	newNode->next = NULL;
	newNode->sType = sType;
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
			removeNode(cNode->previous);			
			return 1;
		}
		cNode = cNode->previous;
	}

	return 0;
}

void removeNode(SyntaxNode node)	{
	//printf("popping %s\n",node->type);
	SyntaxStack cStack = getSynStack(NULL);

	if(strcmp(node->type,R_BRACE))	{
    	cStack->instructionLength--;
	}

	if(node == cStack->start)	{
		if(node->previous != NULL)	{
			cStack->start = node->previous;
			cStack->start->next = NULL;
		} else {
			cStack->start = NULL;
		}
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
	cStack->numNodes--;
}

void popFromStack()	{
	SyntaxStack cStack = getSynStack(NULL);
	SyntaxNode Node	 = cStack->start;

	while(!strcmp(Node->type,R_BRACE))	{
		Node = Node->previous;
	}
		removeNode(Node);
}

char* addNode(SyntaxNode Node)	{

	SyntaxStack cStack = getSynStack(NULL);
	if(cStack->start == NULL)	{
		cStack->start = cStack->end = Node;
	} else	{
		Node->previous = cStack->start;
		cStack->start->next = Node;
		cStack->start = Node;
	}

	if(strcmp(Node->type,R_BRACE))	{
		cStack->instructionLength++; //!only count as part of instruction if not a brace
	}
	return Node->type;
}

void createProgram()	{

	Program newProgram = (Program) checkMalloc(malloc(sizeof(*newProgram)));
	newProgram->tokenList = NULL;
	newProgram->tokenNum = 0;
	newProgram->cw = 0;
	getProgram(newProgram);

}

void prog()	{
	setCw(0);
	if(compCurrCw(R_BRACE))	{
		addNode(createNode(R_BRACE,MAIN));
		setCw(getCw()+1);
	} else {
		ERROR("Missing { to start");
	}
	code();	
}

int code()	{
	//!Base case
	if(checkSynStackEmpty() && !checkNoMoreWords())	{
			ERROR("No more input expected");
			return 0;
	} else if(!checkSynStackEmpty() && checkNoMoreWords()) {
			ERROR("Closing Brace missed");
			return 0;	
	} else if(checkSynStackEmpty() && checkNoMoreWords()) {
		printf("No errors\n");
		return 1;
	}
	statement();
	removeCurrentInstruction();
	setCw(getCw()+1);	//!Moving on current Word to parse
	code();
	return 1;
}

void statement()	{
	Program cProg = getProgram(NULL);
	if(compCurrCw(L_BRACE))	{
		removeLastSNodeOfType(R_BRACE);	
	} else if(compCurrCw(R_BRACE))	{
		addNode(createNode(R_BRACE,MAIN));
	} else if(checkIfMovement(cProg->tokenList[getCw()]))	{
		movementParse(cProg->tokenList[getCw()]);	
	} else if(compCurrCw(DO))	{
		doParse(DO);
	} else if(compCurrCw(SET))	{
		setParse(SET);	
	} else	{
	sprint(cProg->tokenList[getCw()]);
		ERROR("Unrecognisd Symbol");
	}

}

void setParse()	{
	Program cProg = getProgram(NULL);
	addNode(createNode(getCToken(),INSTRUC));
	setCw(getCw()+1);
	if(checkIfVariable(cProg->tokenList[getCw()]))	{
		addNode(createNode(getCToken(),VAR));
		setCw(getCw()+1);
	} else	{
		ERROR("Expected VAR in SET statement");
	}
	if(compCurrCw(EQUALS))	{
		addNode(createNode(getCToken(),VAR));
		setCw(getCw()+1);
	}	else	{
		ERROR("Expected := in SET statement");
	}

	polishParse(getCToken());
	addVariable(getFirstCharacter(getKeywordFromStack(3)),strtol(getKeywordFromStack(1),NULL,10));
}

void polishParse()	{
	char calcRes[MAXCALC];
	while(!compCurrCw(END_POLISH))	{
		if(checkIfOp(getCToken()))	{
			addCalcNode(createCalcOpNode(getCToken()));	
			calculatePolish();
		} else if(checkIfVariable(getCToken()))	{
			addCalcNode(createCalcValNode(getVariable(getFirstCharacter(getCToken()))));
		} else if(checkIfNumber(getCToken()))	{
			addCalcNode(createCalcValNode(strtol(getCToken(),NULL,10)));
		} else {
			ERROR("Expected OP or VARNUM in statement");
		}
		setCw(getCw()+1);
	}
	sprintf(calcRes,"%d",popCalcStack()->value);
	if(!calcStackEmpty())	{
		ERROR("Error in POLISH statement: number of VAR and number of OP do not match");
	}
	addNode(createNode(calcRes,VAR));
	//printStack(); //!prints the current calc
}

void calculatePolish()	{
		CalcNode opNode = popCalcStack();
		CalcNode valNode_2 = popCalcStack();
		CalcNode valNode_1 = popCalcStack();	
		if(!strcmp(opNode->op,ADD))	{
			addCalcNode(createCalcValNode(valNode_1->value+valNode_2->value));
		} else if(!strcmp(opNode->op,SUB))   {
			addCalcNode(createCalcValNode(valNode_1->value-valNode_2->value));
		} else if(!strcmp(opNode->op,MULT))   {
			addCalcNode(createCalcValNode(valNode_1->value*valNode_2->value));
		} else if(!strcmp(opNode->op,DIV))   {
			addCalcNode(createCalcValNode(valNode_1->value/valNode_2->value));
		} else {
			fprintf(stderr,"Unrecognised operator in polish statement\n");
		}
		free(opNode);
		free(valNode_2);
		free(valNode_1);
}

int doParse(char *instruction)	{
	Program cProg = getProgram(NULL);
	addNode(createNode(instruction,INSTRUC));
	setCw(getCw()+1);
	char minVal, maxVal;
	int *pMaxVal;
	int mNumVal, beforeLoopPos;
	if(checkIfVariable(cProg->tokenList[getCw()])){
		addNode(createNode(getCToken(),VAR));
		minVal = getFirstCharacter(getCToken());	
		if(checkVarUnique(getFirstCharacter(getCToken())))	{
			addVariable(getFirstCharacter(getCToken()),NEW_VAR);
		} 
		setCw(getCw()+1);
	} else {
		ERROR("Expected VAR in DO statement");
	}
  	if(compCurrCw(FROM))	{
		addNode(createNode(getCToken(),CONNECTOR));
		setCw(getCw()+1);
	} else	{
		ERROR("Expected FROM in DO statememt");
	}

	if(checkIfVariable(getCToken()))	{
		addNode(createNode(getCToken(),VAR));
		updateVariable(minVal, getVariable(getFirstCharacter(getCToken())));
	} else if(checkIfNumber(getCToken()))	{
		updateVariable(minVal, strtol(addNode(createNode(getCToken(),NUM)),NULL,10));
	} else {
		ERROR("Expected VARNUM in DO statement");
	}
	setCw(getCw()+1);

	if(compCurrCw(TO))	{
		addNode(createNode(getCToken(),CONNECTOR));
		setCw(getCw()+1);
	} else {
		ERROR("Expected TO in DO statement");
	}
	
	if(checkIfVariable(getCToken()))	{
		maxVal = checkVarDeclared(getFirstCharacter(addNode(createNode(getCToken(),VAR))));
		pMaxVal = getVarAddress(maxVal);
		if(*pMaxVal != getVariable(maxVal))	{
			ERROR("Memory Error: Local value variable does not match var table");
		}
	} else if(checkIfNumber(getCToken()))	{
		mNumVal = strtol(addNode(createNode(getCToken(),NUM)),NULL,10);
		pMaxVal = &mNumVal;
	} else {
		ERROR("Expected VARNUM in DO statement");
	}

	setCw(getCw()+1);
	if(compCurrCw(R_BRACE))  {
		addNode(createNode(R_BRACE,MAIN));
		setCw(getCw()+1);
	} else {
		ERROR("Expected { in DO statement");
	}

	beforeLoopPos = getCw();
	for(;getVariable(minVal) < *pMaxVal;updateVariable(minVal,getVariable(minVal)+1))	{
		setCw(beforeLoopPos);
		while(!compCurrCw(L_BRACE))	{
			statement();
			//printf("in loop:\n");
		//	printStack();
			setCw(getCw()+1);
		}
	}
	removeLastSNodeOfType(R_BRACE);
	return 1;
}

/*int getCurrInstructionValues(int symbol)	{
	if(checkIfVariable(syntaxStackquery(symbol)))	{
		return getVariable(getFirstCharacter(syntaxStackquery(symbol)));	 	
	} else if(checkIfNumber(syntaxStackquery(symbol)))	{
		return strtol(syntaxStackquery(symbol),NULL,10);
	}

	ERROR("Not a VARNUM");
}*/

int checkWhileCondition(int current, int max)	{

	if(current > max)	{
		return 0;
	}
	return 1;
}

//! Returns stack node value from specified node from start of queue
char* syntaxStackquery(int n)	{
    SyntaxStack cStack = getSynStack(NULL);
    SyntaxNode node  = cStack->start;
	int i;
	for(i = 0; i < n; i++)	{
		if(node->previous != NULL)	{
			node = node->previous;
		}
	}
	return node->type;

}

void printStack()	{
	SyntaxStack cStack = getSynStack(NULL);
	SyntaxNode Node  = cStack->start;

	while(Node != NULL)	{
		printf("%s\n",Node->type);
		Node = Node->previous;
	}
}

char* getCToken()	{
	return getProgram(NULL)->tokenList[getCw()];
}

/*
 *Parses a movement instruction
 */
int movementParse(char *instruction)	{
	Program cProg = getProgram(NULL);
   	addNode(createNode(instruction,INSTRUC));
	setCw(getCw()+1);
	if(checkIfNumber(cProg->tokenList[getCw()]))	{
   		addNode(createNode(cProg->tokenList[getCw()],INSTRUC));
		addParseNode(getKeywordFromStack(MOVE_COMMAND),strtol(getKeywordFromStack(VARIABLE),NULL,10)); //!sends move command to interpreter
		return 1;
	} else if(checkIfVariable(cProg->tokenList[getCw()]))	{
		addNode(createNode(cProg->tokenList[getCw()],INSTRUC));   
		addParseNode(getKeywordFromStack(MOVE_COMMAND),getVariable(getFirstCharacter(getKeywordFromStack(VARIABLE)))); //!sends variable value to interpreter
		return 1;
	}	else {
		ERROR("Expected Number or Variable after instruction");	
	}
}


char *getKeywordFromStack(int keywordNum)	{

	int i;
	SyntaxStack cStack = getSynStack(NULL);
	SyntaxNode node  = cStack->start;
	for(i = 1; i <= keywordNum; i++)	{
		if(i == keywordNum)	{
			return node->type;
		}
		node = node->previous;
	}
	ERROR("Attempted to retrieve invalid keyword from stack");
}

int checkIfVariable(char *instruction)	{
	if(getTokenLength(instruction) == 2 && isupper(instruction[0]))	{
		return 1;
	}
	return 0;
}

/*
 *Removes current instruction from Syntax stack
 */
void removeCurrentInstruction()	{
	while(getSynStack(NULL)->instructionLength)	{
		popFromStack();	
	}
}

int checkIfNumber(char *instruction)	{
	if(strtol(instruction,NULL,10) || compCurrCw(ZERO))	{
		return 1;
	}
		return 0;
}

int checkIfMovement(char *type)	{
	if(!strcmp(type,FORWARD) || !strcmp(type,R_TURN) || !strcmp(type,L_TURN))	{
		return 1;
	}
	return 0;
}

int checkIfOp(char *type)	{
	if(!strcmp(type,ADD) || !strcmp(type,SUB) || !strcmp(type,MULT) || !strcmp(type,DIV))	{
		return 1;
	}
	return 0;
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

void printCurrentWord()	{
	printf("cw %d is %s\n",getCw(),getProgram(NULL)->tokenList[getCw()]);
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
