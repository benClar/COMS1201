/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "main.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Main -----------*/

int main(){

	initialize();
	readFile("testingFile.txt");
	printTokenList();
	prog();
	//testing();
}

/*---------- Functions ----------*/

void initialize()	{

		createProgram();
		createSuite();
		createSynStack();
}

void testing()	{
	parserUnitTests();
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
