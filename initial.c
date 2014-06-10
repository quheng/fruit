/****************************************************/
/* VESA 24-bit color demo in Watcom C               */
/* copyright (C) Black White.  Mar 1, 2014          */
/* email: iceman@zju.edu.cn                         */
/* web: http://blackwhite.8866.org/bhh              */
/****************************************************/
#include <graphics.h>
#include <stdio.h>

volatile int stop=0, dx=1, dy=1;
#define TICKS_TO_CHANGE_POSITION 5
#define TICKS_TO_CHANGE_MANDEL ((TICKS_TO_CHANGE_POSITION)*50)
#define TICKVARS 10
volatile int *ptickvar[TICKVARS]={NULL};

int palettes, mandels;
byte *ppalette, *pmandel;

#define BLKSIZE  0x100
volatile struct 
{
   int x[2], y[2];              // (x[0],y[0]) are the coordinates of block at page 0
                                // (x[1],y[1]) are the coordinates of block at page 1
   int ticks_to_change_position, ticks_to_change_mandel, new_created;
   struct picture *back_img[2]; // 2 background images under the block
   struct picture *old_img;     // 1 drawing image
   struct picture *new_img;     // 1 image ready to draw
} block;


int get_file_len(FILE *fp)
{
   int n;
   fseek(fp, 0, SEEK_END);
   n = ftell(fp);
   fseek(fp, 0, SEEK_SET);
   return n;
}

void show_color(byte *p)
{
   int x=0, y=-16, i, j;
   dword color;
   char t[10];
   for(i=0; i<256; i++)
   {
      byte c[4];
      c[0] = p[i*3+2];
      c[1] = p[i*3+1];
      c[2] = p[i*3];
      c[3] = 0;
      color = *(dword *)c;
      setcolor(color);
      y += 16;
      if(y >= _height)
      {
         y = 0;
         x += 150;
      }
      for(j=0; j<16; j++)
      {
         line(x, y+j, x+120-1, y+j);
      }
      setcolor(0x00FFFFFF);
      sprintf(t, "%02X", i);
      outtextxy(x+120, y, t); 
   }
}

int read_palette_from_file(byte **pp)
{
   int pal_file_len, palettes;
   byte *pbuf, *p;
   FILE *fp;
   fp = fopen("palette.dat", "rb");
   if(fp == NULL)
      return 0;
   pal_file_len = get_file_len(fp);
   palettes = pal_file_len / 0x300;
   pbuf = malloc(pal_file_len);
   fread(pbuf, 1, pal_file_len, fp);
   fclose(fp);
   *pp = pbuf;
   return palettes;
}

int read_mandel_from_file(byte **pp)
{
   int mandel_file_len, mandels;
   byte *pbuf, *p;
   FILE *fp;
   fp = fopen("mandel.dat", "rb");
   if(fp == NULL)
      return 0;
   mandel_file_len = get_file_len(fp);
   mandels = mandel_file_len / (0x100*0x100);
   pbuf = malloc(mandel_file_len);
   fread(pbuf, 1, mandel_file_len, fp);
   fclose(fp);
   *pp = pbuf;
   return mandels;
}

void show_mandel(int x, int y, byte *pmandel, byte *ppalette)
{
   int i, j;
   byte c[4];
   dword color;
   byte (*p)[0x100] = (byte (*)[0x100])pmandel;
   byte (*pal)[3] = (byte (*)[3])ppalette;
   for(i=0; i<0x100; i++)
   {
      for(j=0; j<0x100; j++)
      {
         c[0] = pal[p[i][j]][2];
         c[1] = pal[p[i][j]][1];
         c[2] = pal[p[i][j]][0];
         c[3] = 0;
         color = *(dword *)c;
         putpixel(x+j, y+i, color);
      }
   }
}

void build_mandel_in_memory(struct picture *pic, byte *pmandel, byte *ppalette)
{
   int i, j;
   byte c[4];
   dword color;
   byte *pbuf = &pic->buffer;
   byte (*p)[0x100] = (byte (*)[0x100])pmandel;
   byte (*pal)[3] = (byte (*)[3])ppalette;
   for(i=0; i<0x100; i++)
   {
      for(j=0; j<0x100; j++)
      {
         c[0] = pal[p[i][j]][2]; // blue
         c[1] = pal[p[i][j]][1]; // green
         c[2] = pal[p[i][j]][0]; // red
         c[3] = 0;
         color = *(dword *)c;
         *(dword *)(pbuf+(i*BLKSIZE+j)*(_color_bits/8)) = color;
      }
   }
   pic->picwidth = pic->picheight = BLKSIZE;
   pic->bpp = _color_bits;
}

int load_24bit_bmp(int x, int y, char *filename)
{
   FILE *fp = NULL;
   byte *p = NULL; /* pointer to a line of bmp data */
   byte *vp = _vp + (_active_page*_page_gap + y*_width + x) * (_color_bits/8);
   dword width, height, bmp_data_offset, bytes_per_line, offset;
   int i;
   p = malloc(1024L * 3);  /* memory for holding a line of bmp data */
   if(p == NULL)  /* cannot allocate enough memory for drawing 1 line */
      goto display_bmp_error;
   fp = fopen(filename, "rb");
   if(fp == NULL) /* cannot open bmp file */
      goto display_bmp_error;
   fread(p, 1, 0x36, fp);     /* read BMP head */
   if(*(word *)p != 0x4D42)   /* check BMP signature */
      goto display_bmp_error; /* not a BMP file */
   if(*(word *)(p+0x1C) != 24)
      goto display_bmp_error; /* not a 24-bit-color BMP file */
   width = *(dword *)(p+0x12);
   height = *(dword *)(p+0x16);
   bmp_data_offset = *(dword *)(p+0x0A);
   fseek(fp, bmp_data_offset, SEEK_SET); /* skip BMP head */
   bytes_per_line = (width * 3 + 3) / 4 * 4; /* must be multiple of 4 */
   for(i=height-1; i>=0; i--)          /* draw from bottom to top */
   {
      fread(p, 1, bytes_per_line, fp); /* read a line of bmp data */
      offset = i * 1024 * 3;
      memcpy(vp+offset, p, width*3);
   }
   free(p);
   fclose(fp);
   return 1;
   display_bmp_error:
   if(p != NULL)
      free(p);
   if(fp != NULL)
      fclose(fp);
   return 0;
}


int initial(char *buffer)
{
   int driver=0, mode=VESA_1024x768x24bit;
   palettes = read_palette_from_file(&ppalette);
   mandels = read_mandel_from_file(&pmandel);
   initgraph(&driver, &mode, "");
   load_24bit_bmp(0, 0, buffer);
   return;
}