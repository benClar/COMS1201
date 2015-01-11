#include "libraryFunctions.h"
#include "testLib.h"

/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0 / M_PI))
#define DEGREES_TO_RADIANS(degrees) (degrees * M_PI / 180.0)

/*----------Structures----------*/

typedef struct turtle *Turtle;

/*----------Function Prototypes-----------*/
void createTurtle();
Turtle getTurtle(Turtle nt);

int turnTurtleLeft(int degrees);
int turnTurtleRight(int degrees);
void setAngle(int nAngle);
int getAngle();

void turtleUnitTests();
void turnTurtleRightTest();
void turnTurtleLeftTest();
void moveTurtleForward(int magnitude);
