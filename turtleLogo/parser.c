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
	double value;
	char *op;	
};

struct varTable	{

	VarNode *vTable;	//! Stores variables
	int vNum; 	//!Tracks number of variables
};

struct varNode	{

	char var;
	double value;

};


/*---------- Functions ----------*/

void createVarTable()	{

	VarTable vt = (VarTable) checkMalloc(malloc(sizeof(*vt)));
	vt->vTable = NULL;
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

/*
 *Adds a new variable to the var table or updates an already existing one
 */
char addVariable(char var, double val)	{

	VarTable vt = getVarTable(NULL);
	if(checkVarUnique(var))	{
		if(vt->vNum < MAXVAR)	{
			if(vt->vTable == NULL)	{
				vt->vTable = (VarNode*) checkMalloc(malloc(sizeof(*vt->vTable)));
			} else	{
				vt->vTable = (VarNode*) checkMalloc(realloc(vt->vTable,(vt->vNum+1)*sizeof(*vt->vTable[vt->vNum])));	
			}
			vt->vTable[vt->vNum] = (VarNode) checkMalloc(malloc(sizeof(*vt->vTable[vt->vNum])));
			vt->vTable[vt->vNum]->var = var;
			vt->vTable[vt->vNum]->value = val;
			vt->vNum++;
		} else	{
			ERROR("Out of Variable memory: Only 26 variables allowed");
		}
	} else { 
		updateVariable(var,val);
	}
	return var;
}
/*
 *Returns Variable Value
 */
double getVariable(char var)	{

	VarTable vt = getVarTable(NULL);
	int i;
	for(i = 0; i < vt->vNum; i++)	{
		if(vt->vTable[i]->var == var)	{
			return vt->vTable[i]->value;
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
		if(vt->vTable[i]->var == var)	{
			return var;
		}
	}
	ERROR("Undefined Symbol");

}

int checkVarUnique(char var)	{
	int i;
	VarTable vt = getVarTable(NULL);
	for(i = 0; i < vt->vNum; i++)	{
		if(vt->vTable[i]->var == var)	{
			return 0;
		}
	}
	return 1;
}

double* getVarAddress(char var)	{
	int i;
	VarTable vt = getVarTable(NULL);

	for(i = 0; i < vt->vNum; i++)	{
		if(vt->vTable[i]->var == var)	{
			return &vt->vTable[i]->value;
		}
	}
	ERROR("Undefined Symbol");
}

double updateVariable(char var,double val)	{
	
	int i;
	VarTable vt = getVarTable(NULL);
	for(i = 0; i < vt->vNum; i++)	{
		if(vt->vTable[i]->var == var)	{
			vt->vTable[i]->value = val;
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

void clearCalcStack()	{

	CalcStack cStack = getCalcStack(NULL);
	CalcNode temp = NULL;

	while(!calcStackEmpty())	{
		temp = cStack->start;
		cStack->start = cStack->start->previous;
		free(temp);
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

CalcNode createCalcValNode(double value)	{
	CalcNode newCNode = createCalcNode();
	newCNode->value = value;
	newCNode->type = NUM;
	return newCNode;
}

void addCalcNode(CalcNode newNode)	{
	assert(newNode != NULL);
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

int getNumberSynNodes()	{
	return getSynStack(NULL)->numNodes;
}

void removeNode(SyntaxNode node)	{
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
	cStack->numNodes++;
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
	if(compCurrCw(P_EQUALS))	{
		addNode(createNode(getCToken(),VAR));
		setCw(getCw()+1);
	}	else	{
		ERROR("Expected := in SET statement");
	}

	polishParse(getCToken());
	addVariable(getFirstCharacter(getKeywordFromStack(3)),(double) strtod(getKeywordFromStack(1),NULL));
}

void polishParse()	{
	char calcRes[MAXCALC];
	CalcNode final = NULL;
	while(!compCurrCw(END_POLISH))	{
		if(checkIfOp(getCToken()))	{
			addCalcNode(createCalcOpNode(getCToken()));	
			calculatePolish();
		} else if(checkIfVariable(getCToken()))	{
			addCalcNode(createCalcValNode(getVariable(getFirstCharacter(getCToken()))));
		} else if(checkIfNumber(getCToken()))	{
			addCalcNode(createCalcValNode(strtod(getCToken(),NULL)));
		} else {
			ERROR("Expected OP or VARNUM in statement");
		}
		setCw(getCw()+1);
	}
	final = popCalcStack();	
	sprintf(calcRes,"%f",final->value);
	free(final);
	if(!calcStackEmpty())	{
		ERROR("Error in POLISH statement: number of VAR and number of OP do not match");
	}
	addNode(createNode(calcRes,VAR));
	//printStack(); //!prints the current calc
}

double calculatePolish()	{
		CalcNode opNode = popCalcStack();
		CalcNode valNode_2 = popCalcStack();
		CalcNode valNode_1 = popCalcStack();	
		double result = 0;
		if(!strcmp(opNode->op,ADD))	{
			addCalcNode(createCalcValNode((result = valNode_1->value+valNode_2->value)));
		} else if(!strcmp(opNode->op,SUB))   {
			addCalcNode(createCalcValNode((result = valNode_1->value-valNode_2->value)));
		} else if(!strcmp(opNode->op,MULT))   {
			addCalcNode(createCalcValNode((result = valNode_1->value*valNode_2->value)));
		} else if(!strcmp(opNode->op,DIV))   {
			addCalcNode(createCalcValNode((result = valNode_1->value/valNode_2->value)));
		} else {
			fprintf(stderr,"Unrecognised operator in polish statement\n");
		}
		free(opNode);
		free(valNode_2);
		free(valNode_1);
		return result;
}

int doParse(char *instruction)	{
	Program cProg = getProgram(NULL);
	addNode(createNode(instruction,INSTRUC));
	setCw(getCw()+1);
	char minVal, maxVal;
	double *pMaxVal;
	double mNumVal, beforeLoopPos;
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
		updateVariable(minVal, strtod(addNode(createNode(getCToken(),NUM)),NULL));
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
		mNumVal = strtod(addNode(createNode(getCToken(),NUM)),NULL);
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
	for(;getVariable(minVal) <= *pMaxVal; updateVariable(minVal,getVariable(minVal)+1))	{
		setCw(beforeLoopPos);
		while(!compCurrCw(L_BRACE))	{
			statement();
		//	printStack();
			setCw(getCw()+1);
		}
	}
	removeLastSNodeOfType(R_BRACE);
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
		addParseNode(getKeywordFromStack(MOVE_COMMAND),strtod(getKeywordFromStack(VARIABLE),NULL)); //!sends move command to interpreter
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

/*
 *Checks if string is an integer or floating point number
 */
int checkIfNumber(char *instruction)	{
	int i;
	for(i = 0;instruction[i] != '\0'; i ++)	{
		if(instruction[i] != '.')	{
			if(isdigit(instruction[i]))	{
			/* Do Nothing*/	
			} else {
				return 0;	
			}
		}
	}
	return 1;
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
	if(cProg->tokenNum > 0)	{
		if(!strcmp(comparison,cProg->tokenList[getCw()]))	{
			return 1;
		}
	}
	return 0;
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

void clearTokens()	{
	Program cProg = getProgram(NULL);
	int i;
	for (i = cProg->tokenNum - 1; i > 0; i--)	{
		free(cProg->tokenList[i]);
		cProg->tokenList[i] = NULL;
		cProg->tokenList = (char**) checkMalloc(realloc(cProg->tokenList,cProg->tokenNum - 1 * sizeof(*cProg->tokenList[i])));
	}
    cProg->tokenList = NULL;
    cProg->tokenNum = 0;
    cProg->cw = 0;
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
	varTableTests();
	polishCalcTests();
	programArrayTests();
	syntaxStackTests();
	parsingTests();
}

void varTableTests()	{
    enterSuite("Variable Table Tests");
    testVal(addVariable('A',20),'A',"Valid: adding new variable to var table",EQUALS);
    testVal(getVariable('A'),20,"Valid: check variable value stored as 20",EQUALS);
    testVal(addVariable('B',40),'B',"Valid: adding new variable to var table",EQUALS);
    testVal(getVariable('B'),40,"Valid: check variable value stored as 40",EQUALS);
    testVal(checkVarUnique('C'),1,"Valid: C has not yet been entered as a variable",EQUALS);
    testVal(checkVarDeclared('B'),'B',"Valid: B has been entered as a variable",EQUALS);
    testVal(updateVariable('B',30),1,"Valid: Updating B's value in variable table",EQUALS);
    testVal(getVariable('B'),30,"Valid: check B's updated variable value is stored as 30",EQUALS);
    leaveSuite();
}

void polishCalcTests()	{
    enterSuite("Polish Calculator Tests");
    addCalcNode(createCalcValNode(1.5));
    addCalcNode(createCalcValNode(1));
    addCalcNode(createCalcOpNode("+"));
    testVal(calculatePolish(),2.5,"Valid: 1 + 1 = 2",EQUALS);
    addCalcNode(createCalcValNode(1));
    addCalcNode(createCalcOpNode("-"));
    testVal(calculatePolish(),1.5,"Valid: 2.5 - 1 = 1.5",EQUALS);
    addCalcNode(createCalcValNode(2));
    addCalcNode(createCalcOpNode("*"));
    testVal(calculatePolish(),3,"Valid: 1.5 * 2 = 3",EQUALS);
    addCalcNode(createCalcValNode(3));
    addCalcNode(createCalcOpNode("/"));
    testVal(calculatePolish(),1,"Valid: 3 / 3 = 1",EQUALS);
	testVal(calcStackEmpty(),0,"Valid: Test Stack not Empty",EQUALS);
	clearCalcStack();
	testVal(calcStackEmpty(),1,"Valid: Test Stack Empty",EQUALS);
    leaveSuite();
}

void programArrayTests()	{

    enterSuite("Program Token Tests");
	Program cProg = getProgram(NULL);
	addToken(FORWARD);
	testVal(getTotalTokens(),1,"Valid: Number of Tokens is 1",EQUALS);
	testVal(compCurrCw(FORWARD),1,"Valid: Current Token is FD",EQUALS);
	testVal(checkIfMovement(cProg->tokenList[getCw()]),1,"Valid: FD is movement keyword",EQUALS);
	testVal(checkIfMovement(R_TURN),1,"Valid: RT is a movement keyword",EQUALS);
	testVal(checkIfMovement(L_TURN),1,"Valid: LT is a movement keyword",EQUALS);
	testVal(checkIfMovement("AD"),0,"invalid: not a movement keyword",EQUALS);
	testVal(checkIfMovement("1"),0,"invalid: not a movement keyword",EQUALS);
	testVal(checkIfMovement("TESTING"),0,"invalid: not a movement keyword",EQUALS);
	testVal(checkIfMovement("RD"),0,"invalid: not a movement keyword",EQUALS);
	addToken("10");
	testVal(getTotalTokens(),2,"Valid: Number of Tokens is 2",EQUALS);
	setCw(getCw()+1);
	testVal(compCurrCw("10"),1,"Valid: Moved to the next token to parse",EQUALS);
	clearTokens();
	testVal(getTotalTokens(),0,"Valid: Cleared all tokens",EQUALS);
    leaveSuite();
}

void syntaxStackTests()	{
	bracketsTest();
	clearTokens();
}

void bracketsTest()	{
    enterSuite("Popping Brackets from Syntax stack Tests");
	addNode(createNode(R_BRACE,MAIN));
	testVal(strcmp(R_BRACE,getKeywordFromStack(1)),0,"Valid: Top of Syn stack is {",EQUALS);
	removeLastSNodeOfType(R_BRACE);
	testVal(checkSynStackEmpty(),1,"Valid: Removed last {.  Empty Synax Stack",EQUALS);
	testVal(getNumberSynNodes(),0,"Valid: Number of syntax nodes is 0",EQUALS);
    leaveSuite();
}

void parsingTests()	{
    enterSuite("General Parsing Function Tests");
	testVal(checkIfNumber("0"),1,"Valid: 0 is a number",EQUALS);
	testVal(checkIfNumber("1.2"),1,"Valid: 1.2 is a number",EQUALS);
	testVal(checkIfNumber("10"),1,"Valid: 10 is a number",EQUALS);
	testVal(checkIfNumber("12shd"),0,"Invalid: not a number",EQUALS);
	testVal(checkIfNumber("DO"),0,"Invalid: not a number",EQUALS);
	testVal(checkIfNumber("egehd"),0,"Invalid: not a number",EQUALS);
	testVal(checkIfVariable("A"),1,"Valid: A is valid variable name",EQUALS);
	testVal(checkIfVariable("AE"),0,"Invalid: Variables names can only be one letter long",EQUALS);
	testVal(checkIfVariable("1"),0,"Invalid:  Variable names cannot be numbers",EQUALS);
	testVal(checkIfVariable("a"),0,"Invalid:  Variable names must be uppercase",EQUALS);
	testVal(checkIfOp("*"),1,"Valid: * is multiplication operator",EQUALS);	
	testVal(checkIfOp("+"),1,"Valid: + is addition operator",EQUALS);	
	testVal(checkIfOp("-"),1,"Valid: - is subtraction operator",EQUALS);	
	testVal(checkIfOp("/"),1,"Valid: / is division operator",EQUALS);	
	testVal(checkIfOp("1"),0,"Invalid: not an operator",EQUALS);	
	testVal(checkIfOp("dksjhas"),0,"Invalid: not an operator",EQUALS);	
	testVal(checkIfOp("&"),0,"Invalid: not an operator",EQUALS);	
    leaveSuite();
}
