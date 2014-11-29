/*----------Enumerated Types-----------*/

typedef enum { ALIVE, DEAD } buttonState;

/*----------Symbolic Constants-----------*/

#define MAXCOL      7   //!width of board
#define MAXROW      8   //!height of board
#define MINCOORD    0   //!smallest coordinate allowed

/*----------Typedefs----------*/

typedef struct boardQueueHead *BoardQueueHead;
typedef struct boardNode *BoardNode;
typedef struct button *Button;

/*----------Function Prototypes-----------*/
void createQueue();
BoardQueueHead getQueue(BoardQueueHead newQueue);
void setTargetMove(int tRow, int tCol);
void testTargets();
void *checkMalloc(void *malP);
int incrementID();
void addBoard(int PID, Button **boardtoCopy);
void populateBoard(BoardNode newBoard, int PID, Button **boardToCopy);
