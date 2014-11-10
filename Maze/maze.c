//
// maze.c
// Created by bclarke on 2014/10/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/maze.h"
#include ".headers/mazeDataFunctions.h"

/*---------- Main -----------*/

int main(int argc, char *argv[]){
	argc--;
	readMaze(argv[1]);
	return 0;

}

/*---------- Functions ----------*/

FILE *readMaze(char fileLocation[])	{
	char dimensions[2][3];
	FILE *fp;
	char letter;
	int lineCount = 0, charCount = 0, rowDim = 0, colDim = 0;
	if ((fp = fopen(fileLocation,"r")) != NULL) {
		while((letter = getc(fp)) != EOF)	{
			//iprint(letter);
			if(letter == '\n')	{
				lineCount++;
			}
			//First line only
			if(!lineCount && isdigit(letter))	{	
					gettingDimensions(dimensions,letter,charCount++, &rowDim, &colDim);
				if(charCount > 3)	{
					createMap(rowDim,colDim);
				}
			} else {
				//putchar(letter);
			}

			}
	} else {
		fprintf(stderr, "null address generated\n");	
		exit(1);
	}
	
	return fp;
}

int gettingDimensions(char dimensions[2][3], char letter,int charCount, int *rowLngth, int *colLngth)	{
		switch (charCount)	{
			case 0:
				dimensions[0][0] = letter;	
				break;
			case 1:
				dimensions[0][1] = letter;
				dimensions[0][2] = '\0';
				*rowLngth = atoi(dimensions[0]);
				break;
			case 2:
				dimensions[1][0] = letter;
				break;
			case 3:
				dimensions[1][1] = letter;
				dimensions[1][2] = '\0';
				*colLngth = atoi(dimensions[1]);
				break;
			default:
				break;
		}

	return 1;
}
