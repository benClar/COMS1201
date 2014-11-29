/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

/*----------TypeDefs-----------*/

typedef struct chamber *Chamber;
typedef struct chStack *ChStack;
typedef struct chCoords *ChCoords;

/*----------Function Prototypes-----------*/
void addChamber(ChStack chamberStack);
void popChamber(ChStack chamberStack,
    int *topLeftX, int *topLeftY,
    int *topRightX, int *topRightY,
    int *botLeftX, int *botLeftY,
    int *botRightX, int *botRightY);
void setTopLeft(ChStack chamberStack, int row, int col);
void setBotLeft(ChStack chamberStack, int row, int col);
void setBotRight(ChStack chamberStack, int row, int col);
void setTopRight(ChStack chamberStack, int row, int col);
ChStack createChamberStack();
