#ifndef turtle_h
#define turtle_h

#include "libraryFunctions.h"
#include "testLib.h"

/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define M_PI           3.14159265358979323846
#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0 / M_PI))
#define DEGREES_TO_RADIANS(degrees) (degrees * M_PI / 180.0)

/*----------Structures----------*/

typedef struct turtle *Turtle;

/*----------Function Prototypes-----------*/
void createTurtle();
Turtle getTurtle(Turtle nt);
void initTurtle();

double turnTurtleLeft(double degrees);
double turnTurtleRight(double degrees);
void setAngle(double nAngle);

double getAngle();
double getTargetY();
double getTargetX();
double getOriginY();
double getOriginX();

void turtleUnitTests();
void turnTurtleRightTest();
void turnTurtleLeftTest();
void moveTurtleForward(double magnitude);

#endif
