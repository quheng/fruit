#include <graphics.h>
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "key.c"
#include "sound.c"
#include "initial.c"
#include "throw.c"
#include "music.c"
#include "ini256.c"

void firstwin(){
	int i,j,k;
	for (i = 0; i < 64; ++i)
		{
			for (j = 0; j <256;++j)
			{
				if (player1[i][j]!=0)
				putpixel(j+400,i+320,16776960);
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
				putpixel(j+400,i+320,16711680);
			}
		}
}



int main( ){
	int winer ,first=0,wind,c;
	int angle,strength,lx,ly,rx,ry,dx,dy;
	char loop='y';
	int kk;
	setbgm();
	input();
	while (loop=='y') {
		initial("bg.bmp"); getbackground();
		lx = (1234 * rand()% 99 ) % 200 +70;
		ly = (1234 * rand()% 99 ) % 300 +170;
		rx = (1234 * rand()% 99 ) % 200 +730;
		ry = (1234 * rand()% 99 ) % 300 +250;
		draw_player (lx,ly,rx,ry);
		first = 0;
		winer = 0; 
		while (winer == 0){
			srand( (unsigned)time( NULL ) );  
			wind = rand()%100-90;
			set_wind(wind);

			
			angle = set_angleleft(first);
			while((c = getchar()) != '\n' && c != EOF);
			strength = set_strengthleft(first);
			while((c = getchar()) != '\n' && c != EOF);

			setth();
			if ( throw(angle,wind,strength,lx,ly,rx,ry,0) ){
				winer = 1;
				setaim();
				collision_r(rx, ry);
				delay(100);
				break;
			}
			setbgm();

			angle = set_angleright(first);
			while((c = getchar()) != '\n' && c != EOF);
			strength = set_strengthright(first);
			while((c = getchar()) != '\n' && c != EOF);

			setth();
			if ( throw(angle,wind,strength,lx,ly,rx,ry,1) ){
				winer = 2;
				setaim();
				collision_m(lx, ly);
				delay(100);
				break;
			}
			setbgm();
			first = 1;
		}
		setbgm();
		if (winer==1) firstwin();
		else secondwin();

		while (!bioskey(0)) printf("%d\n", bioskey(0));;
		initial("out.bmp");
		loop=getchar();
		while (loop!='y'&& loop!='n') 	{ printf("Please input again!\n");loop=getchar();}
		getchar();
	}
	nosound();
	system("cls");
	return (0);
}
