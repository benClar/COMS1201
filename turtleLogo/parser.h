#include "grammar.h"
#include "testLib.h"
#include "interpreter.h"

/*----------Enumerated Types-----------*/


/*----------TypeDefs-----------*/

typedef struct program *Program;
typedef struct syntaxStack *SyntaxStack;
typedef struct syntaxNode *SyntaxNode;
typedef struct calcStack *CalcStack;
typedef struct calcNode *CalcNode;
typedef struct varTable *VarTable;
typedef enum synType    {

        MAIN= 1,
        INSTRUC = 2,
        OP = 3,
        VAR = 4,
        NUM = 5,
		CONNECTOR = 6

} synType;

/*----------Symbolic Constants-----------*/

#define MAXCALC		15		//! Maximum number of digits in result of polish calculator
#define MAXVAR		26		//! Maximum number of variables that can be declared
#define VAR_T_COLUMNS	2	//!Number of columns in variable table
#define V_NAME_COL	0		//!Column for storing variable name
#define V_VAL_COL	1		//!Column for storing variable value
#define VARIABLE	1
#define	MOVE_COMMAND	2
#define NEW_VAR		0		//!Initial value for undefined variable
/*----------Function Prototypes-----------*/

void createProgram();
Program getProgram(Program cProg);
void addToken(char *token);
char** increaseStringList(char **stringList, int current);
void printTokenList();
int getTotalTokens();

//!Variable functions
char checkVarDeclared(char var);
void createVarTable();
VarTable getVarTable(VarTable nVT);
char addVariable(char var, int val);
int getVariable(char var);
int checkVarUnique(char var);
int updateVariable(char var,int val);
int* getVarAddress(char var);

int getCw();
void setCw(int newValue);
int compCurrCw(char *comparison);
char* getCToken();

//!Calculation Stack Functions
void createCalcStack();
CalcNode createCalcNode();
CalcNode createCalcOpNode(char *op);
CalcNode createCalcValNode(int value);
void addCalcNode(CalcNode newNode);
CalcStack getCalcStack(CalcStack cStack);
CalcNode popCalcStack();

SyntaxNode createNode(char *type, synType sType);
void createSynStack();
SyntaxStack getSynStack(SyntaxStack nStack);
char* addNode(SyntaxNode Node);
int checkSynStackEmpty();
int removeLastSNodeOfType(char *type);
void removeNode(SyntaxNode node);
int checkNoMoreWords();
int checkForUnfinished();
SyntaxNode getFirstSyntaxNode();
int checkSType(SyntaxNode node, synType sType);
void popFromStack();
int compTopOfStack(char *comparison);
int checkIfMovement(char *type);
int checkIfOp(char *type);
void removeCurrentInstruction();
void printStack();
char *getKeywordFromStack(int keywordNum);
char* syntaxStackquery(int n);

//!Checks for each type of token
int valueCheck();
int movementParse(char *instruction);
int doParse(char *instruction);
void setParse();
void polishParse();

int checkIfNumber();
int checkIfVariable(char *instruction);
int checkWhileCondition(int current, int max);

int code();
void prog();
void statement();
void printCurrentWord();

//! Interpretor functions
void calculatePolish();
int calcStackEmpty();
/*----------Testing Function Prototypes-----------*/

void parserUnitTests();
