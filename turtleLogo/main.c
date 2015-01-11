/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "main.h"

/*---------- Symbolic Constants  -----------*/


/*---------- To do -----------*/
//!Do Storing Variables
//! Do looping 
//! Do moving graphics
/*---------- Main -----------*/

int main(){

	initialize();
	setAngle(270);
	//readFile("testingFile.txt");
	//printTokenList();
	//prog();
	while(1)	{
		clearFrame();
		moveTurtleForward(10);		
		endFrame();
		endAnimation();
	}
	//testing();
}

/*---------- Functions ----------*/

void initialize()	{

		createProgram();
		createSuite();
		createSynStack();
		createCalcStack();
		createTurtle();
		init_SDL();
}

void testing()	{
	parserUnitTests();
	turtleUnitTests();
}

void readFile(char *fileName)	{
	
	FILE *fp;
	int tokenLength = 0;
	char *token;
	char c;
	if((fp = fopen(fileName, "r")) != NULL){
		while((c = getc(fp)) != EOF)	{
			if(c != ' ' && c != '\n')	{
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
