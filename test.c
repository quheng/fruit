#include<io.h>
#include<dos.h>
#include<conio.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<alloc.h>






#include"timer.h"
#define n1 20
#define n2 100
struct WavHead
{
char riff[4];
long size0;
char wavefmt[8];
long size1;
int fmttag;
int channel;
long samplespersec;
long bytespersec;
int blockalign;
int bitspersample;
char flg[4];
}whead;
unsigned Port=0x210;
char Found=0;
unsigned cnt1,cnt2;
void PortReset();
void outwave(un

signed char huge *,long);
void WritePortC(unsigned char);
void errexit(char *);
void main()
{
int fp;
unsigned n,r,nn,i,j;
char name[32];
long fermem,rr,datasize;
unsigned char huge *data,huge *p;
if(argc<2)errexit("miss file name\n");
strcpy(name,argv[1]);strcat(name,".wav");
fp=-open(name,0-RDONLY);if(fp=-1)errexit("Error open fil
e\n");
-read(fp,&whead,sizeof(WavHead));
if(whead.blockalign=1 && strncmp(whead.flg,"data",4)==0)
{
-read(fp,&datasize,4);//单声道WAV数据
}
else if(whead.blockalign=2 && strncmp(whead.flg,"fact"
,4)==0)
{
lseek(fp,12l,1);
-read(fp,&datasize,4);//双声道WAV数据
}
else errexit("Error file struct\n");
farmem=farcoreleft();
PortReset();//初始化DSP端口
Counter=0;//开始计时
SetTimer(NewTimer,44100);//调整时间中断频率
WritePortC(0xd1);//接通喇叭
if(farmem≥datasize)//数据量不超过内存容量
{
p=data=(unsigned char huge *)farmalloc(datasize);
n=datasize/32768;r=datasize%32768;
for(i=0;i<n;i++,p+=32768) -read(fp,p,32768);
-read(fp,p,r);
outwave(data,datasize);
}