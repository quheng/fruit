#include <graphics.h>
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define length 79
#define LEFT 0
#define RIGHT 1



int p[2]={128,432};

void draw_angleleft(int angle,int flag){
    int i ,j;
    double percentage;
    percentage=(double)angle/90;

    if (flag==1)
         for (j=0; j<=29; j++)     putpixel(p[0]+length*percentage, 79+j, 251);
    else  for (j=0; j<=29; j++)    putpixel(p[0]+length*percentage, 79+j, BLACK);
}

void draw_angleright(int angle, int flag){
    int i ,j;
    double percentage;
    percentage=(double)angle/90;

    if (flag==1)
         for (j=0; j<=29; j++)     putpixel(p[1]+length*percentage, 79+j, 249);
    else  for (j=0; j<=29; j++)    putpixel(p[1]+length*percentage, 79+j, BLACK);
}

int set_angleleft(){
    int angle=0,i,j,flag=1;
    for (i=0; i<=length; i++)
        for (j=0; j<=29; j++)  putpixel(p[0]+i, 79+j, BLACK);
     while (!bioskey(1)){
        angle+=flag;
        if (angle==89)  flag=-1;
        if (angle==0 )  flag=1;
        draw_angleleft(angle,flag);
        for (i=0;i<80000;i++)delay(0.99999999999999994);
     }
     return angle;
}

int set_angleright(){
    int angle=0,i,j,flag=1;
    for (i=0; i<=length; i++)
        for (j=0; j<=29; j++)  putpixel(p[0]+i, 79+j, BLACK);
     while (!bioskey(1)){
    	angle+=flag;
    	if (angle==89)  flag=-1;
        if (angle==0 )  flag=1;
    	draw_angleright(angle,flag);
    	for (i=0;i<80000;i++)delay(0.99999999999999994);
     }
     return angle;
}