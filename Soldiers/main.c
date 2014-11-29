//
// main.c
// Created by bclarke on 2014/29/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "boardModule.h"
#include "debug.h"
#include "main.h"

/*---------- Main -----------*/

int main(int argc, char *argv[]){
	
	createQueue();
	//userEnterTargetDestination();	
	addBoard(1,Testing);
	return 0;

}

/*---------- Functions ----------*/

/*
 *	User enters target destination for counter
 */
void userEnterTargetDestination()	{
	int row, col;	
	printf("Please enter your target x coordinate:	");
	scanInt(&col,MAXCOL,MINCOORD);
	pNL();
	printf("Please enter your target y coordinate:	");
	scanInt(&row,MAXROW,MINCOORD);
	setTargetMove(row,col);
}

/*
 *  Safely Scans integers from keyboard within specified range
 */
void scanInt(int *toScan, int max, int min) {

	do  {
        while(!scanf("%d", toScan))       {
               clearInputBuf
               printf("please enter integer\n");
        }
		if(*toScan > max)	{
			printf("Please ensure integer entered is less than %d\n",max);
		} else if (*toScan < min)	{
			printf("Please ensure integer entered is greater than %d\n",min);
		}
	} while (*toScan > max || *toScan < min);

}

void readDefaultMap()	{

	FILE *fp;
	int button, col, row;
	if((fp = fopen("defaultBoard.txt","r")) != NULL) {
		for(col = 0, row = 0; (button = getc(fp)) != EOF ; col++)	{
			if(col == MAXCOL)	{
				col = 0;
				row++;
			}
			if(button == 1 || button == 0)	{
				addButtonToBoard(firstBoard, button, row, col);
			}
		}
	}
}
