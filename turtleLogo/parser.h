#include "testLib.h"

/*----------Enumerated Types-----------*/


/*----------TypeDefs-----------*/

typedef struct program *Program;
typedef struct syntaxStack *SyntaxStack;
typedef struct syntaxNode *SyntaxNode;

/*----------Symbolic Constants-----------*/


/*----------Function Prototypes-----------*/

void createProgram();
Program getProgram(Program cProg);
void addToken(char *token);
char** increaseStringList(char **stringList, int current);
void printTokenList();
void prog();
int getTotalTokens();

int getCw();
void setCw(int newValue);
int compCurrCw(char *comparison);

SyntaxNode createNode(char *type);
void createSynStack();
SyntaxStack getSynStack(SyntaxStack nStack);
void addNode(SyntaxNode Node);
int checkSynStackEmpty();
int removeLastSNodeOfType(char *type);
void removeNode(SyntaxNode node);
int checkNoMoreWords();
/*----------Testing Function Prototypes-----------*/

void parserUnitTests();

