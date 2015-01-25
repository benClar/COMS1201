/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "main.h"

/*---------- Symbolic Constants  -----------*/


/*---------- To do -----------*/
//! Testing
//! Extension
//! Argv
/*---------- Main -----------*/

int main(){
	initialize();
	if(TESTING)	{
		testing();
	} 
	readFile("testingFile.txt");
	prog();
	do	{
  		readParseArray();
		endFrame();
		initTurtle();
	}while(!checkFinished());
	end();
}

/*---------- Functions ----------*/

void initialize()	{

		createProgram();
		createSuite();
		createSynStack();
		createCalcStack();
		createTurtle();
		createVarTable();
		createParseArr();
		if(!TESTING)	{
			Neill_SDL_Init();
		}
}

void end()	{
	freeParseArr();
	clearTokens();
	exit(1);
}


void readFile(char *fileName)	{
	
	FILE *fp;
	int tokenLength = 0;
	char *token = NULL;
	char c;
	if((fp = fopen(fileName, "r")) != NULL){
		while((c = getc(fp)) != EOF)	{
			if(c != ' ' && c != '\n' && c != '	')	{
				token = increaseCharBuffer(token,tokenLength+1);
				token[tokenLength] = c;
				tokenLength++;
			} else if (tokenLength){
				token = increaseCharBuffer(token,tokenLength+1);
				token[tokenLength] = '\0';
				addToken(token);
				tokenLength = 0;
				free(token);
				token = NULL;
			}
		}
	} else {
		fprintf(stderr, "Cannot open %s\n",fileName);
		exit(1);
	}
}

void testing()	{
	
	//!Unit Tests
	parserUnitTests();
	turtleUnitTests();
	interpreterUnitTests();
	libraryUnitTests();

	//!Component Tests
	interpreterToTurtle();	
	parserToTurtle();

	finishTesting();
	end();
}

void parserToTurtle()	{
	enterSuite("Parser passing to Interpreter which moves turtle");
	int i,oldX,oldY;
	setAngle(0);	
	//! Simulating a do loop being placed in program array
	addToken(DO);
	addToken("A");
	addToken(FROM);
	addToken("1");
	addToken(TO);
	addToken("4");
	addToken(R_BRACE);
	addToken(SET);
	addToken("B");
	addToken(P_EQUALS);
	addToken("A");
	addToken("5");
	addToken(ADD);
	addToken(END_POLISH);
	addToken(FORWARD);
	addToken("B");
	addToken(L_TURN);
	addToken("90");
	addToken(L_BRACE);	

	//!Parsing Loop
	doParse(DO);
	removeCurrentInstruction();
	testVal(checkSynStackEmpty(),1,"Valid: While Instruction has been popped. Empty Synax Stack",EQUALS);
	//! Checking correct number of intructions have been passed to interpreter
	testVal(getParseNodeNumber(),8,"Valid: While loop has placed 8 items in parse array to be interpreted",EQUALS);
	for(i = 0 ; i < getParseNodeNumber(); i++)	{
		oldX = getTargetX();
		oldY = getTargetY();
		moveInterpret(pCommand(getSpecParseNode(i+1)),pVal(getSpecParseNode(i+1)));
		switch(i)	{
			case 0:
				testVal(oldX,getTargetX(),"Valid: Turtle moves right along x axis only",LESS);
				testVal(oldY,getTargetY(),"Valid: Turtle y axis remains the same",EQUALS);
				break;
			case 1:
				testVal(getAngle(),270,"Valid: Turtle turned anti-clockwise 90 degrees",EQUALS);
				break;
			case 2:
				testVal(oldX,getTargetX(),"Valid: Turtle x axis remains the same",EQUALS);
				testVal(oldY,getTargetY(),"Valid: Turtle moves up along y axis",GREATER);
				break;
			case 3:
				testVal(getAngle(),180,"Valid: Turtle turned anti-clockwise 90 degrees",EQUALS);
				break;
			case 4:
				testVal(oldX,getTargetX(),"Valid: Turtle moves left along x axis",GREATER);
				testVal(oldY,getTargetY(),"Valid: Turtle y axis remains the same",EQUALS);
				break;
			case 5:
				testVal(getAngle(),90,"Valid: Turtle turned anti-clockwise 90 degrees",EQUALS);
				break;
			case 6:
				testVal(oldX,getTargetX(),"Valid: Turtle x axis remains the same",EQUALS);
				testVal(oldY,getTargetY(),"Valid: Turtle moves down along y axis",LESS);
				break;
			case 7:
				testVal(getAngle(),0,"Valid: Turtle turned anti-clockwise 90 degrees",EQUALS);
				break;
			default:
				fprintf(stderr,"Test loop has done too many iterations");
				exit(1);
				break;
		}
	}
	clearParseArr();
	initTurtle();
	leaveSuite();		
}

void interpreterToTurtle()	{
	enterSuite("Interpreter moving turtle");
	setAngle(0);
	addParseNode(FORWARD,10);
	double oldX, oldY, oldAng;
	oldX = getTargetX();
	oldY = getTargetY();
	oldAng = getAngle();

	moveInterpret(pCommand(getSpecParseNode(1)),pVal(getSpecParseNode(1))); //! Reading first parse token
	testVal(getTargetX(),oldX,"Valid: Turtle moved forward along x axis only",GREATER);
	testVal(getTargetY(),oldY,"Valid: Turtle has not moved along y axis",EQUALS);
	addParseNode(R_TURN,180);
	moveInterpret(pCommand(getSpecParseNode(2)),pVal(getSpecParseNode(2))); //! Reading second parse token
	testVal(oldAng,getAngle(),"Valid: Turtle turned clockwise by 180 degrees and will have larger angle",LESS);
	oldAng = getAngle();
	addParseNode(L_TURN,70);
	moveInterpret(pCommand(getSpecParseNode(3)),pVal(getSpecParseNode(3))); //! Reading third parse token
	testVal(oldAng,getAngle(),"Valid: Turtle turned anti-clockwise 70 degrees and will have smaller angle",GREATER);

	addParseNode(FORWARD,70);
	oldX = getTargetX();
	oldY = getTargetY();	
	moveInterpret(pCommand(getSpecParseNode(4)),pVal(getSpecParseNode(4)));	
	testVal(oldX,getTargetX(),"Valid: Turtle moved left along x axis",GREATER);
	testVal(oldY,getTargetY(),"Valid: Turtle moved down along y axis",LESS);
	clearParseArr(); //!Removing all tokens from parse array
	initTurtle(); //!Reset turtle for other test suites
	leaveSuite();
}
