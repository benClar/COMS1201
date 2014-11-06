/*----------Enumerated Types-----------*/


/*----------Symbolic Constants-----------*/

#define MAXNAME 20
#define WORDSIZE	4
/*----------Structures----------*/
struct node {
	char name[MAXNAME];
	struct node *next;
};

typedef struct node *Node;

/*----------Function Prototypes-----------*/

