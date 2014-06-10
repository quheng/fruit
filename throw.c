#include <stdio.h>
#include <math.h>
#include <graphics.h>
#include <bios.h>
#include <conio.h>
#include <stdlib.h>

#define G 1.0

int paint(int x, int y, int color)
{
    if (((x>0)&&(y>0))&&((x<1024)&&(y<768))) putpixel(x, 768-y, color);
	return 0;
}

void CF(int x, int y){
	int i, j;
	
	for (i=-40; i<=40; i++)
		for (j=-40; j<=40; j++){
			paint(x+i, y+j, background[x+i][y+j]);
		}
	
	return;
}

int print (int x, int k, int y)
{
	int i,j;
	for (i=-40; i<=40; i++)
	{
		paint(x-40, y+i, background[x-40][y+i]);
	}
	if (k<y) {
		for (i=-40; i<=40; i++)
			for (j=k-40; j<=y-40; j++)
				paint(x+i, j, background[x+i][j]);
	} else {
		for (i=-40; i<=40; i++)
			for (j=y+40; j<=k+40; j++)
				paint(x+i, j, background[x+i][j]);
	}
	return 0;
}


int PF1(int x, int y, int r)
{
	int i, j, i0, j0;
	for (i=-40; i<=40; i++)
		for (j=-40; j<=40; j++){
			if (banana[r][i+40][40-j]!=0) paint(x+i, y+j, banana[r][i+40][40-j]);
			else paint(x+i, y+j, background[x+i][y+j]);
		}

	return 0;
}

int PF2(int x, int y, int r)
{
	int i, j, i0, j0;
	for (i=-40; i<=40; i++)
		for (j=-40; j<=40; j++){
			if (carrot[r][i+40][40-j]!=0) paint(x+i, y+j, carrot[r][i+40][40-j]);
			else paint(x+i, y+j, background[x+i][y+j]);
		}

	return 0;
}

int f(int x, double angle, double windforce, double  velocity){    
	double t;
	t=x/(velocity*cos(angle)-windforce);
	return (int)(velocity*sin(angle)*t-G*t*t);
}

int CC(int x ,int y){
	int i, j;
	
	for (i=-24; i<=24; i++)
		for (j=-24; j<=24; j++)
			if (((x+i>0)&&(y+j>0))&&((x+i<1024)&&(y+j<768)))
				if ((check[x+i][y+j])&&(i*i+j*j<24*24)) return 1;
	return 0;
}

void getphoto()
{
	int i, j;
	for (i=1; i<=1023; i++)
		for (j=1; j<=767; j++)
			background[i][768-j]=getpixel(i, j);
}

void AWS(double *A, double *W, double *S){
	*S=*S/2;
	*A=*A/180*3.141592653589;
	*W=*S*cos(*A)**W/100;
	return;
}

int throw(double angle, double windforce, double velocity, int xL, int yL, int xR, int yR, int flag){
	int i, j, k, d;
	int m_x, m_y, r_x, r_y;
	
	getphoto();	 AWS(&angle, &windforce, &velocity);
	m_x=xL; m_y=yL; r_x=xR; r_y=yR;
	m_y=768-m_y; r_y=768-r_y;
	for (i=0; i<1024; i++)
		for (j=0; j<768; j++) check[i][j]=0;
	for (i=0; i<=100; i++)
		for (j=0; j<=100; j++){
			if (monkey[i][j]) {
				check[i+m_x-50][768-(j+m_y-50)]=1; 
			}
			if (rabbit[i][j]) {
				check[i+r_x-50][768-(j+r_y-50)]=1;
			}
		}
	i=0; j=0; k=0;
	if (flag==0){
		for (i=0; i+xL<1024; i++){
			k=j; 
			j=f(i, angle, windforce, velocity);
			if (flag==0) PF1(i+xL, j+yL, i / 3 % 24);
			else PF2(i+xL, j+yL, i / 3 % 24);
			print(i+xL, k+yL, j+yL);
			if (CC(i+xL, j+yL) && (i+xL>500)){
				CF(i+xL, j+yL);
				return 1;
			}
		}
		CF(i+xL, j+yL);		
	} else{
		for (i=0; xR-i>0; i++){
			k=j;
			j=f(i, angle, windforce, velocity);
			if (flag==1) PF2(xR-i, j+yR, (1000-i) / 3 % 24);
			else  PF1(xR-i, j+yR, (1000-i) / 3 % 24);
			print(xR-i, k+yR, j+yR);
			if (CC(xR-i, j+yR) && (xR-i<560)){
				CF(xR-i, j+yR);	
				return 1;	
			}
		}
		CF(xR-i, j+yR);
	}
	
	return 0;
}
