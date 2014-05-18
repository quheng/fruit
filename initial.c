
#include   <stdio.h>
#include   <stdlib.h>
#include   <fcntl.h>
#include   <dos.h>
#include   <process.h>
#include   <bios.h>
#include   <graphics.h> 
#pragma pack(1); 


/* [%] 重写结构类型 */
typedef struct tagBITMAPFILEHEADER
{
    short   bfType;         /* 通常是 'BM' 。现在来看似乎判断OS/2的标识已无什么意义*/
    long    bfSize;         /* 文件大小，以字节为单位*/
    short   bfReserved1;    /*保留，必须设置为0*/
    short   bfReserved2;    /*保留，必须设置为0*/
    long    bfOffBits;      /*从文件头开始到实际的图象数据之间的字节的偏移量。这*/
                            /*个参数是非常有用的，因为位图信息头和调色板的长度会*/
                            /*根据不同情况而变化，可以用这个偏移值迅速的从文件中*/
                            /*读取到位数据。 */
} BITMAPFILEHEADER;

/*信息头结构*/
typedef struct tagBITMAPINFOHEADER
{
    long    biSize;             /* 信息头大小 */
    long    biWidth;            /* 图像宽度 */
    long    biHeight;           /* 图像高度 */
    short   biPlanes;           /* 必须为1 */
    short   biBitCount;         /* 每像素位数，必须是1, 4, 8或24 */
    long    biCompression;      /* 压缩方法 */
    long    biSizeImage;        /* 实际图像大小，必须是4的倍数 */
    long    biXPelsPerMeter;    /* 水平方向每米像素数 */
    long    biYPelsPerMeter;    /* 垂直方向每米像素数*/
    long    biClrUsed;          /* 所用颜色数*/
    long    biClrImportant;     /* 重要的颜色数 */
} BITMAPINFOHEADER;

/*调色板*/
typedef struct tagRGBQUAD
{
    char    rgbBlue;   /*蓝色分量*/
    char    rgbGreen;  /*绿色分量*/
    char    rgbRed;    /*红色分量*/
    char    rgbReserved;
} RGBQUAD;


int COLS=640, ROWS=480;		/* 缺省为256色640*480模式 */

/*设置调色板*/
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

     if((fp=fopen("bb.bmp","rb"))==NULL) /*判断打开文件是否正确*/
     {
	printf("Can't open file: %s",buffer);
	return;
     }

     if (fread((char *)&FileHeader, sizeof(FileHeader), 1, fp) != 1) {
        printf("Can't read file header !\n"); /* 读文件头 */
        return;
     }
     if (FileHeader.bfType != 0X4D42) {  /* BM */
        fprintf(stderr, "Not a BMP file !\n");
        return;
     }

     if (fread((char *)&bmiHeader, sizeof(bmiHeader), 1, fp) != 1) {
        fprintf(stderr, "Can't read bmiHeader !\n");	/* 读信息头 */
        return;
     }

     width = (unsigned int)bmiHeader.biWidth;
     height = (unsigned int)bmiHeader.biHeight;
     linebytes = ((width*(long)bmiHeader.biBitCount+31)/32)*4; /* 每行字节数--4的整数倍 */

     if (fread((char *)&bmiColors[0], 4, 256, fp) != 256) { /* 读调色板数据 */
        fprintf(stderr, "Can't get palette !\n");
        return;
     }


     /* [%] 增加以下复合语句进行图形模式初始化: */
     {
        int driver=VGA, mode=VGAHI;
        savemode = _mode_index; /* 保存原图形模式 */
        initgraph(&driver, &mode, "");
     }


     COLS=640;ROWS=480;
     for (i = 0; i < 256; i++) {
        r[i] = bmiColors[i].rgbRed;
        g[i] = bmiColors[i].rgbGreen;
        b[i] = bmiColors[i].rgbBlue;
     }
     set_SVGA_palette(r, g, b);	/* 设置调色板 */

     offset = FileHeader.bfOffBits;
     fseek(fp, offset, SEEK_SET);    /* 跳到位图数据的起始位置 */
     for(j=height-1;j>=0;j--) {
        fread(buffer,linebytes,1,fp);
        for(i=0,n=0;i<width;i++,n++) {
           position=j*(long)COLS+i; /*计算要显示点的显存位置*/

           /* [%] 增加以下语句替换后面删除的代码: */
           memcpy(_vp+position, buffer, linebytes);

        }
     }
     fclose(fp);
     setbgm();
}

