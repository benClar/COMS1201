//! Bit enccoder module
//
// Extension: Encodes and decodes boards into bit strings through bit wise operators.  Reduces amount of memory 
// board takes to store and can be used for quick comparrison via equality operator
//

/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "bitEncoder.h"

/*---------- Functions ----------*/

/*
 *Encodes board formation into a bit string
 */
int bitEncoder(BoardNode board)	{
    int row = 0, col = 0;
    uint64_t bitID = 0;
    for(row = 0; row < MAXROW; row++)   {
        for(col = 0; col < MAXCOL; col++)   {
            if(getButtonStatus(board,row,col))    {
                if(bitID==0)    {
                    bitID = 1;
                } else {	//!addings 1 to LSB
                    bitID <<= 1;
                    bitID |=  1;
                }
            } if(!getButtonStatus(board,row,col)){
                bitID <<= 1; //!adding zero to LSB
            }
        }
    }
	setBitID(board,bitID); //!Setting bit ID on board structure
	return bitID;
	
}

/*
 *Decodes bits back to 2d array from bit string
 */
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

/*
 *Decodes final series of boards
 */
void decodeFinalSeries()	{

	BoardNode fBoard= getFinalBoard();

	if(fBoard != NULL)	{
		while(fBoard!= NULL)	{
			bitDecoder(fBoard);
			fBoard = getParent(fBoard);
		}
	}
}


