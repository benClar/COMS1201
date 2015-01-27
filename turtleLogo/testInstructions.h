#ifndef interpreter_h
#define interpreter_h

#include "libraryFunctions.h"
#include "testLib.h"
#include "turtle.h"
#include "grammar.h"
#include "neillsdl2.h"

/*----------Enumerated Types-----------*/

typedef struct parseArr *ParseArr;
typedef struct parseNode *ParseNode;

/*----------Symbolic Constants-----------*/

#define TESTING 1

/*----------Structures----------*/


/*----------Function Prototypes-----------*/

void createParseArr();
ParseArr getParseArr(ParseArr cp);
int addParseNode(char *cmd, int value);
void clearParseArr();
void freeParseArr();
char* pCommand(ParseNode n);
int pVal(ParseNode n);
int getParseNodeNumber();
void readParseArray();
ParseNode getSpecParseNode(int n);

void interpreterUnitTests();
void parseArrayTests();
#endif
