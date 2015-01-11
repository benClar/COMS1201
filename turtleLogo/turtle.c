/*---------- Standard Headers -----------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*---------- Custom Headers -----------*/
#include "turtle.h"
#include "display.h"
#include "debug.h"

struct turtle	{

		double x;
		double y;
		int angle;
};

/*---------- Functions ----------*/
void createTurtle()	{

	Turtle t = checkMalloc(malloc(sizeof(*t)));
	t->x = SCREEN_WIDTH/HALF;
	t->y = SCREEN_HEIGHT/HALF;
	t->angle = 0;
	getTurtle(t);
}

Turtle getTurtle(Turtle nt)	{

	static Turtle ct;

	if(nt != NULL)	{
		ct = nt;
	}

	return ct;

}

int turnTurtleRight(int degrees)	{

	Turtle t = getTurtle(NULL);
	t->angle += (degrees%DEGREES);	
	t->angle = (t->angle%DEGREES);
	return t->angle;
}

int turnTurtleLeft(int degrees)	{

	Turtle t = getTurtle(NULL);
	degrees = (degrees%DEGREES);
	if(degrees > t->angle)	{
		degrees -= t->angle;	
		t->angle = DEGREES - degrees;
	} else {
		t->angle -= degrees;
	}
	return t->angle;
}

void setAngle(int nAngle)	{

	Turtle t = getTurtle(NULL);
	t->angle = nAngle;

}

int getAngle()	{

	Turtle t = getTurtle(NULL);
	return t->angle;
}

void moveTurtleForward(int magnitude)	{

	Turtle t = getTurtle(NULL);
	t->x += magnitude*(cos((double) DEGREES_TO_RADIANS((double) t->angle)));
	t->y -= magnitude*(sin((double) DEGREES_TO_RADIANS((double) t->angle)));
}

/*---------- Testing Functions ----------*/

void turtleUnitTests()	{

	enterSuite("Turning Turtle ClockWise");
	turnTurtleRightTest();
	leaveSuite();

	enterSuite("Turning Turtle AntiClockWise");
	turnTurtleLeftTest();
	leaveSuite();
}

void turnTurtleRightTest()	{

	setAngle(0);
	testVal(turnTurtleRight(90),90,"Valid: Turning turtle facing 0 degrees clockwise 90 degrees");
	testVal(turnTurtleRight(360),90,"Valid: Turning turtle facing 90 degrees clockwise 360 degrees");
	testVal(turnTurtleRight(270),0,"Valid: Turning turtle facing 90 degrees clockwise 270 degrees");
	setAngle(330);
	testVal(turnTurtleRight(100),70,"Valid: Turning turtle facing 330 degrees clockwise 100 degrees");
}

void turnTurtleLeftTest()	{
	setAngle(0);
	testVal(turnTurtleLeft(90),270,"Valid: Turning turtle facing 0 degrees anti-clockwise 90 degrees");
	testVal(turnTurtleLeft(240),30,"Valid: Turning turtle facing 270 degrees anti-clockwise 240 degrees");
	testVal(turnTurtleLeft(110),280,"Valid: Turning turtle facing 30 degrees anti-clockwise 110 degrees");
	
}





