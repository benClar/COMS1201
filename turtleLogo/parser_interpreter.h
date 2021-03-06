#include "grammar.h"
#include "testLib.h"
#include "testInstructions.h"

/*----------Enumerated Types-----------*/
#define MAX_F_NAME	40

/*----------TypeDefs-----------*/

typedef struct program *Program;
typedef struct syntaxStack *SyntaxStack;
typedef struct syntaxNode *SyntaxNode;
typedef struct calcStack *CalcStack;
typedef struct calcNode *CalcNode;
typedef struct varTable *VarTable;
typedef struct varNode *VarNode;
typedef struct modeNode *ModeNode;
typedef struct addressStack *AddressStack;
typedef struct functionAddrTable *FunctionAddrTable;
typedef struct functionAddrNode *FunctionAddrNode;

typedef enum synType    {

        MAIN= 1,
        INSTRUC = 2,
        OP = 3,
        VAR = 4,
        NUM = 5,
		CONNECTOR = 6

} synType;

typedef enum ifComp	{
	
	GR = 1,
	LT = 2,
	GR_E = 3,
	LT_E = 4,
	EQU = 5,
	N_EQU = 6

} ifComp;

typedef enum iMode	{
	
	skip = 0,
	exec = 1 

} iMode;

/*----------Symbolic Constants-----------*/

#define MAXCALC			15		//! Maximum number of digits in result of polish calculator
#define MAXVAR			26		//! Maximum number of variables that can be declared
#define VAR_T_COLUMNS	2	//!Number of columns in variable table
#define V_NAME_COL		0		//!Column for storing variable name
#define V_VAL_COL		1		//!Column for storing variable value
#define VARIABLE		1
#define	MOVE_COMMAND	2
#define NEW_VAR			0		//!Initial value for undefined variable
#define RESET_CW		0		//!Resets current word to 0
#define SET_VARIABLE	3		//! Variable to set in set instruction
#define SET_VALUE		1		//!Value to set variable
/*----------Function Prototypes-----------*/

//!Program token list functions
void createProgram();
Program getProgram(Program cProg);
void addToken(char *token);
char** increaseStringList(char **stringList, int current);
void printTokenList();
int getTotalTokens();
void clearTokens();
int getCw();
void setCw(int newValue);
int compCurrCw(char *comparison);
char* getCToken();
void freeProgramArray();

//!Variable functions
char addVariable(char var, double val);
char checkVarDeclared(char var);
void createVarTable();
VarTable getVarTable(VarTable nVT);
double getVariable(char var);
int checkVarUnique(char var);
double updateVariable(char var,double val);
double* getVarAddress(char var);

//!Calculation Stack Functions
void createCalcStack();
CalcNode createCalcNode();
CalcNode createCalcOpNode(char *op);
CalcNode createCalcValNode(double value);
void addCalcNode(CalcNode newNode);
CalcStack getCalcStack(CalcStack cStack);
CalcNode popCalcStack();
double calculatePolish();
int calcStackEmpty();
void clearCalcStack();
void freeCalcStack();

//!Syntax Stack functions
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
int getNumberSynNodes();
void pushMode(iMode nm);
iMode popMode();
iMode getMode();
void setMode(iMode nM);
int getInstructionLength();
void freeSyntaxStack();

//!Checks for each type of token
int valueCheck();
int movementParse(char *instruction);
int doParse(char *instruction);
void setParse();
void polishParse();
void ifParse(char *instruction);

//!Interpreting functions
void moveInterpret(char *command, double value);

//!General parse support functions
int checkIfNumber();
int checkIfVariable(char *instruction);
ifComp checkIfComparator(char *comparator);
int ifComparison(ifComp op, double valA, double valB);
int countBlock();
int specParse(char *error,char *comparison, synType t );
int lBraceParse(char *error);
void closeBlock(char *error);

//! If statement functions
void elseParse();
void skipElse();
void skipElif();
void ifDecision(int ifResult);
void ifBlock(iMode mT);
int checkForEmptyBlock();
void funcParse();
int validateFunctionName(char *fName); 

//!Recursive descent functions
int code();
void prog();
void statement();

void addNewFunction(char *fName, int address);
FunctionAddrTable getFTable(FunctionAddrTable nTab);
void createFunctionTable();
int checkifFuncUnique(char *func);
int checkIfFunction(char *name);
int getFunctionAddress(char *name);
void popAddressStackNode();
void freeFunctionTable();
void clearFunctionTable();
/*----------Testing Function Prototypes-----------*/

void parserUnitTests();
void polishCalcTests();
void varTableTests();
void programArrayTests();
void syntaxStackTests();
void bracketsTest();
void parsingTests();
void functionTests();
