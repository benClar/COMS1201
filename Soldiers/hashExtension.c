/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "hashExtension.h"

/*---------- Symbolic Constants  -----------*/

struct zobrist	{

	ZobristVal **hashKey;	
	
};

struct zobristVal	{
	int alive;
	int dead;
};

struct hashTable	{

	HashNode *table;

};

struct hashNode	{

	BoardNode hashedBoard;
	HashNode nextHNode;

};
/*---------- Functions ----------*/

void createHashTable()	{

	HashTable newTable = (HashTable) checkMalloc(malloc(sizeof(*newTable)));
	newTable->table = (HashNode*) checkMalloc(calloc(HASHTABLESIZE, sizeof(*(newTable->table))));
	getHashTable(newTable);

}

HashNode addHashNode(BoardNode uniqueBoard)	{

	HashNode HNode = (HashNode) checkMalloc(malloc(sizeof(*HNode)));
	HNode->hashedBoard = uniqueBoard;
	HNode->nextHNode = NULL;	
	return HNode;
}

HashTable getHashTable(HashTable currTable)	{

	static HashTable hTable;

	if(currTable != NULL)	{
		hTable = currTable;
	}

	return hTable;

}

void freeHashingStructures()	{

		HashTable hTable = getHashTable(NULL);
		Zobrist zb = getZValues(NULL);
		HashNode boardToFree;
		HashNode temp;
		int row, col;

		for(row = 0; row < MAXROW; row++)	{
			for(col = 0; col < MAXCOL; col++)	{
				free(zb->hashKey[row][col]);
			}
		}
		free(zb);
		for(row = 0; row < HASHTABLESIZE; row++)	{
			boardToFree = hTable->table[row];
			if(boardToFree != NULL)	{
				while(boardToFree->nextHNode != NULL)	{
					temp = boardToFree->nextHNode;
					free(boardToFree);
					boardToFree = temp;	
				}
				free(boardToFree);
			}
		}
		free(hTable);
}

void initZobrist()	{

		int aliveRandom, deadRandom, row, col;
		Zobrist values = (Zobrist) checkMalloc(malloc(sizeof(*values)));
		values->hashKey = (ZobristVal**) checkMalloc(malloc(MAXROW * sizeof(*(values->hashKey))));
		for(row = 0; row < MAXROW; row++)	{
			values->hashKey[row] = (ZobristVal*) checkMalloc(malloc(MAXCOL * sizeof(ZobristVal)));
			for(col = 0; col < MAXCOL; col++)	{
				values->hashKey[row][col] = (ZobristVal) checkMalloc(malloc(sizeof(*(values->hashKey[row][col]))));
			}
		}
		for(row = 0; row < MAXROW; row++)	{
			for(col = 0; col < MAXCOL; col++)	{
				do {
					deadRandom = rand()%HASHTABLESIZE;
					aliveRandom = rand()%HASHTABLESIZE;
				} while(!validateKeys(values->hashKey,deadRandom,aliveRandom));
				values->hashKey[row][col]->alive = deadRandom;
				values->hashKey[row][col]->dead = aliveRandom;
			}
		}
		getZValues(values);
}

Zobrist getZValues(Zobrist zValues)	{

	static Zobrist currZValues;
	if(zValues != NULL)	{
		currZValues = zValues;
	}

	return currZValues;
}

int validateKeys(ZobristVal **array, int dRndm, int aRndm)	{

	int row, col;

	for(row = 0; row < MAXROW; row++)	{
		for(col = 0; col < MAXCOL; col++)	{
			if(dRndm == array[row][col]->alive || dRndm == array[row][col]->dead || dRndm < MINKEYSIZE)	{
				return 0;
			} else if(aRndm == array[row][col]->alive || aRndm == array[row][col]->dead || aRndm < MINKEYSIZE)   {
				return 0;
			}	
		}
	}
	return 1;
}


int generateHashKey(BoardNode boardToHash)	{
		int row = 0, col = 0, key  = 0;

		if(getButtonStatus(boardToHash,row,col))    {
             key = (getZValues(NULL)->hashKey[row][col]->alive);
        } if(!getButtonStatus(boardToHash,row,col)){
             key = (getZValues(NULL)->hashKey[row][col]->dead);
        }

		for(row = 1; row < MAXROW; row++)	{
			for(col = 1; col < MAXCOL; col++)	{
				if(getButtonStatus(boardToHash,row,col))	{
					key ^= (getZValues(NULL)->hashKey[row][col]->alive);
				} if(!getButtonStatus(boardToHash,row,col)){
					key ^= (getZValues(NULL)->hashKey[row][col]->dead);
				}
			}
		}
		key = key%HASHTABLESIZE;
		return key;
}
/*
 * Collision Resolution handled by a linear probe decrementing
 */

int hashBoard(int newKey, BoardNode board)	{
	HashNode currNode = getHashTable(NULL)->table[newKey];
	int compFlag;
	if(getHashTable(NULL)->table[newKey] == NULL)	{
		getHashTable(NULL)->table[newKey] = addHashNode(board);	
		return 1;
	} else if(currNode->nextHNode == NULL)  {
		compFlag = compareTwoBoards(board,currNode->hashedBoard);
	} else{ 
		while(currNode->nextHNode != NULL && (compFlag = compareTwoBoards(board,currNode->hashedBoard)))	{
			currNode = currNode->nextHNode;
		}
	}

	if(compFlag)	{
		currNode->nextHNode=addHashNode(board);
		return 1;
	}
	free(board);
	return 0;
}

void printEmptyKeys()	{

		int i;
	for(i = 0; i < HASHTABLESIZE; i++)	{
		if(getHashTable(NULL)->table[i] == NULL){
			printf("%d\n",i);
		}

	}
}
