#ifndef testingModule_h
#define testingModule_h
#include "dataParsing.h"
#include "moveModule.h"
#include "boardModule.h"

/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define TRUE        1       //!For testing.  Expecting true value.
#define FALSE       0       //!For testing.  Expecting False value.
#define MOVETYPECOL 2       //! this element of test move array is a move, not a coordinate
#define TESTDATACOL 3       //! Number of columns in test data file
#define CURRROW     0       //! Column that stores test row
#define CURRCOL     1       //! Columns that stores test col
#define MOVETYPE    2       //! Column that stores move type



/*----------Typedefs-----------*/
typedef struct testData *TestData;

/*----------Structures----------*/



/*----------Function Prototypes-----------*/
void testing();
void testVal(int testResult,int expectedResult, char *description);
void startTest();
TestData createTestDataStr();
TestData getTestData(TestData tData);
void setTestDataArr(int row, int col, int move);
int parsingMoveType(char type, int *moveRow, int *moveCol);
int testMakeMove(BoardNode testBoard);
void growMoveDataArray(TestData testStr, int row);
void readMoveList(TestData tData);

#endif
