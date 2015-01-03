#include "grammar.h"
#include "testLib.h"

/*----------Enumerated Types-----------*/


/*----------TypeDefs-----------*/

typedef struct program *Program;
typedef struct syntaxStack *SyntaxStack;
typedef struct syntaxNode *SyntaxNode;
typedef struct calcStack *CalcStack;
typedef struct calcNode *CalcNode;

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

/*----------Function Prototypes-----------*/

void createProgram();
Program getProgram(Program cProg);
void addToken(char *token);
char** increaseStringList(char **stringList, int current);
void printTokenList();
int getTotalTokens();

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
void addNode(SyntaxNode Node);
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

//!Checks for each type of token
int valueCheck();
int movementParse(char *instruction);
int doParse(char *instruction);
void setParse();
void polishParse();

int checkIfNumber();
int checkIfVariable(char *instruction);

int code();
void prog();
void statement();
void printCurrentWord();

//! Interpretor functions
void calculatePolish();
int calcStackEmpty();
/*----------Testing Function Prototypes-----------*/

void parserUnitTests();
void setParse(char *instruction);
