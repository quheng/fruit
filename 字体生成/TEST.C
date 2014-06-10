#include "picture.c"
#include "stdio.h"

int main(){
	int i,j;
	for (i = 0; i < 64; ++i)
		{
			for (j = 0; j < 64; ++j)
			{
				if (play1[i][j]>1) printf("%d",1);
					else printf("0");
			}
			printf("\n");
		}
	getchar();
	
} 
