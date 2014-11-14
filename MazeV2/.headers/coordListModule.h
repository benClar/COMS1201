/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/


/*----------Typedefs----------*/

typedef struct pathList *PathList;
typedef struct pathNode *PathNode;

/*----------Function Prototypes-----------*/
PathList createList();
void addNode(PathList list, int addRow, int addCol);
void cleanList(MazeMap maze, PathList list);
void nextNode(PathList list);
void setCurr(PathList list, PathNode node);
void setStart(PathList list, PathNode node);
int getCurrCol(PathList list);
int getCurrRow(PathList list);
PathNode getCurr(PathList list);
PathNode getStart(PathList list);
