#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#pragma pack(pop); 

struct WaveData {
unsigned long SoundLength;
unsigned int Frequency;
char *Sample;
};

struct HeaderType {
long         RIFF;      /*RIFF header*/
char         NI1 [18];  /*not important*/
unsigned int Channels;  /*channels 1 = mono; 2 = stereo*/
long         Frequency; /*sample frequency*/
char         NI2 [6];   /*not important*/
char         BitRes;    /*bit resolution 8/16 bit*/
char         NI3 [12];  /*not important*/
} Header;

struct WaveData Voice;         /*Pointer to wave file*/
unsigned int    Base;          /*Sound Blaster base address*/
char            WaveFile [25]; /*File name for the wave file to be played*/
long count,rest,i,j;

/****************************************************************************
** 利用屏幕垂直回扫计时                                                            **
****************************************************************************/
void DelayTime(float delaytime)
{
int action_flag = 0;
long count=0;
do
{
if ((inportb(0x3da)&0x08)!= 0 && action_flag == 0)
{
count++;
if (count >= delaytime)
break;
action_flag = 1;
}
else if ((inportb(0x3da)&0x08)== 0 && action_flag == 1)
action_flag = 0;
}while(1);
}

/****************************************************************************
** 检查声卡是否在给定的几个地址                                          **
** 存在返回1，不存在返回0                                                     **
****************************************************************************/
int ResetDSP(unsigned int Test)
{
/*Reset the DSP*/
outportb (Test + 0x6, 1);
delay(10);
outportb (Test + 0x6, 0);
delay(10);
/*Check if (reset was succesfull*/
if ((inportb(Test + 0xE) & 0x80 == 0x80) && (inportb(Test + 0xA) == 0xAA))
{
/*DSP was found*/
Base = Test;
return (1);
}
else
/*No DSP was found*/
return (0);
}
/****************************************************************************
** DSP播放声音                                                                          **
****************************************************************************/
void WriteDSP(unsigned char Value)
{
/*Wait for the DSP to be ready to accept data*/
while ((inportb(Base + 0xC) & 0x80) == 0x80);
/*Send byte*/
outportb (Base + 0xC, Value);
}
/****************************************************************************
** 将内存中的数据读出，放到DSP端口                                        **
****************************************************************************/
void PlayBack (struct WaveData *Wave)
{
long          LinearAddress;
unsigned int  Page, OffSet;
unsigned char TimeConstant;
TimeConstant = (65536 - (256000000 / Wave->Frequency)) >> 8;
WriteDSP(0x40);                  /*DSP-command 40h - Set sample frequency*/
WriteDSP(TimeConstant);          /*Write time constant*/
/*Convert pointer to linear address*/
LinearAddress = FP_SEG (Wave->Sample);
LinearAddress = (LinearAddress << 4) + FP_OFF (Wave->Sample);
Page = LinearAddress >> 16;      /*Calculate page*/
OffSet = LinearAddress & 0xFFFF; /*Calculate offset in the page*/
/*
Note - this procedure only works with DMA channel 1
*/
outportb (0x0A, 5);              /*Mask DMA channel 1*/
outportb (0x0C, 0);              /*Clear byte pointer*/
outportb (0x0B, 0x49);           /*Set mode*/
/*
The mode consists of the following:
0x49 = binary 01 00 10 01
|  |  |  |
|  |  |  +- DMA channel 01
|  |  +---- Read operation (the DSP reads from memory)
|  +------- Single cycle mode
+---------- Block mode
*/
outportb (0x02, OffSet & 0x100); /*Write the offset to the DMA controller*/
outportb (0x02, OffSet >> 8);
outportb (0x83, Page);           /*Write the page to the DMA controller*/
outportb (0x03, Wave->SoundLength & 0x100);
outportb (0x03, Wave->SoundLength >> 8);
outportb (0x0A, 1);              /*Unmask DMA channel*/
WriteDSP(0x14);                  /* DSP-command 14h - Single cycle playback*/
WriteDSP(Wave->SoundLength & 0xFF);
WriteDSP(Wave->SoundLength >> 8);
}
/****************************************************************************
** 将wavfile读入内存                                                                  **
** 检查wav文件是否标准                                                            **
****************************************************************************/
int LoadVoice (struct WaveData *Voice,FILE *WAVFile)
{

free(Voice->Sample);
Voice->Sample = (char *)malloc(Voice->SoundLength+2); /*Assign memory*/
if (!Voice->Sample)
{
	printf("!Voice->Sample\n");
exit(0);
}
if (i == 0)
{
/*Load the sample data*/
fread(&Header, 46, 1, WAVFile);
/*Check RIFF header*/
if (Header.RIFF != 0x46464952){
printf("Header.RIFF != 0x46464952");
exit(0);
}
Header.Channels = 1;
Header.BitRes = 8;
/*Check channels*/
if (Header.Channels != 1){
printf("Header.Channels :%d\n ",Header.Channels);

exit(0);
}
//Check bit resolution
if (Header.BitRes != 8){
printf("Header.BitRes %d \n", Header.BitRes);

exit(0);
}
Voice->Frequency = Header.Frequency;
/*Load the sample data*/
}
/*else fseek(WAVFile,i*(44100+2)+43,SEEK_SET);*/
fread(Voice->Sample, Voice->SoundLength + 2, 1, WAVFile);
return (1);
}

int playwav (char* Filename, float delaytime )
{
FILE *WAVFile;
if (ResetDSP (0x220))
{
/*at 220h*/
}
else
{
if (ResetDSP (0x240))
{
/*at 240h*/
} else
{
/*or none at all*/
exit(0);
}
}
/*If it can't be opened...*/
WAVFile = fopen(Filename, "rb");
if (WAVFile == NULL)
{
printf("openfile error\n");
exit(0);
}
/*Return length of file for sound length minus 48 bytes for .WAV header*/
fseek(WAVFile, 0L, SEEK_END);
Voice.SoundLength = ftell (WAVFile) - 48;
fseek(WAVFile, 0L, SEEK_SET);
/*Check RIFF header*/
if (Voice.SoundLength > 44100)
{
count = Voice.SoundLength / 44100;
rest = Voice.SoundLength % 44100;
if (rest != 0)
count++;
Voice.SoundLength = 44100;
}
else
{
count = 1;
rest = Voice.SoundLength;
}
/*Load wave file*/
for (i=0; i<count; i++)
{
if (i == count -1)
Voice.SoundLength = rest;
if (LoadVoice(&Voice,WAVFile))
{
/*Start playback*/
PlayBack (&Voice);
if (i != count -1)
DelayTime(44100.0/Voice.Frequency*60+delaytime);
/*Stops DMA-transfer*/
WriteDSP(0xD0);
}
}
fclose (WAVFile); /*Close the file*/
return 0;
}

