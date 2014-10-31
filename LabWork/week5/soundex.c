/*---------- Standard Headers -----------*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include "debug.h"

/*---------- Symbolic Constants  -----------*/

#define LENGTH	4

void extraChar(int count);
void doRules(char **codes, char *name); 
void createCodes(char **codes, int size);
char** create2dArr(int size);
void printArr(char *arr, int size);
int scanNames(int argCnt, char **array, char **codes);
int searchCode(char **codes, char *letters, int letter,int size, int prevSoundCode);

/*---------- Main -----------*/


int main(int argc, char *argv[]){
	char **codes = create2dArr(10);
	
	createCodes(codes, 10);
	int argCnt = argc - 1;
	if (argCnt >= 1)	{
		scanNames(argCnt, argv, codes);
	}
	return 0;

}

/*---------- Functions ----------*/

int scanNames(int argCnt, char **array, char **codes)	{

	FILE *fp;
        fp = fopen(array[argCnt],"r");
        char name[10];
        while(fscanf(fp,"%s",name)!= EOF) {
                doRules(codes,name);
        }

	return 1;
}

void doRules(char **codes, char *name)	{

	int	count,i, soundCode, prevSoundCode = -1;
	for(i = 0, count = 0; count < LENGTH && name[i] != '\0'; i++)	{
		soundCode = searchCode(codes, name, i, 10, prevSoundCode);
		if (soundCode != prevSoundCode && soundCode >= 0 )	{
			if (count == 0)	{
				putchar(name[i]);
				count++;
			} else {
					switch (soundCode)	{
					case 1:
						printf("1");
						break;
					case 2:
						printf("2");
						break;
					case 3:
						printf("3");
						break;
					case 4:
						printf("4");
						break;
					case 5:
						printf("5");
						break;
					case 6:
						printf("6");
						break;
					default:
						break;
					}
					if (soundCode > 0)	{
						count++;
					}
			}
				prevSoundCode = soundCode;
		}
	}
	if (count < 4)	{
		extraChar(count);
	}

	pNL();
}

void extraChar(int count)	{
	int	extra = LENGTH - count;
	int	i;
	for (i= 0; i < extra; i++ ) {
		printf("0");	
	} 

}



int searchCode(char **codes, char *name, int letter,int size, int prevSoundCode)	{

	int	row, col = 1;
	for (row = 0; row < size; row++)	{
		for (col = 0; codes[row][col] != '\0'; col++)	{
			if (toupper(name[letter]) == codes[row][col])	{
				if (row != prevSoundCode)	{
				        if (toupper(name[letter - 1]) == 'H' || toupper(name[letter - 1] == 'W'))       {
						int recurseCode = searchCode(codes,name,letter-2,size,row);
						return recurseCode;
					}
					return row;
				}	else	{
					return -1;
				}
			}
		}
	}
	return -1;	
}	

void printArr(char *arr, int size)	{

	int	i;
	for (i =0; i < size; i++)	{
		printf("%c\n",arr[i]);
	}
}

char** create2dArr(int size)	{
	int	i;
	char **arr = (char**) malloc(size * sizeof(char*));
	for(i = 0; i< size; i++)	{
		arr[i] = (char*) malloc(size * sizeof(char));
	}
	return arr;
}

void createCodes(char **codes, int size)	{

        codes[0] = "AEHIOUWY";
        codes[1] = "BFPV";
        codes[2] = "CGJKQSXZ";
        codes[3] = "DT";
        codes[4] = "L";
        codes[5] = "MN";
        codes[6] = "R";

}
