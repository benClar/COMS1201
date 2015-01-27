/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "main.h"

/*---------- Symbolic Constants  -----------*/


/*---------- To do -----------*/
//! PROG BLACKBOX IF TESTING
//!CODE REFACTOR
/*---------- Main -----------*/

int main(int argc, char *argv[]){
	initialize(argc);
	if(TESTING)	{
		testing();
	} 
	readFile(argv[FILE_ARG]);
	prog();
	do	{
		endFrame();
	}while(!checkFinished());
	end();
}

/*---------- Functions ----------*/

void initialize(int argc)	{
		if(argc != MAX_ARG && !TESTING)	{
			ERROR("You must specify file.");
		}
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
	freeCalcStack();
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
	whileLoopTestTurtle();
	ifStatementTest();
	finishTesting();
	end();
}

void ifStatementTest()	{
	singleIfTest();
    singleElifTest();
    singleElseTest();
    nestedInIfTest();
    nestedIfInElifTest();
    ElifNestedInElseTest();
	emptyIf();
}

void clearForTesting()	{
	setCw(RESET_CW);
	clearTokens();
    clearParseArr();
    initTurtle();
	setAngle(0);
}
void singleIfTest()	{
    enterSuite("Simulating single if statement and moving turtle");
	int oldY, oldX;
    oldX = getTargetX();
    oldY = getTargetY();

	clearForTesting();
    addToken(IF);
    addToken("1");
    addToken(L_THAN);
    addToken("2");
    addToken(R_BRACE); //!True: Execute
    addToken(FORWARD);
    addToken("10");
    addToken(L_BRACE);
	addToken(L_BRACE);
	ifParse(IF);
	removeCurrentInstruction();
    testVal(checkSynStackEmpty(),1,"Valid: If Instruction has been popped. Empty Synax Stack",EQUALS);
	testVal(getParseNodeNumber(),1,"Valid: One instruction has been placed on parse queue",EQUALS);	
	testVal(pVal(getSpecParseNode(1)),10,"Valid: The ELIF instruction has been placed in list",EQUALS);
	moveInterpret(pCommand(getSpecParseNode(1)),pVal(getSpecParseNode(1)));

	testVal(oldX,getTargetX(),"Valid: Turtle moves right along x axis only",LESS);
	testVal(oldY,getTargetY(),"Valid: Turtle y axis remains the same",EQUALS);

	clearForTesting();	
	leaveSuite();
}

void singleElifTest()	{
	enterSuite("Simulating a single if else statement and moving turtle");
	clearForTesting();

	int oldY, oldX;
    oldX = getTargetX();
    oldY = getTargetY();

    addToken(IF);
    addToken("1");
    addToken(G_THAN);
    addToken("2");
    addToken(R_BRACE);
    addToken(FORWARD); //!False
    addToken("10");
    addToken(L_BRACE);
	addToken(ELIF);
	addToken("10");
	addToken(G_THAN);
	addToken("5");
	addToken(R_BRACE); //! True: Execute
	addToken(R_TURN);
	addToken("90");
	addToken(L_BRACE);
	addToken(L_BRACE);
	ifParse(IF);
	removeCurrentInstruction();
	testVal(checkSynStackEmpty(),1,"Valid: If Instruction has been popped. Empty Synax Stack",EQUALS);
	testVal(getParseNodeNumber(),1,"Valid: One instruction has been placed in list",EQUALS);
	testVal(pVal(getSpecParseNode(1)),90,"Valid: The ELIF instruction has been placed in list",EQUALS);
	testVal(oldX,getTargetX(),"Valid: Turtle has not moved",EQUALS);
    testVal(oldY,getTargetY(),"Valid: Turtle has not moved",EQUALS);
    testVal(getAngle(),90,"Valid: Turtle turned 90 degrees clockwise",EQUALS);
	clearForTesting();	
	leaveSuite();
}

void singleElseTest()	{
	enterSuite("Simulating a single if, elif, else statement and moving turtle");
	clearForTesting();

    int oldY, oldX;
    oldX = getTargetX();
    oldY = getTargetY();

    addToken(IF);
    addToken("1");
    addToken(G_THAN);
    addToken("2");
    addToken(R_BRACE); //!False
    addToken(FORWARD);
    addToken("10");
    addToken(L_BRACE);
	addToken(ELIF);
	addToken("10");
	addToken(L_THAN);
	addToken("5");
	addToken(R_BRACE); //!False
	addToken(R_TURN);
	addToken("90");
	addToken(L_BRACE);
	addToken(ELSE); //!Execute
	addToken(R_BRACE);
	addToken(L_TURN);
	addToken("100");
	addToken(L_BRACE);
	addToken(L_BRACE);
	ifParse(IF);
	removeCurrentInstruction();
	testVal(checkSynStackEmpty(),1,"Valid: If Instruction has been popped. Empty Synax Stack",EQUALS);
	testVal(getParseNodeNumber(),1,"Valid: One instruction has been placed in list",EQUALS);
	testVal(pVal(getSpecParseNode(1)),100,"Valid: The ELIF instruction has been placed in list",EQUALS);
	testVal(oldX,getTargetX(),"Valid: turtle has not moved",EQUALS);
    testVal(oldY,getTargetY(),"Valid: turtle has not moved",EQUALS);
	testVal(getAngle(),260,"Valid: Turtle turned 100 degrees anti-clockwise",EQUALS);
	clearForTesting();	
	leaveSuite();
}

void nestedInIfTest()	{
	enterSuite("Nested if statement in if section of parent statement.");
	clearForTesting();

	int oldY, oldX;
    oldX = getTargetX();
    oldY = getTargetY();

    addToken(IF);
    addToken("1");
    addToken(L_THAN);
    addToken("2");
    addToken(R_BRACE); //!True
    addToken(IF);
    addToken("4");
    addToken(G_THAN);
    addToken("2");
    addToken(R_BRACE); //!True : Execute
    addToken(FORWARD);
    addToken("10");
    addToken(L_BRACE);
    addToken(L_BRACE);
	addToken(ELSE);
	addToken(R_BRACE);
	addToken(R_TURN);
	addToken("100");
	addToken(L_BRACE);
	addToken(L_BRACE);
	ifParse(IF);
	removeCurrentInstruction();
    testVal(checkSynStackEmpty(),1,"Valid: If Instruction has been popped. Empty Synax Stack",EQUALS);
	testVal(getParseNodeNumber(),1,"Valid: One instruction has been placed on parse queue. Else statement ignored.",EQUALS);	
	testVal(pVal(getSpecParseNode(1)),10,"Valid: The ELIF instruction has been placed in list",EQUALS);
	testVal(oldX,getTargetX(),"Valid: Turtle moves right along x axis only",LESS);
    testVal(oldY,getTargetY(),"Valid: Turtle y axis remains the same",EQUALS);
	clearForTesting();
	leaveSuite();

}

void nestedIfInElifTest()	{
	enterSuite("If statement nested in elif statement of parent statement");
	clearForTesting();

	int oldY, oldX;
    oldX = getTargetX();
    oldY = getTargetY();

    addToken(IF);
    addToken("1");
    addToken(G_THAN);
    addToken("2");
    addToken(R_BRACE); //!False
    addToken(FORWARD);
    addToken("10");
    addToken(L_BRACE);
	addToken(ELIF);
	addToken("10");
	addToken(G_THAN); //!True
	addToken("5");
	addToken(R_BRACE);
	addToken(IF);
	addToken("20");
	addToken(EQ);
	addToken("20");
	addToken(R_BRACE); //!True: Execute
	addToken(R_TURN);
	addToken("90");
	addToken(L_BRACE);
	addToken(L_BRACE);
	addToken(L_BRACE);
	ifParse(IF);
	removeCurrentInstruction();
	testVal(checkSynStackEmpty(),1,"Valid: If Instruction has been popped. Empty Synax Stack",EQUALS);
	testVal(getParseNodeNumber(),1,"Valid: One instruction has been placed in list",EQUALS);
	testVal(pVal(getSpecParseNode(1)),90,"Valid: The ELIF instruction has been placed in list",EQUALS);

	testVal(oldX,getTargetX(),"Valid: Turtle has not moved",EQUALS);
    testVal(oldY,getTargetY(),"Valid: Turtle turtle has not moved",EQUALS);
	testVal(getAngle(),90,"Valid: Turtle turned 90 degrees clockwise",EQUALS);

	clearForTesting();	
	leaveSuite();


}

void emptyIf()	{
	enterSuite("Simulating empty if statement");
	clearForTesting();
	addToken(IF);
	addToken("1");
	addToken(G_THAN);
	addToken("2");
	addToken(R_BRACE);
	addToken(L_BRACE);
	addToken(ELIF);
	addToken("1");
	addToken(G_THAN);
	addToken("0");
	addToken(R_BRACE);
	addToken(L_BRACE);
	addToken(ELSE);
	addToken(R_BRACE);
	addToken(IF);
	addToken("1");
	addToken(G_THAN);
	addToken("0");
	addToken(R_BRACE);
	addToken(L_BRACE);
	addToken(L_BRACE);
    ifParse(IF);
    removeCurrentInstruction();
    testVal(checkSynStackEmpty(),1,"Valid: If Instruction has been popped. Empty Synax Stack",EQUALS);
    testVal(getParseNodeNumber(),0,"Valid: No instructions placed in list",EQUALS);
	leaveSuite();

}

void ElifNestedInElseTest()	{
enterSuite("Simulating satisfying an ELIF statement nested in parent ELSE statement and moving turtle");
    clearForTesting();

	int oldY, oldX;
    oldX = getTargetX();
    oldY = getTargetY();

	//!If indenting simulated below for addition of tokens
    addToken(IF);
    addToken("1");
    addToken(G_THAN);
    addToken("2");
   	addToken(R_BRACE); //!False
    	addToken(FORWARD);
 		addToken("10");
    addToken(L_BRACE);
    addToken(ELIF);
	addToken("10");
    addToken(L_THAN);
    addToken("5");
    addToken(R_BRACE); //!False
		addToken(R_TURN);
		addToken("90");
    addToken(L_BRACE);
    addToken(ELSE);
    addToken(R_BRACE); //! Execute else: IF and ELIF both false
		addToken(IF);
		addToken("1");
		addToken(G_THAN);
		addToken("2");
		addToken(R_BRACE); //!False
			addToken(FORWARD);
			addToken("10");
		addToken(L_BRACE);
		addToken(ELIF);
		addToken("500");
		addToken(G_THAN);
	    addToken("5");
		addToken(R_BRACE);	//! True
			addToken(R_TURN);
			addToken("200");
		addToken(L_BRACE);
    addToken(L_BRACE);
    addToken(L_BRACE);
    ifParse(IF);
    removeCurrentInstruction();
    testVal(checkSynStackEmpty(),1,"Valid: If Instruction has been popped. Empty Synax Stack",EQUALS);
    testVal(getParseNodeNumber(),1,"Valid: One instruction has been placed in list",EQUALS);
    testVal(pVal(getSpecParseNode(1)),200,"Valid: The ELIF instruction has been placed in list",EQUALS);
	testVal(oldX,getTargetX(),"Valid: Turtle has not moved",EQUALS);
    testVal(oldY,getTargetY(),"Valid: Turtle has not moved",EQUALS);
	testVal(getAngle(),200,"Valid: Turtle has turned 200 degrees clockwise",EQUALS);
    clearForTesting();
    leaveSuite();
}



void whileLoopTestTurtle()	{
	enterSuite("simulating while loop moving turtle");
	int i,oldX,oldY;
	setAngle(0);	
	setCw(RESET_CW);
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
	clearForTesting();
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
