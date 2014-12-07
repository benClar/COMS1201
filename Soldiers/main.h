#ifndef main_h
#define main_h

#include "dataParsing.h"
#include "hashExtension.h"
#include "testingModule.h"
#include "moveModule.h"
#include "boardModule.h"

/*----------Enumerated Types-----------*/

//typedef enum moveStage {JUMP, DELETE, DONE} moveStage;

/*----------Symbolic Constants-----------*/

#define EMPTY		0		//!button not present in space from default file
#define	FULL		1		//!button present in space from default file
#define ARGCNT		3		//! Number of arguments that may be passed into program

/*----------Typedefs----------*/


/*----------Function Prototypes-----------*/

int checkInt(int toCheck, int max, int min);
void readDefaultMap();
BoardNode getFinalBoard();
void userEnterTargetDestination(char *x, char *y);
void checkArg(int *argc,char **args);
char *checkEnteredString(char *toCheck);

#endif
