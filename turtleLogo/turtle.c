//////
//
//Turtle.c
//Holds values of turtle and provides means to access and set them
//
/////////////////////


/*---------- Standard Headers -----------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*---------- Custom Headers -----------*/
#include "turtle.h"
#include "neillsdl2.h"
#include "debug.h"

//! Structure that holds turtle values
struct turtle	{

		double originX; //@ Starting point of move on x axis 
		double originY;	//@ Starting point of move on y axis
		double targetX;	//@ Destination of move on x axis
		double targetY;	//@ Destination of move on y axis
		double angle;	//@Current angle turtle is facing
};

/*---------- Functions ----------*/

/*
 *Creates turtle structure
 */
void createTurtle()	{

	Turtle t = checkMalloc(malloc(sizeof(*t)));
	getTurtle(t);
	initTurtle();
}
/*
 *Resets turtle's data
 */
void initTurtle()	{
	Turtle t = getTurtle(NULL);
	t->originX = SCREEN_WIDTH/HALF;
	t->originY = SCREEN_HEIGHT/HALF;
	t->targetX = SCREEN_WIDTH/HALF;
	t->targetY = SCREEN_HEIGHT/HALF;
	t->angle = 270;
	
}

/*
 *Returns pointer to turtle structure
 */
Turtle getTurtle(Turtle nt)	{

	static Turtle ct;

	if(nt != NULL)	{
		ct = nt;
	}

	return ct;

}

/*
 *Accessor for target Y value
 */
double getTargetY()	{
	return getTurtle(NULL)->targetY;
}

/*
 *Accessor for target X value
 */
double getTargetX()	{
	return getTurtle(NULL)->targetX;
}

/*
 *Accessor for origin Y value
 */
double getOriginY()	{
	return getTurtle(NULL)->originY;
}

/*
 *Accessor for origin X value
 */
double getOriginX()	{
	return getTurtle(NULL)->originX;
}

/*
 *Turn turtle clockwise
 */
double turnTurtleRight(double degrees)	{

	Turtle t = getTurtle(NULL);
	t->angle += fmod(degrees,DEGREES);	
	t->angle = fmod(t->angle,DEGREES);
	return t->angle;
}

/*
 *Turn turtle anti-clockwise
 */
double turnTurtleLeft(double degrees)	{

	Turtle t = getTurtle(NULL);
	degrees = (fmod(degrees,DEGREES));
	if(degrees > t->angle)	{
		degrees -= t->angle;	
		t->angle = DEGREES - degrees;
	} else {
		t->angle -= degrees;
	}
	return t->angle;
}

/*
 *Set function for turtle angle value
 */
void setAngle(double nAngle)	{

	Turtle t = getTurtle(NULL);
	t->angle = nAngle;

}

/*
 *Accessor for angle value
 */
double getAngle()	{

	Turtle t = getTurtle(NULL);
	return t->angle;
}

/*
 *Moves turtle value forward by specified amount
 */
void moveTurtleForward(double magnitude)	{

	Turtle t = getTurtle(NULL);
	t->originX = t->targetX;
	t->originY = t->targetY;
	t->targetX += magnitude*(cos(DEGREES_TO_RADIANS(t->angle)));
	t->targetY += magnitude*(sin(DEGREES_TO_RADIANS(t->angle)));
}

/*---------- Testing Functions ----------*/

void turtleUnitTests()	{

	turnTurtleRightTest();
	turnTurtleLeftTest();
}

void turnTurtleRightTest()	{

	enterSuite("Turning Turtle ClockWise");
	setAngle(0);
	testVal((int) turnTurtleRight(90),90,"Valid: Turning turtle facing 0 degrees clockwise 90 degrees",EQUALS);
	testVal((int) turnTurtleRight(360),90,"Valid: Turning turtle facing 90 degrees clockwise 360 degrees",EQUALS);
	testVal((int) turnTurtleRight(270),0,"Valid: Turning turtle facing 90 degrees clockwise 270 degrees",EQUALS);
	setAngle(330);
	testVal((int) turnTurtleRight(100),70,"Valid: Turning turtle facing 330 degrees clockwise 100 degrees",EQUALS);
	initTurtle();
	leaveSuite();
}

void turnTurtleLeftTest()	{
	enterSuite("Turning Turtle AntiClockWise");
	setAngle(0);
	testVal(turnTurtleLeft(90),270,"Valid: Turning turtle facing 0 degrees anti-clockwise 90 degrees",EQUALS);
	testVal(turnTurtleLeft(240),30,"Valid: Turning turtle facing 270 degrees anti-clockwise 240 degrees",EQUALS);
	testVal(turnTurtleLeft(110),280,"Valid: Turning turtle facing 30 degrees anti-clockwise 110 degrees",EQUALS);
	initTurtle();	
	leaveSuite();
}
