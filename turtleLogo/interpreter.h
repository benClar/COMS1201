#ifndef interpreter_h
#define interpreter_h

#include "libraryFunctions.h"
#include "testLib.h"
#include "turtle.h"
#include "grammar.h"
//#include "display.h"
#include "neillsdl2.h"

/*----------Enumerated Types-----------*/

typedef struct parseArr *ParseArr;
typedef struct parseNode *ParseNode;

/*----------Symbolic Constants-----------*/


/*----------Structures----------*/


/*----------Function Prototypes-----------*/

void createParseArr();
ParseArr getParseArr(ParseArr cp);
int addParseNode(char *cmd, int value);
void freeParseArr();
char* pCommand(ParseNode n);
int pVal(ParseNode n);

void moveInterpret(char *command, int value);
void readParseArray();
void interpreterUnitTests();

#endif
