//! HashExtension.c
//
// This module sits slots in between making moves & changing the boards.  
//
// It takes two approaches for hashing.  The first is using the Zobrist function, 
// which is utilised for other board based games such as chess.  It
// Uses a set of hash keys that are randomly generated at the start to
// assign values to each possibility on each square of the board (
// in this programs case, that there is a dead or an alive button).  
// When a board is presented, it is compared against this set of values
// for each element.  Each value is x-ored together.  This allows the Xor
// to be done again for a value to quickly achieve the inverse moves value.
// E.g. xOr the zobrist hash value of a button being IN element 1,7.  xOr
// this value again to get the hash value of this removed.
//
// The second approach uses the bit ID generated from the bit encoding module 
// get a hash value for the hash table.
//
// Both of my hash methods work concurrently with the linear queue, their nodes made up
// of pointers to this queue.  The queue stores the boards, whilst the hashes are used to
// search for unique boards.

///*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"
#include "hashExtension.h"

/*---------- Symbolic Constants  -----------*/

//! zobrist hashing structures

//!Holds table that holds a randomised integer for each square on grid
struct zobrist	{

	ZobristVal **hashKey;	
	
};

//! Elements of zobrist value table
struct zobristVal	{
	int alive;
	int dead;
};

//!Hash table that holds zobrist nodes pointing to unique boards
struct hashTable	{

	HashNode *table;

};

//!Poinys to unique boards in queue
struct hashNode	{

	BoardNode hashedBoard;
	HashNode nextHNode;

};

//! Bit Hashing structures
//!Holds the bit hash table
struct bitHashTable	{

	BitHashNode *bitTable;

};

//!Nodes that populate the bit hash table
struct bitHashNode	{

	BitHashNode next;
	BoardNode hashedBoard;
};

/*---------- Functions ----------*/
/*
 *Creates bit hash table
 */
void createBitHashTable()	{

	BitHashTable newBitTable = (BitHashTable) checkMalloc(malloc(sizeof(*newBitTable)));
	newBitTable->bitTable = (BitHashNode*) checkMalloc(calloc(HASHTABLESIZE, sizeof(*(newBitTable->bitTable))));
	getBTable(newBitTable);
}

/*
 *Return memory address of bit hash table
 */
BitHashTable getBTable(BitHashTable table)	{

	static BitHashTable cTable;

	if(table != NULL)	{
		cTable = table;	
	}

	return cTable;
}

/*
 *Adds node to bit hash table
 */
BitHashNode addBitNode()	{
	BitHashNode newNode = (BitHashNode) checkMalloc(malloc(sizeof(*newNode)));
	newNode->next = NULL;
	return newNode;	
}

/*
 *Create Zobrist hash table
 */
void createHashTable()	{

	HashTable newTable = (HashTable) checkMalloc(malloc(sizeof(*newTable)));
	newTable->table = (HashNode*) checkMalloc(calloc(HASHTABLESIZE, sizeof(*(newTable->table))));
	getHashTable(newTable);

}

/*
 * Add Zobrist Hash node to table
 */
HashNode addHashNode(BoardNode uniqueBoard)	{

	HashNode HNode = (HashNode) checkMalloc(malloc(sizeof(*HNode)));
	HNode->hashedBoard = uniqueBoard;
	HNode->nextHNode = NULL;	
	return HNode;
}

/*
 *Get Zobrist Hash table address
 */
HashTable getHashTable(HashTable currTable)	{

	static HashTable hTable;

	if(currTable != NULL)	{
		hTable = currTable;
	}

	return hTable;

}

/*
 *Selects which hash structures to free based on program mode
 */
void freeHashingStructures()	{

	if(getMode() == BHASH)	{
		freeBHashingStructures();
	} else if (getMode() == ZHASH)	{
		freeZHashingStructures();
	} else	{
		fprintf(stderr,"Hashing mode is not enabled.  Should not be trying to free hash structures\n");
	}

}

/*
 *Free all structures for Bit Hashing
 */
void freeBHashingStructures()	{
	int row;
	BitHashTable btable = getBTable(NULL);	
	BitHashNode bNode, temp;
	for(row = 0, bNode = btable->bitTable[row]; row < HASHTABLESIZE; bNode = btable->bitTable[++row])	{
		if(bNode != NULL)	{
			while(bNode->next != NULL)	{
				temp = bNode->next;
				free(bNode);
				bNode = temp;	
			}
		}

	}

	free(btable);	

}

/*
 * Free all structures used for Zobrist Hashing
 */
void freeZHashingStructures()	{

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

/*
 *Creates the Zobrist Hash Keys
 */
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

/*
 *Get Zobrist values table memory address
 */
Zobrist getZValues(Zobrist zValues)	{

	static Zobrist currZValues;
	if(zValues != NULL)	{
		currZValues = zValues;
	}

	return currZValues;
}

/*
 *Valids randomly generated Zobrist hash values
 */
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

/*
 *Generate hash key from BitID
 */
int generateBitHashKey(BoardNode boardToHash)	{
	int key;
	key = getBitID(boardToHash)%HASHTABLESIZE;
	return addToBitHTable(key,addBitNode(),boardToHash);
}

/*
 *adds board to bit hash table if it is unique.  Uses equality operators for quick comparison.
 */
int addToBitHTable(int key,BitHashNode newNode, BoardNode board)	{
	newNode->hashedBoard = board;
	BitHashTable bTab = getBTable(NULL); 
	BitHashNode currNode = bTab->bitTable[key];
	if(currNode == NULL)	{
		bTab->bitTable[key]	= newNode;
		return 1;
	} else if(currNode->next == NULL) {
		if(getBitID(newNode->hashedBoard) == getBitID(currNode->hashedBoard))	{
			free(newNode);
			return 0;
		}
	} else {
		while(currNode->next != NULL)	{
			if(getBitID(newNode->hashedBoard) == getBitID(currNode->hashedBoard))	{
				free(newNode);
				return 0;
			}
			currNode = currNode->next;	
		}
	}
	currNode->next = newNode;
	return 1;
}

/*
 *Generate Zobrist hash key using Xor bitwise operator
 */
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
				key = key^(getZValues(NULL)->hashKey[row][col]->alive);
			} if(!getButtonStatus(boardToHash,row,col)){
				key = key^(getZValues(NULL)->hashKey[row][col]->dead);
			}
		}
	}
		
	key = key%HASHTABLESIZE;
	return key;
}


/*
 * Adds board to Zobrist hash table if it does not already exist.  Manually compares boards.
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

/*
 *Debug: Print any unused hash elements
 */
void printEmptyKeys()	{

		int i;
	for(i = 0; i < HASHTABLESIZE; i++)	{
		if(getHashTable(NULL)->table[i] == NULL){
			printf("%d\n",i);
		}

	}
}
