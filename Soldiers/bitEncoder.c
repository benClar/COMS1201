/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "bitEncoder.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Main -----------*/

/*---------- Functions ----------*/

int bitEncoder(BoardNode board)	{
    int row = 0, col = 0;
    uint64_t bitID = 0;
    for(row = 0; row < MAXROW; row++)   {
        for(col = 0; col < MAXCOL; col++)   {
            if(getButtonStatus(board,row,col))    {
                if(bitID==0)    {
                    bitID = 1;
                } else {
                    bitID <<= 1;
                    bitID |=  1;
                }
            } if(!getButtonStatus(board,row,col)){
                bitID <<= 1;
            }
        }
    }
	setBitID(board,bitID);
	return bitID;
	
}

BoardNode bitDecoder(BoardNode board)	{

	uint64_t bitID = getBitID(board);
	createBoardArray(board);
	int row = 0, col = 0;

	for(row = MAXROW-1; row >= 0; row--)	{
		for(col = MAXCOL -1; col >= 0; col--)	{
			if(bitID%2 ==0)	{
				changeButton(board,row,col,DEAD);		
			} else {
				changeButton(board,row,col,ALIVE);
			}
				bitID>>=1;
		}
	}

	return board;
}

void decodeFinalSeries()	{

	BoardNode fBoard= getFinalBoard();

	if(fBoard != NULL)	{
		while(fBoard!= NULL)	{
			bitDecoder(fBoard);
			fBoard = getParent(fBoard);
		}
	}
}


