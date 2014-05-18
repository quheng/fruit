
#include   <stdio.h>
#include   <stdlib.h>
#include   <fcntl.h>
#include   <dos.h>
#include   <process.h>
#include   <bios.h>
#include   <graphics.h> 
#pragma pack(1); 


/* [%] ��д�ṹ���� */
typedef struct tagBITMAPFILEHEADER
{
    short   bfType;         /* ͨ���� 'BM' �����������ƺ��ж�OS/2�ı�ʶ����ʲô����*/
    long    bfSize;         /* �ļ���С�����ֽ�Ϊ��λ*/
    short   bfReserved1;    /*��������������Ϊ0*/
    short   bfReserved2;    /*��������������Ϊ0*/
    long    bfOffBits;      /*���ļ�ͷ��ʼ��ʵ�ʵ�ͼ������֮����ֽڵ�ƫ��������*/
                            /*�������Ƿǳ����õģ���Ϊλͼ��Ϣͷ�͵�ɫ��ĳ��Ȼ�*/
                            /*���ݲ�ͬ������仯�����������ƫ��ֵѸ�ٵĴ��ļ���*/
                            /*��ȡ��λ���ݡ� */
} BITMAPFILEHEADER;

/*��Ϣͷ�ṹ*/
typedef struct tagBITMAPINFOHEADER
{
    long    biSize;             /* ��Ϣͷ��С */
    long    biWidth;            /* ͼ���� */
    long    biHeight;           /* ͼ��߶� */
    short   biPlanes;           /* ����Ϊ1 */
    short   biBitCount;         /* ÿ����λ����������1, 4, 8��24 */
    long    biCompression;      /* ѹ������ */
    long    biSizeImage;        /* ʵ��ͼ���С��������4�ı��� */
    long    biXPelsPerMeter;    /* ˮƽ����ÿ�������� */
    long    biYPelsPerMeter;    /* ��ֱ����ÿ��������*/
    long    biClrUsed;          /* ������ɫ��*/
    long    biClrImportant;     /* ��Ҫ����ɫ�� */
} BITMAPINFOHEADER;

/*��ɫ��*/
typedef struct tagRGBQUAD
{
    char    rgbBlue;   /*��ɫ����*/
    char    rgbGreen;  /*��ɫ����*/
    char    rgbRed;    /*��ɫ����*/
    char    rgbReserved;
} RGBQUAD;


int COLS=640, ROWS=480;		/* ȱʡΪ256ɫ640*480ģʽ */

/*���õ�ɫ��*/
void set_SVGA_palette(unsigned char r[],  unsigned char g[], unsigned char b[])
{
	int  k;
 	for (k = 0; k < 256; k++) {
  		outportb(0x03C8,k);
		outportb(0x03C9,r[k]>>2);
		outportb(0x03C9,g[k]>>2);
		outportb(0x03C9,b[k]>>2);
	}
}

void initial()
{
     BITMAPFILEHEADER    FileHeader;
     BITMAPINFOHEADER    bmiHeader;
     RGBQUAD             bmiColors[256];
     unsigned char       buffer[1024], r[256], g[256], b[256];
     unsigned int	 width, height, linebytes;
     long		 offset, position;
     char page_new=0,page_old=0;
     int i,j,k,n,savemode;
     FILE *fp;

     if((fp=fopen("bb.bmp","rb"))==NULL) /*�жϴ��ļ��Ƿ���ȷ*/
     {
	printf("Can't open file: %s",buffer);
	return;
     }

     if (fread((char *)&FileHeader, sizeof(FileHeader), 1, fp) != 1) {
        printf("Can't read file header !\n"); /* ���ļ�ͷ */
        return;
     }
     if (FileHeader.bfType != 0X4D42) {  /* BM */
        fprintf(stderr, "Not a BMP file !\n");
        return;
     }

     if (fread((char *)&bmiHeader, sizeof(bmiHeader), 1, fp) != 1) {
        fprintf(stderr, "Can't read bmiHeader !\n");	/* ����Ϣͷ */
        return;
     }

     width = (unsigned int)bmiHeader.biWidth;
     height = (unsigned int)bmiHeader.biHeight;
     linebytes = ((width*(long)bmiHeader.biBitCount+31)/32)*4; /* ÿ���ֽ���--4�������� */

     if (fread((char *)&bmiColors[0], 4, 256, fp) != 256) { /* ����ɫ������ */
        fprintf(stderr, "Can't get palette !\n");
        return;
     }


     /* [%] �������¸���������ͼ��ģʽ��ʼ��: */
     {
        int driver=VGA, mode=VGAHI;
        savemode = _mode_index; /* ����ԭͼ��ģʽ */
        initgraph(&driver, &mode, "");
     }


     COLS=640;ROWS=480;
     for (i = 0; i < 256; i++) {
        r[i] = bmiColors[i].rgbRed;
        g[i] = bmiColors[i].rgbGreen;
        b[i] = bmiColors[i].rgbBlue;
     }
     set_SVGA_palette(r, g, b);	/* ���õ�ɫ�� */

     offset = FileHeader.bfOffBits;
     fseek(fp, offset, SEEK_SET);    /* ����λͼ���ݵ���ʼλ�� */
     for(j=height-1;j>=0;j--) {
        fread(buffer,linebytes,1,fp);
        for(i=0,n=0;i<width;i++,n++) {
           position=j*(long)COLS+i; /*����Ҫ��ʾ����Դ�λ��*/

           /* [%] ������������滻����ɾ���Ĵ���: */
           memcpy(_vp+position, buffer, linebytes);

        }
     }
     fclose(fp);
     setbgm();
}

