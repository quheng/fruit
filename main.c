#include <graphics.h>
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "key.c"
#include "sound.c"
#include "initial.c"
#include "picture.c"


void firstwin(){
	int i,j,k;
	for (i = 0; i < 64; ++i)
		{
			for (j = 0; j <256;++j)
			{
				if (player1[i][j]!=0)
				putpixel(j+180,i+170,player1[i][j]-4);
			}
		}
}

void secondwin(){
	int i,j,k;
	for (i = 0; i < 64; ++i)
		{
			for (j = 0; j < 256; ++j)
			{
				if (player2[i][j]!=0)
				putpixel(j+180,i+170,player2[i][j]-6);
			}
		}
}



int main(int argc, char const *argv[]){
	int winer=0,i,j;
	printf("1233");
	initial();
	while (!winer){
		set_angleleft();
		getchar();
		firstwin();
		set_angleright();
		secondwin();
		winer =1 ;
	}
	return (0);
}