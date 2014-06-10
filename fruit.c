#include <graphics.h>
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include "initial.c"
#include "music.c"


 int main( ){
 	initial("bg1.bmp");
 	getch();
 	initial("bg2.bmp");
 	getch();
	playwav("warning.wav",20);
	system("main.exe");
	return (0);
}