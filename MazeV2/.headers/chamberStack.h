/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

/*----------TypeDefs-----------*/

typedef struct chamber *Chamber;
typedef struct chStack *ChStack;

/*----------Function Prototypes-----------*/
void setTop(ChStack chamberStack, int top);
void setLeft(ChStack chamberStack, int left);
void setBottom(ChStack chamberStack, int bottom);
void setRight(ChStack chamberStack, int right);
void popChamber(ChStack chamberStack, int *top, int *bottom, int *left, int *right);
void addChamber(ChStack chamberStack);
ChStack createChamberStack();
