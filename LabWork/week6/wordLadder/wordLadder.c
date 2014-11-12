//
// wordLadder.c
// Created by bclarke on 201WORDSIZE/05/11
// tags: 
//
/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"
#include ".headers/wordLadder.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Main -----------*/
FILE* openfile(char *input);
Node AllocateNode(char *name);
int** malArray(int width, int height);
int permutationsCalc(int num);
void numbersCalc(int **array, int range,int size);
int checkWord(Node start, char *wStart, char *wEnd, int **permutations,int row, int col);
int searchWord(Node start, char*word);

int main(int argc, char *argv[]){
	argc--;
	Node start = NULL, current = NULL;

	FILE *wordsF = openfile(".text/words.txt");
	char word[5];
	int success = 0;
	int count = 0, row = 0;
	int captured = 0, i = 0;
	while(fscanf(wordsF,"%s",word) != EOF)	{
		if (!count)	{
			start = current = AllocateNode(word);
			count++;
		}
		current->next = AllocateNode(word); //pointing next to next structure
		current = current->next;	//setting current to next structure
	}
	int **perms = malArray(WORDSIZE,permutationsCalc(WORDSIZE));
	numbersCalc(perms,permutationsCalc(WORDSIZE),WORDSIZE);
	
	for (row = 0; row < 24  && success == 0; row++)	{	
		if((captured = checkWord(start,argv[1],argv[2],perms,row,0)))	{
				success = 1;
				printf("%s\n",argv[1]);
			for(i = 0; i < 4; i++)	{
				argv[1][perms[captured][i] -1] = argv[2][perms[captured][i] -1];
				printf("%s %d\n",argv[1],perms[captured][i]);	
			}
		}
	}
	return 0;

}

/*---------- Functions ----------*/

int checkWord(Node start, char *wStart, char *wEnd, int **permutations,int row, int col)	{
	char seaWord[5];
	strcpy(seaWord,wStart);
	seaWord[(permutations[row][col])-1] = wEnd[(permutations[row][col])-1];
	if(searchWord(start,seaWord))   {
		if (col == 3 ) { return row; }
		return checkWord(start, seaWord, wEnd, permutations, row, ++col);
    } 
	return 0;
}

int searchWord(Node start, char *word)	{
		
	Node current = start;
	while(current->next!=NULL)	{
		if(!strcmp(word,current->name))	{
			return 1;
		}
		current = current->next;
	}

	return 0;
}

FILE* openfile(char *input)	{
        FILE *fp;
        fp = fopen(input,"r");

        return fp;

}

Node AllocateNode(char *word)	{

	Node newNode = (Node) malloc(sizeof(Node*));

	if(newNode==NULL)	{
		printf("Cannot Allocate Node\n");
	}
	strcpy(newNode->name, word);
	newNode->next = NULL;
	
	return newNode;
}


int permutationsCalc(int num)	{

	if(num == 0)	{ return 1; }

	num = num * (permutationsCalc(num-1));
	return num;
}

void numbersCalc(int **array, int range,int size)	{
	
	int a, b, c, d;
	int	row, col;
	row = col = 0;
	for(a = 1; a <= size; a++)	{
		for(b = 1; b <= size; b++)	{
			for(c = 1; c <= size; c++)	{
				for(d = 1, col = 0; d <= size; d++ )	{
					if((a != b) && (a != c) && (a != d) && (b != c) && (b != d) && (c != d))	{
						array[row][col]=a;
						col++;
						array[row][col]=b;
						col++;
						array[row][col]=c;
						col++;
						array[row][col]=d;
						row++;
						col = 0;
					}
				}	
			}
		}
	}
}

int** malArray(int width, int height)	{
	int **array = (int**) malloc(height*sizeof(int*));
	int	i;

	for(i = 0; i < height; i++)	{
		array[i] = (int*) malloc(width*sizeof(int));
	}
	return array;
}
