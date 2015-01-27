//////
//
//Parser_interpreter.c
//This module provides the parsing and interpreting functionality for the program.  Tokens are
//read from the program structure, and are then parsed and interpreted.
//
///////////////////

/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "parser_interpreter.h"

/*---------- Structures -----------*/

/**!
 *Holds list of read in program tokens to be parsed and interpreted
 */
struct program	{

		char **tokenList; //@ List of tokens
		int tokenNum; //@Number of tokens
		int cw;	//@Current token to be parsed
		//int nw;
};

/**!
 *Head of syntax stack.  Not true stack: Doubly linked and brackets will be removed from anywhere.  Every other type of command gets treated like stack
 */
struct syntaxStack	{

		SyntaxNode start; //@Start of stack
		SyntaxNode end;	//@ End of stack
		int numNodes;	//@Number of nodes in stack
		int instructionLength;	//@Length of current instruction, ignoring brackets
		ModeNode modeStackStart;	//@Stack holding execution nodes
};

/**!
 *Makes up stack controlling current execution mode of program
 */
struct modeNode	{

	iMode pM; //! Value holding execution mode
	ModeNode previous; //! Previous node in stack

};

/**!
 *Nodes making up syntax list
 */
struct syntaxNode	{

	char *type; //@ String representation of current instruction
	synType sType; //@Syntax type of instruction
	SyntaxNode previous;	//@ Previous node in list
	SyntaxNode next;	//@ next node in list
};

/**!
 *Head of polish calculator stack
 */
struct calcStack	{
	CalcNode start; //@Start of stack
	int numNodes; //@Number of held nodes
};

/**!
 *Nodes making up polish calculator stack
 */
struct calcNode	{

	CalcNode previous;	//@Previous node in stack
	synType type;	//@Syntax type of node
	double value;	//@Value of node
	char *op;		//@Operator value of node
};

/**!
 *Variable table
 */
struct varTable	{

	VarNode *vTable;	//@ Stores variables
	int vNum; 	//@ Tracks number of variables
};

/**!
 *Node making up variable table
 */
struct varNode	{

	char var;	//@ Variable name
	double value;	//@ Variable value

};


/*---------- Functions ----------*/

/*
 *Creates variable table struecture
 */
void createVarTable()	{

	VarTable vt = (VarTable) checkMalloc(malloc(sizeof(*vt)));
	vt->vTable = NULL;
	vt->vNum = 0;
	getVarTable(vt);
}

/*
 *Returns pointer to variable table
 */
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
	if(!getMode())	{
		return 1;
	}
	ERROR("Undefined Symbol");
}

/*
 *Checks if a variable is present in var table
 */
char checkVarDeclared(char var)	{
	int i;
	VarTable vt = getVarTable(NULL);

	for(i = 0; i < vt->vNum; i++)	{
		if(vt->vTable[i]->var == var)	{
			return var;
		}
	}
	ERROR("Undefined Symbol");
}

/*
 *Returns true if variable is unqiue
 */
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

/*
 *Returns variable address
 */
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

/*
 *Updates specified variable with specified value
 */
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

/*
 *Prints polish stack.  Debug.
 */
void printCalcStack()	{
	CalcStack cStack = getCalcStack(NULL);
	CalcNode temp = cStack->start;
	if(temp != NULL)	{
		while(temp->previous != NULL)	{
			if(temp->type == OP )	{
				sprint(temp->op);
			} else if(temp->type == NUM)	{
				dprint(temp->value);
			}
			temp = temp->previous;
		}
	}
}

/*
 *Removes node from top of polish calculator stack and returns value
 */
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

/*
 * Returns true if polish calculator stack is empty
 */
int calcStackEmpty()	{

	if(getCalcStack(NULL)->start == NULL)	{
		return 1;
	}	else {
		return 0;
	}
}

/*
 *Clears polish calculator stack
 */
void clearCalcStack()	{

	CalcStack cStack = getCalcStack(NULL);
	CalcNode temp = NULL;

	while(!calcStackEmpty())	{
		temp = cStack->start;
		cStack->start = cStack->start->previous;
		free(temp);
	}
}

/*
 *Clears and frees polish calculator stack
 */
void freeCalcStack()	{
	clearCalcStack();
	free(getCalcStack(NULL));
}

/*
 *Creates polish calculator stack data structure
 */
void createCalcStack()	{
	CalcStack newCStack = (CalcStack) checkMalloc(malloc(sizeof(*newCStack)));
	newCStack->start = NULL;
	newCStack->numNodes = 0;
	getCalcStack(newCStack);
}

/*
 *Creates polish calculator node
 */
CalcNode createCalcNode()	{
	CalcNode newCNode = (CalcNode) checkMalloc(malloc(sizeof(*newCNode)));
	newCNode->previous = NULL;
	return newCNode;
}

/*
 *initializes polish calculator operator node
 */
CalcNode createCalcOpNode(char *op)	{

	CalcNode newCNode = createCalcNode();
	newCNode->op = (char*) checkMalloc(calloc(getTokenLength(op),sizeof(char)));
	strcat(newCNode->op,op);
	newCNode->type = OP;
	return newCNode;
}

/*
 *initializes polish calculator value node
 */
CalcNode createCalcValNode(double value)	{
	CalcNode newCNode = createCalcNode();
	newCNode->value = value;
	newCNode->type = NUM;
	return newCNode;
}

/*
 *Adds node to top of polish calculator stack
 */
void addCalcNode(CalcNode newNode)	{
	assert(newNode != NULL);
	CalcStack cStack = getCalcStack(NULL);
	newNode->previous = cStack->start;
	cStack->start = newNode;

}

/*
 *Returns pointer to polish calculator stack head
 */
CalcStack getCalcStack(CalcStack cStack)	{

	static CalcStack currCalcStack;
	if(cStack != NULL)	{
		currCalcStack = cStack;
	}
	return currCalcStack;
}

/*
 *Creates syntax stack structure
 */
void createSynStack()	{

	SyntaxStack newStack = (SyntaxStack) checkMalloc(malloc(sizeof(*newStack)));
	newStack->start = newStack->end = NULL;
	newStack->numNodes = 0;
	newStack->instructionLength = 0;
	newStack->modeStackStart = checkMalloc(malloc(sizeof(*newStack->start)));
	newStack->modeStackStart->pM = exec;
	newStack->modeStackStart->previous = NULL;
	getSynStack(newStack);
}

/*
 *Pushes node to execution mode stack
 */
void pushMode(iMode nm)	{

	SyntaxStack cStack = getSynStack(NULL);
	ModeNode temp;
	assert(cStack->modeStackStart != NULL);
	temp = cStack->modeStackStart;
	cStack->modeStackStart = checkMalloc(malloc(sizeof(*cStack->start)));
	cStack->modeStackStart->pM = nm;
	cStack->modeStackStart->previous = temp;	
}

/*
 *Removes node and returns value from execution mode stack
 */
iMode popMode()	{

    SyntaxStack cStack = getSynStack(NULL);
    ModeNode temp;
	iMode tMode;
    assert(cStack->modeStackStart != NULL);
    temp = cStack->modeStackStart;
	cStack->modeStackStart = cStack->modeStackStart->previous;
	tMode = temp->pM;
	free(temp);
	return tMode;
}

/*
 *Returns pointer to syntax stack
 */
SyntaxStack getSynStack(SyntaxStack nStack)	{

	static SyntaxStack cStack;
	if(nStack != NULL)	{
		cStack = nStack;	
	}

	return cStack;
}

/*
 *Creates Node for syntax stack that holds parse symbols for instructions
 */
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
	ERROR("Tried to remove symbol that does not exist.  Likely brackets mis-match.");
	return 0;
}

/*
 *Returns number of syntax nodes on stack
 */
int getNumberSynNodes()	{
	return getSynStack(NULL)->numNodes;
}

/*
 *Removes syntax node from stack
 */
void removeNode(SyntaxNode node)	{
	SyntaxStack cStack = getSynStack(NULL);

	assert(cStack->start != NULL);

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

/*
 *Removes all types of node except Right braces from top of stack
 */
void popFromStack()	{
	SyntaxStack cStack = getSynStack(NULL);
	SyntaxNode Node	 = cStack->start;
	
	assert(cStack->start != NULL);

	while(!strcmp(Node->type,R_BRACE))	{
		Node = Node->previous;
	}
		removeNode(Node);
}

/*
 *Adds syntax node to top of stack
 */
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

/*
 *Creates program array data structure
 */
void createProgram()	{

	Program newProgram = (Program) checkMalloc(malloc(sizeof(*newProgram)));
	newProgram->tokenList = NULL;
	newProgram->tokenNum = 0;
	newProgram->cw = 0;
	getProgram(newProgram);

}

/*
 * Entry point to parsing process.  Checks for opening brace.
 */
void prog()	{
	setCw(0);
	specParse("Missing { to start",R_BRACE,MAIN);
	code();	
}

/*
 *Recursively calls itelf to parse program list statement by statement
 */
int code()	{
	//!Base case
	if(checkSynStackEmpty() && !checkNoMoreWords())	{
		ERROR("No more input expected");
	} else if(!checkSynStackEmpty() && checkNoMoreWords()) {
		ERROR("Closing Brace missed");
	} else if(checkSynStackEmpty() && checkNoMoreWords()) {
		printf("No errors\n");
		return 1;
	}
	statement();
	removeCurrentInstruction();	//!Removes last parsed instruction before continuing parsing
	code();
	return 1;
}

/*
 *Entry point for all statement parsing.  If start of statement is not matched here it is invalid.
 */
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
	} else if(compCurrCw(IF))	{
		ifParse(IF);	
	} else	{
		iprint(getCw());
		printf("%s\n",getCToken());
		ERROR("Unrecognisd Symbol");
	}
	setCw(getCw()+1);
}

/*
 *Parses and interprets set command
 */
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
	specParse("Expected := in SET statement",P_EQUALS,VAR);

	polishParse(getCToken());
	if(getMode())	{ //! Only add variable if in exec mode
		addVariable(getFirstCharacter(getKeywordFromStack(3)),(double) strtod(getKeywordFromStack(1),NULL));
	}
}

/*
 *Parses and interprets polish expresions
 */
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
	if(checkIfNumber(calcRes))	{
		addNode(createNode(calcRes,VAR));
	}	else	{
		ERROR("Calculation not possible: please check");
	}
}

/*
 *Calculates results of polish expressions to be placed back on calc stack
 */
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
			ERROR("Unrecognised operator in polish statement");
		}
		free(opNode);
		free(valNode_2);
		free(valNode_1);
		return result;
}

/*
 *Parses and interprets base if statement
 */
void ifParse(char *instruction)	{

	pushMode(getMode());	//! Copying previous execution mode
	addNode(createNode(instruction,INSTRUC));
	setCw(getCw()+1);
    double *pValB;
    double *pValA;
    double valANum, valBNum;
	int ifResult;
	iMode mTemp  = getMode();
	ifComp cmpOp;
	
	//!First VARNUM Presence check
	if(checkIfVariable(getCToken()))	{
		pValA = getVarAddress(checkVarDeclared(getFirstCharacter(addNode(createNode(getCToken(),VAR)))));
	} else if(checkIfNumber(getCToken()))	{
		valANum = strtod(addNode(createNode(getCToken(),NUM)),NULL);
		pValA = &valANum;	
	} else	{
		sprint(getCToken());
		ERROR("Expected VARNUM in IF statement");
	}

	//!Operator Check
	setCw(getCw()+1);
	if((cmpOp = checkIfComparator(getCToken())))	{
		addNode(createNode(getCToken(),OP));
	}
	setCw(getCw()+1);

	//!Second Varnum Presence check
	if(checkIfVariable(getCToken()))	{
		pValB = getVarAddress(checkVarDeclared(getFirstCharacter(addNode(createNode(getCToken(),VAR)))));
	} else if(checkIfNumber(getCToken()))	{
		valBNum = strtod(addNode(createNode(getCToken(),NUM)),NULL);
		pValB = &valBNum;
	} else	{
		ERROR("Expected VARNUM in IF statement");
	}

	setCw(getCw()+1);
	specParse("Expected { in IF statement",R_BRACE,MAIN);
	if((ifResult = ifComparison(cmpOp,*pValA,*pValB)) && (mTemp == exec))	{
		while(!compCurrCw(L_BRACE))	{
			statement(); //!Do statements in if loop if condition is true and mode is execution
		}
	} else	{
		ifBlock(mTemp);
	}

	lBraceParse("IF block not closed");
	setCw(getCw()+1);





	ifDecision(ifResult);
	popMode();
}

/*
 * Check to see if there is another if block to parse
 */
void ifDecision(int ifResult)	{
    if(compCurrCw(ELIF))    {
        if(!ifResult && getMode())  {
            ifParse(IF);
        } else  {
            skipElif();
        }
    } else if(compCurrCw(ELSE)) {
        if(!ifResult && getMode())  {
            elseParse();
        } else  {
            skipElse();
        }
    } else  {
        setCw(getCw()-1);
    }	

}

/*
 *Parse and interpret else block
 */
void elseParse()	{
	if(compCurrCw(ELSE)) {
		addNode(createNode(ELSE,INSTRUC));
	} else	{
		ERROR("Trying to parse statement like ELSE");
	}
	setCw(getCw()+1);
	specParse("Expected { in IF statement",R_BRACE,MAIN);
	while(!compCurrCw(L_BRACE))	{
		statement();
		//setCw(getCw()+1);
	}

	if(compCurrCw(L_BRACE)) {
        removeLastSNodeOfType(R_BRACE);
    } else  {
        ERROR("IF block not closed");
    }

	setCw(getCw()+1);
	if(compCurrCw(ELIF))	{
		ERROR("Cannot have ELSE inside ELIF block");
	}
	setCw(getCw()-1);
}

/*
 *Parse ELIF in skip mode
 */
void skipElif()	{
	iMode mTemp = getMode(); //!Saving current execution mode
	if(compCurrCw(ELIF))	{
		setCw(getCw()+1);
		if(checkIfVariable(getCToken()))	{
			checkVarDeclared(getFirstCharacter(addNode(createNode(getCToken(),VAR))));	
		} else if(checkIfNumber(getCToken()))	{
			addNode(createNode(getCToken(),NUM));	
		} else	{
			ERROR("Expected VARNUM in IF statement");
		}

		setCw(getCw()+1);
		if((checkIfComparator(getCToken())))	{
			addNode(createNode(getCToken(),OP));
		}
		setCw(getCw()+1);
		if(checkIfVariable(getCToken()))	{
			checkVarDeclared(getFirstCharacter(addNode(createNode(getCToken(),VAR))));
		} else if(checkIfNumber(getCToken()))	{
			addNode(createNode(getCToken(),NUM));
		} else	{
			ERROR("Expected VARNUM in IF statement");
		}

		setCw(getCw()+1);
		specParse("Expected { in IF statement",R_BRACE,MAIN);
		ifBlock(mTemp);

		lBraceParse("IF block not closed");
	
		setCw(getCw()+1); //!Peaking at next token to see if it is part of else
		if(compCurrCw(ELIF))	{
			skipElif();
		} else if (compCurrCw(ELSE))	{
			skipElse();
		} else  {
			setCw(getCw()-1);
		}
	} else	{
		ERROR("Skipping elif erroneously");
	}
}

void ifBlock(iMode mT)	{
	while(!compCurrCw(L_BRACE))	{
		setMode(skip);
		//if(!checkForEmptyBlock())	{
       	statement();
	//	}
        if(mT)   {
			setMode(exec);
		}
	} /*while(!compCurrCw(L_BRACE));*/
}

/*
 *Returns true if empty block
 */
int checkForEmptyBlock()    {
	sprint(getCToken());
	if(compCurrCw(L_BRACE))	{
		return 1;
	}
	setCw(getCw()-1);
	return 0;
}

/*
 *Skip else statement
 */
void skipElse()	{
	iMode mTemp = getMode();
	specParse("Skipping ELSE on non-ELSE statement",ELSE,INSTRUC);
	specParse("Expected { in ELSE statement",R_BRACE,MAIN);
	ifBlock(mTemp);
	lBraceParse("Failed to close ELSE statement");
}


/*
 *Performs specified IF statement comparison
 */
int ifComparison(ifComp op, double valA, double valB)	{

	switch(op)	{
		case GR:
			if(valA>valB)	{
				return 1;
			}
			break;
		case LT:
			if(valA<valB)	{
				return 1;
			}
			break;
		case GR_E:
			if(valA>=valB)	{
				return 1;
			}
			break;
		case LT_E:
			if(valA<=valB)	{
				return 1;
			}
			break;
		case EQU:
			if(valA == valB)	{
				return 1;
			}
			break;
		case N_EQU:
			if(valA != valB)	{
				return 1;
			}
			break;
		default:
			ERROR("Invalid comparison operator");
			break;
	}
	return 0;

}

/*
 *Converts comparator token to enumerated type
 */
ifComp checkIfComparator(char *comparator)	{
	assert(comparator != NULL);
	if(!strcmp(comparator,G_THAN))	{
		return GR;	
	} else if(!strcmp(comparator,L_THAN)) {
		return LT;
	} else if(!strcmp(comparator,G_THAN_E)) {
		return GR_E;
	} else if(!strcmp(comparator,L_THAN_E)) {
		return LT_E;
	} else if(!strcmp(comparator,EQ)) {
		return EQU;
	} else if(!strcmp(comparator,N_EQ))	{
		return N_EQU;
	} else	{
		ERROR("Not valid comparison operator");
	}
}

/*
 *Parses and interprets do statement
 */
int doParse(char *instruction)	{
	Program cProg = getProgram(NULL);
	addNode(createNode(instruction,INSTRUC));
	setCw(getCw()+1);
	char minVal, maxVal;
	double *pMaxVal = NULL;
	double mNumVal, beforeLoopPos;
	if(checkIfVariable(cProg->tokenList[getCw()])){
		addNode(createNode(getCToken(),VAR));
		minVal = getFirstCharacter(getCToken());	
		if(getMode())	{
			if(checkVarUnique(getFirstCharacter(getCToken())))	{
				addVariable(getFirstCharacter(getCToken()),NEW_VAR);
			} 
		}
		setCw(getCw()+1);
	} else {
		ERROR("Expected VAR in DO statement");
	}

	specParse("Expected FROM in DO statement",FROM,CONNECTOR);

	if(checkIfVariable(getCToken()))	{
		addNode(createNode(getCToken(),VAR));
		if(getMode())	{
			updateVariable(minVal, getVariable(getFirstCharacter(getCToken())));
		}
	} else if(checkIfNumber(getCToken()))	{	
		if(getMode())	{
			updateVariable(minVal, strtod(addNode(createNode(getCToken(),NUM)),NULL));
		}
	} else {
		ERROR("Expected VARNUM in DO statement");
	}
	setCw(getCw()+1);

	specParse("Expected TO in DO statement",TO,CONNECTOR);
	
	if(checkIfVariable(getCToken()))	{
		if(getMode())	{
			maxVal = checkVarDeclared(getFirstCharacter(addNode(createNode(getCToken(),VAR))));
			pMaxVal = getVarAddress(maxVal);
			if(*pMaxVal != getVariable(maxVal))	{
				ERROR("Memory Error: Local value variable does not match var table");
			}
		}
	} else if(checkIfNumber(getCToken()))	{
		if(getMode())	{
			mNumVal = strtod(addNode(createNode(getCToken(),NUM)),NULL);
			pMaxVal = &mNumVal;
		}
	} else {
		ERROR("Expected VARNUM in DO statement");
	}

	setCw(getCw()+1);
	specParse("Expected { in DO statement",R_BRACE,MAIN);
	beforeLoopPos = getCw();
	if(getMode())	{
		for(;getVariable(minVal) <= *pMaxVal; updateVariable(minVal,getVariable(minVal)+1))	{
			setCw(beforeLoopPos);
			while(!compCurrCw(L_BRACE))	{
				statement();
			}
		}
	} else {
		while(!compCurrCw(L_BRACE))	{
			statement();
		}
	}
	lBraceParse("WHILE block not closed");
	return 1;
}

/*
 *Performs specified comparison and increments token counter
 */
int specParse(char *error,char *comparison, synType t )	{
	if(compCurrCw(comparison))  {
        addNode(createNode(getCToken(),t));
        setCw(getCw()+1);
    } else {
        ERROR(error);
    }
	return 1;
}

/*
 *	Closing left braces
 */
int lBraceParse(char *error)	{
	if(compCurrCw(L_BRACE))	{
		removeLastSNodeOfType(R_BRACE);
	} else	{
		ERROR(error);
	}
	return 1;
}

/*
 *	Returns stack node value from specified node from start of queue
 */
char* syntaxStackquery(int n)	{
    SyntaxStack cStack = getSynStack(NULL);
    SyntaxNode node  = cStack->start;
	assert(cStack->start != NULL);
	int i;
	for(i = 0; i < n; i++)	{
		if(node->previous != NULL)	{
			node = node->previous;
		}
	}
	return node->type;

}

/*
 * Prints current syntax stack.  Debug.
 */
void printStack()	{
	SyntaxStack cStack = getSynStack(NULL);
	SyntaxNode Node  = cStack->start;

	while(Node != NULL)	{
		printf("%s\n",Node->type);
		Node = Node->previous;
	}
}

/*
 *Returns string of current program array 
 */
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
		if(TESTING && getMode())	{
			addParseNode(getKeywordFromStack(MOVE_COMMAND),strtod(getKeywordFromStack(VARIABLE),NULL));
		}
		if(getMode())	{
			moveInterpret(getKeywordFromStack(MOVE_COMMAND),strtod(getKeywordFromStack(VARIABLE),NULL));
		}
		return 1;
	} else if(checkIfVariable(cProg->tokenList[getCw()]))	{
		addNode(createNode(cProg->tokenList[getCw()],INSTRUC));   
		if(TESTING)	{
		addParseNode(getKeywordFromStack(MOVE_COMMAND),getVariable(getFirstCharacter(getKeywordFromStack(VARIABLE))));
		}
		if(getMode())	{
			moveInterpret(getKeywordFromStack(MOVE_COMMAND),getVariable(getFirstCharacter(getKeywordFromStack(VARIABLE))));
		}
		return 1;
	}	else {
		ERROR("Expected Number or Variable after instruction");	
	}
}

/*
 *Interprets parsed move instructions
 */
void moveInterpret(char *command, double value) {
    if(!strcmp(command,FORWARD))    {
        moveTurtleForward(value);
        if(!TESTING)    {
            drawLine(getOriginX(),getOriginY(),getTargetX(),getTargetY());
        }
    } else if(!strcmp(command,R_TURN))  {
        turnTurtleRight(value);
    } else if(!strcmp(command,L_TURN))  {
        turnTurtleLeft(value);
    }
}

/*
 *Returns current execution mode
 */
iMode getMode()	{
	return(getSynStack(NULL)->modeStackStart->pM);
}

/*
 *Chanes current execution mode
 */
void setMode(iMode nM)	{
	getSynStack(NULL)->modeStackStart->pM = nM;
}

/*
 *Return string representation of specified keyword from syntax stack
 */
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

/*
 *Checks if passed string matches syntax for variable name
 */
int checkIfVariable(char *instruction)	{
	if(getTokenLength(instruction) == 2 && isupper(instruction[0]) && instruction[1] == '\0')	{
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

/*
 *checks if passed string matches syntax for movement
 */
int checkIfMovement(char *type)	{
	assert(type!=NULL);
	if(!strcmp(type,FORWARD) || !strcmp(type,R_TURN) || !strcmp(type,L_TURN))	{
		return 1;
	}
	return 0;
}

/*
 *Check if passed in string matches syntax for operator
 */
int checkIfOp(char *type)	{
	assert(type!=NULL);
	if(!strcmp(type,ADD) || !strcmp(type,SUB) || !strcmp(type,MULT) || !strcmp(type,DIV))	{
		return 1;
	}
	return 0;
}

/*
 *Return total number of tokens
 */
int getTotalTokens()	{
	return getProgram(NULL)->tokenNum;
}

/*
 *Check if there are more tokens to parse
 */
int checkNoMoreWords()	{
	assert(getCw() <= getTotalTokens());
	if(getTotalTokens() == getCw())	{
		return 1;
	} else {
		return 0;
	}
}

/*
 *Check syntax stack is empty
 */
int checkSynStackEmpty()	{

	SyntaxStack cStack = getSynStack(NULL);
	if(cStack->start == NULL)	{
		return 1;
	}	

	return 0;
}

/*
 *Compares passed in parameter against current program symbol
 */
int compCurrCw(char *comparison)	{
	assert(comparison != NULL);
	Program cProg = getProgram(NULL);
	if(getCw() < getTotalTokens()){	
		if(cProg->tokenNum > 0)	{
			if(!strcmp(comparison,cProg->tokenList[getCw()]))	{
				return 1;
			}
		}
	} else	{
		ERROR("Closing Brace missed");	
	}
	return 0;
}

/*
 *Return index of current token
 */
int getCw()	{
	return getProgram(NULL)->cw;
}

/*
 *Set current CW indec to absolute passed in value
 */
void setCw(int newValue)	{
	getProgram(NULL)->cw = newValue;
}

/*
 *Return pointer to program token array
 */
Program getProgram(Program cProg)	{

	static Program prog;
	if(cProg != NULL)	{
		prog = cProg;
	}

	return prog;
}
/*
 *Add token to program list
 */
void addToken(char *token)	{
	assert(token != NULL);

	Program cProg = getProgram(NULL);
	cProg->tokenList = increaseStringList(cProg->tokenList,++cProg->tokenNum);
	cProg->tokenList[cProg->tokenNum - 1] = createCharBuffer(cProg->tokenList[cProg->tokenNum - 1],getTokenLength(token));
	strcat(cProg->tokenList[cProg->tokenNum - 1],token);
}

/*
 *Clear program array of all tokens
 */
void clearTokens()	{
	Program cProg = getProgram(NULL);
	int i;
	for (i = 0; i < cProg->tokenNum; i++)	{
		free(cProg->tokenList[i]);
		cProg->tokenList[i] = NULL;
	}
	free(cProg->tokenList);
    cProg->tokenList = NULL;
    cProg->tokenNum = 0;
    cProg->cw = 0;
}

/*
 *Print token list.  Debug.
 */
void printTokenList()	{

	Program cProg = getProgram(NULL);
	int i;
	for(i = 0; i < cProg->tokenNum; i++)	{
		printf("%s\n",cProg->tokenList[i]);
	}
}

/*
 *Increase program token array to specified size.
 */
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
	testVal(compCurrCw("TEST"),0,"Invalid: Current Token is not TEST",EQUALS);
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
	addToken("10");
	testVal(compCurrCw("10"),1,"Valid: Able to add tokens again after clearing",EQUALS);
	clearTokens();
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
