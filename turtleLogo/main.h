#include "parser.h"
#include "interpreter.h"
#include "turtle.h"
//#include "display.h"
#include "neillsdl2.h"

/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define FILE_ARG 1
#define MAX_ARG		2

/*----------Structures----------*/



/*----------Function Prototypes-----------*/

void initialize(int argc);
void end();
void readFile(char *fileName);
void testing();

/*----------Testing Function Prototypes-----------*/

void interpreterToTurtle();
void singleIfTest();
void singleElifTest();
void singleElseTest();
void nestedInIfTest();
void nestedIfInElifTest();
void ElifNestedInElseTest();
void nestedElifTest();
void nestedElseTest();

void whileLoopTestTurtle();

