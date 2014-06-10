#include <graphics.h>
#include <dos.h>
#include <bios.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "picture.c"

#define length 127
#define lengthWF 142
#define PY 124
#define CL 48

int pa[2]={206,692};    //角度条的初始位置
int ps[2]={32, 862};    //力量条的初始位置
int pw=513;             //风力初始位置
int banana[24][81][81];
int carrot[24][81][81];
int rabbit[161][161];
int monkey[161][161];
int background[1024][768];
int background1[1024][768];
int check[1024][768];

 void draw_angleleft(int angle, int flag){
     int i ,j;
     double percentage;
     percentage=(double)angle/90;

     if (flag==1)
          for (j=0; j<=CL; j++)   
 			for(i=length*(percentage-0.02); i<=length*percentage; i++) {if (i>=0) putpixel(pa[0]+i, PY+j, 16776960);}
     else  for (j=0; j<=CL; j++)    
 			for(i=length*percentage; i>=length*(percentage-0.02); i--) {if (i>=0) putpixel(pa[0]+i, PY+j, BLACK);}
 }

 void draw_angleright(int angle, int flag){
     int i ,j;
     double percentage;
     percentage=(double)angle/90;

     if (flag==1)
         for (j=0; j<=CL; j++)     
 			for(i=length*(percentage-0.02); i<=length*percentage; i++) {if (i>=0) putpixel(pa[1]+i, PY+j, 16711680);}
     else  for (j=0; j<=CL; j++)  
 			for(i=length*percentage; i>=length*(percentage-0.02); i--) {if (i>=0) putpixel(pa[1]+i, PY+j, BLACK);}
 }

 void draw_strengthL(int angle,int flag){
     int i ,j;
     double percentage;
     percentage=(double)angle/90;

     if (flag==1)
          for (j=0; j<=CL; j++)     
 			for(i=length*(percentage-0.02); i<=length*percentage; i++) {if (i>=0) putpixel(ps[0]+i, PY+j, 16776960);}
     else  for (j=0; j<=CL; j++)    
			for(i=length*percentage; i>=length*(percentage-0.02); i--) {if (i>=0) putpixel(ps[0]+i, PY+j, BLACK);}
 }

 void draw_strengthR(int angle,int flag){
     int i ,j;
     double percentage;
     percentage=(double)angle/90;

     if (flag==1)
          for (j=0; j<=CL; j++)     
 			for(i=length*(percentage-0.02); i<=length*percentage; i++) {if (i>=0) putpixel(ps[1]+i, PY+j, 16711680);}
     else  for (j=0; j<=CL; j++)   
 			for(i=length*percentage; i>=length*(percentage-0.02); i--) {if (i>=0) putpixel(ps[1]+i, PY+j, BLACK);}
 }

 void draw_WF(int angle,int flag){
     int i ,j;
     double percentage;
     percentage=(double)angle/90;

     if (flag==1)
         for (i=pw+lengthWF*percentage-1; i<=pw+lengthWF*percentage; i++)
             for (j=0; j<=CL; j++)     putpixel(i, PY+j, 11540255);
     else  
         for (i=pw+lengthWF*percentage; i>=pw+lengthWF*percentage-1; i--)
             for (j=0; j<=CL; j++)    putpixel(i, PY+j, BLACK);
 }

 int set_angleleft(int first){
     static int angle=0,i,j,flag=1;
     if (first==0) {
         angle = 0 ;
         flag = 1 ;
         for (i=pa[0]; i<=pa[0]+length; i++)
             for (j=0; j<=CL; j++)     putpixel(i, PY+j, BLACK);
     }
     while (!bioskey(1)){
         angle+=flag;
         if (angle==89)  flag=-1;
         if (angle==0 )  flag=1;
         draw_angleleft(angle,flag);
         for (i=0;i<80000;i++)delay(0.99999999999999994);
     }
     return angle;
 }

 int set_angleright(int first){
     static int angle=0,i,j,flag=1;
     if (first==0) {
         for (i=pa[1]; i<=pa[1]+length; i++)
             for (j=0; j<=CL; j++)     putpixel(i, PY+j, BLACK);
         angle = 0;
         flag = 1 ;
     }
     while (!bioskey(1)){
         angle+=flag;
         if (angle==89)  flag=-1;
         if (angle==0 )  flag=1;
         draw_angleright(angle,flag);
         for (i=0;i<80000;i++)delay(0.99999999999999994);
     }
     return angle;
 }

  int set_strengthleft(int first){
     static int angle=0,i,j,flag=1;
     if (first==0) {
         angle = 0 ;
         flag = 1 ;
         for (i=ps[0]; i<=ps[0]+length; i++)
             for (j=0; j<=CL; j++)     putpixel(i, PY+j, BLACK);
     }
     while (!bioskey(1)){
         angle+=flag;
         if (angle==89)  flag=-1;
         if (angle==0 )  flag=1;
         draw_strengthL(angle,flag);
         for (i=0;i<80000;i++)delay(0.99999999999999994);
      }
     return angle;
 }

 int set_strengthright(int first){
     static int angle=0,i,j,flag=1;
     if (first==0) {
         angle = 0;
         flag = 1 ;
         for (i=ps[1]; i<=ps[1]+length; i++)
             for (j=0; j<=CL; j++)     putpixel(i, PY+j, BLACK);
     }
     while (!bioskey(1)){
         angle+=flag;
         if (angle==89)  flag=-1;
         if (angle==0 )  flag=1;
         draw_strengthR(angle,flag);
         for (i=0;i<80000;i++)delay(0.99999999999999994);
     }
     return angle;
 }

 void set_wind(int angle){
     int i ,j;
     double percentage;
     percentage=(double)angle/100;
        
     for (i=pw-lengthWF; i<=pw+lengthWF; i++)
         for (j=0; j<=CL+1; j++)     putpixel(i, PY+j, BLACK);
    
     if (angle>0){
         for (i=pw; i<=pw+lengthWF*percentage; i++)
             for (j=0; j<=CL+2; j++){
                 putpixel(i, PY+j,7667712);
                 delay(0.99999);
             }
     } else{
         for (i=pw; i>=pw+lengthWF*percentage; i--)
             for (j=0; j<=CL+2; j++){
                 putpixel(i, PY+j,7667712);
                 delay(0.99999);
             }
     }
 }

void draw_player(int m_x, int m_y, int r_x, int r_y){
 int i, j, k;
    
    getphoto();
    for (i=0; i<=m_x; i++){
        j=((double)m_y/(double)m_x)*i; 
        k=((double)m_y/(double)m_x)*(i-1); 
        D1(i, j);
        D3(i, k, j);
    }
    for (i=1024; i>=r_x; i--){
        j=((double)r_y/(1024-(double)r_x))*(1024-i);
        k=((double)r_y/(1024-(double)r_x))*(1024-i+1);
        D2(i, j);
        D3(i, k, j);
    }
    
    return;
}


 void input(){
 	FILE *fin;
 	int i, j, k;

 	fin=fopen("fruit.txt","r");
 	for (i=0; i<24; i++){
 		for (j=0; j<81; j++)
 			for (k=0; k<81; k++) fscanf(fin, "%d ", &banana[i][j][k]);
 		fscanf(fin, "\n");
 	}
 	for (i=0; i<24; i++){
 		for (j=0; j<81; j++)
 			for (k=0; k<81; k++) fscanf(fin, "%d ", &carrot[i][j][k]);
 		fscanf(fin, "\n");
 	}		
	for (i=0; i<=160; i++)
		for (j=0; j<=160; j++)
			fscanf(fin, "%d ", &monkey[i][j]);
	for (i=0; i<=160; i++)
		for (j=0; j<=160; j++)
			fscanf(fin, "%d ", &rabbit[i][j]);
 	fclose(fin);
	
 	return;
 }

 
void D1(int m_x, int m_y){
 int i, j;
    
    m_y=m_y;
    for (i=0; i<=160; i++)
        for (j=0; j<=160; j++){
            if (monkey[i][160-j]) paint(i+m_x-80, j+m_y-80, monkey[i][160-j]);
                else paint(i+m_x-80, j+m_y-80, background[i+m_x-80][j+m_y-80]);
        }
    
    return;
}

void D2(int r_x, int r_y){
 int i, j;
    
    r_y=r_y;
    for (i=0; i<=160; i++)
        for (j=0; j<=160; j++){
            if (rabbit[i][160-j]) paint(i+r_x-80, j+r_y-80, rabbit[i][160-j]);
                else paint(i+r_x-80, j+r_y-80, background[i+r_x-80][j+r_y-80]);
        }
    
    return;
}

int D3(int x, int k, int y)
{
    int i,j;
    for (i=-80; i<=80; i++)
    {
        paint(x-80, y+i, background[x-80][y+i]);
    }
    if (k<y) {
        for (i=-80; i<=80; i++)
            for (j=k-80; j<=y-80; j++)
                paint(x+i, j, background[x+i][j]);
    } else {
        for (i=-80; i<=80; i++)
            for (j=y+80; j<=k+80; j++)
                paint(x+i, j, background[x+i][j]);
    }
    return 0;
}

void getbackground()
{
	int i, j;
	for (i=1; i<=1023; i++)
		for (j=1; j<=767; j++)
			background1[i][768-j]=getpixel(i, j);
}

void collision_m(int m_x, int m_y){
	int i, j, k, r;
	double x, y, x0, y0;
	double D=0.2617993877;
	
	for (k=1; k<=3; k++){
		for (i=0; i<=160; i++)
			for (j=0; j<=160; j++){
				x=i-80; y=j-80;
				for (r=0; r<k; r++){
					x0=x; y0=y;
					x=x0*cos(D)-y0*sin(D);
					y=x0*sin(D)+y0*cos(D);
				}
				paint((int)x0+m_x, (int)y0+m_y, background1[(int)x0+m_x][(int)y0+m_y]);
			}	
		for (i=0; i<=160; i++)		
			for (j=0; j<=160; j++){
				x=i-80; y=j-80;
				for (r=0; r<k; r++){
					x0=x; y0=y;
					x=x0*cos(D)-y0*sin(D);
					y=x0*sin(D)+y0*cos(D);
				}
				if (monkey[i][160-j]) paint((int)x+m_x, (int)y+m_y, monkey[i][160-j]);
					else paint((int)x+m_x, (int)y+m_y, background1[(int)x+m_x][(int)y+m_y]);
			}
		delay(1);
	}
	return;
}

void collision_r(int m_x, int m_y){
	int i, j, k, r;
	double x, y, x0, y0;
	double D=-0.2617993877;
	
	for (k=1; k<=3; k++){
		for (i=0; i<=160; i++)
			for (j=0; j<=160; j++){
				x=i-80; y=j-80;
				for (r=0; r<k; r++){
					x0=x; y0=y;
					x=x0*cos(D)-y0*sin(D);
					y=x0*sin(D)+y0*cos(D);
				}
				paint((int)x0+m_x, (int)y0+m_y, background1[(int)x0+m_x][(int)y0+m_y]);
			}	
		for (i=0; i<=160; i++)		
			for (j=0; j<=160; j++){
				x=i-80; y=j-80;
				for (r=0; r<k; r++){
					x0=x; y0=y;
					x=x0*cos(D)-y0*sin(D);
					y=x0*sin(D)+y0*cos(D);
				}
				if (rabbit[i][160-j]) paint((int)x+m_x, (int)y+m_y, rabbit[i][160-j]);
					else paint((int)x+m_x, (int)y+m_y, background1[(int)x+m_x][(int)y+m_y]);
			}
		delay(10);
	}
	return;
}
