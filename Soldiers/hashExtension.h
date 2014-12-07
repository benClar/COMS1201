#ifndef hashExtension_h
#define hashExtension_h

#include "boardModule.h"

/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define MINKEYSIZE	0
#define HASHTABLESIZE 10000000

/*----------Typedefs----------*/
typedef struct zobrist *Zobrist;
typedef struct zobristVal *ZobristVal;
typedef struct hashTable *HashTable;
typedef struct hashNode *HashNode;



/*----------Function Prototypes-----------*/
char *int2bin(int a, char *buffer, int buf_size);
void initZobrist();
Zobrist getZValues(Zobrist zValues);
HashNode addHashNode(BoardNode uniqueBoard);
int generateHashKey(BoardNode boardToHash);
int validateKeys(ZobristVal **array, int dRndm, int aRndm);
void createHashTable();
HashTable getHashTable(HashTable currTable);
int hashBoard(int key, BoardNode board);
void printEmptyKeys();
#endif
