/*---------- Standard Headers -----------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*---------- Custom Headers	-----------*/

#include ".headers/debug.h"

/*---------- Symbolic Constants  -----------*/


/*---------- Main -----------*/
int isPrime(int num);
void bobsworld(char *arrayOne, char *arrayTwo, int sizeOne, int sizeTwo);
int testVowel(char letter, char *vowel, int size);
int main(){

	char alpha[]="abcdefghijklmnopqrstuvwxyz";
	char vowel[]="aeiou";
	
	bobsworld(alpha,vowel,26,5);
	return 0;

}

/*---------- Functions ----------*/

int isPrime(int num)	{
	int i;
	for (i=2; i < num; i++)	{
		if(num%i==0)	{
			return 0;
		}
	}
	return 1;

}

void bobsworld(char *arrayOne, char *arrayTwo, int sizeOne, int sizeTwo)	{

	int one, two, three;

	for (one =0; one <sizeOne; one++)	{
		for(two =0; two < sizeOne; two++)	{
			for(three = 0; three < sizeOne; three++)	{
				if (isPrime((one+1) + (two+1) + (three+1)) && arrayOne[one] == arrayOne[three])	{
					if(testVowel(arrayOne[one],arrayTwo,5) && testVowel(arrayOne[three],arrayTwo,5))	{
						if(!testVowel(arrayOne[two],arrayTwo,5))	{
						printf("%c %c %c\n",arrayOne[one],arrayOne[two],arrayOne[three]);
						}
					} else if (!testVowel(arrayOne[one],arrayTwo,5) && !testVowel(arrayOne[three],arrayTwo,5)) 	{
						if(testVowel(arrayOne[two],arrayTwo,5))   {
						printf("%c %c %c\n",arrayOne[one],arrayOne[two],arrayOne[three]);
						}
					}
				}
			}
		} 
	}
}

int testVowel(char letter, char *vowel, int size)	{

	int i;
	for (i = 0; i < size; i++)	{
		if(letter == vowel[i])	{
			return 1;
		}
	}
	return 0;	
}


