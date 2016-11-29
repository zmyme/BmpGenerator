#ifndef _BMP_H
#define _BMP_H

#include <iostream>
#include <fstream>
#include <cstring>

#include <cstdlib>

#pragma pack(2)

typedef short INT_16;
typedef int INT_32;
typedef unsigned char INT_8;

using namespace std;

class Palette
{
public:
    Palette(void)
    {
        Blue=0;
        Green=0;
        Red=0;
    }
    Palette(INT_32 color)
    {
		Blue  =color>>16&0x000000ff;
		Green =color>>8&0x000000ff;
		Red   =color&0x000000ff;
    }
    Palette(INT_8 b, INT_8 g, INT_8 r)
    {
		Blue  =b;
		Green =g;
		Red   =r;
    }
    static INT_32 GetColor(INT_8 b,INT_8 g,INT_8 r);
    static Palette GetPalette(INT_32 color);
    static Palette GetPalette(INT_8 b,INT_8 g,INT_8 r);
    INT_32 TellColor(void);

    //below is data.
    INT_8 Blue;
    INT_8 Green;
    INT_8 Red;
};

struct BmpFileHeader
{
	INT_16 bfType;      //'BM', claim that this is a bmp file.
	INT_32 bfSize;      //picture size.
	INT_16 bfReserved1;
	INT_16 bfReserved2; //reserved data.
	INT_32 bfOffbits;   //offset to real picture data.
};

struct BmpInfoHeader
{
	INT_32 biSize;          // size of this struct.
	INT_32 biWidth;         //picture width.
	INT_32 biHeight;        //picture height.
	INT_16 biPlanes;        // planes, should always be 1
	INT_16 biBitCount;      // bit depth, we only support 24 bit.
	INT_32 biCompression;   //compress or not. we do not support compressed pic.
	INT_32 biSizeImage;     // we use RGB mode, 0 will be okay.
	INT_32 biXPelsPerMeter; // 0 will be okay.
	INT_32 biYpelsPerMeter; // 0 will be okay.
	INT_32 biClrUsed;       // 0 will be okay.
	INT_32 biClrImportant;  //0 will be okay.
};

class Bmp
{
public:
	Bmp(void)
	{
		InitFileHeader();
		InitInfoHeader();
		data=NULL;
	}
	Bmp(INT_32 width,INT_32 height)
	{
		InitFileHeader();
		InitInfoHeader();
		file.bfSize=((width*3+3)/4)*4*height+file.bfOffbits;
		info.biWidth=width;
		info.biHeight=height;
		data = new Palette[width*height];
	}
	Bmp(const char *filename)
	{
		data=NULL;
		load(filename);
	}
	~Bmp()
	{
		if(data!=NULL)
		{
			delete []data;
		}
	}

	//get info.
	INT_32 width(void){return info.biWidth;}
	INT_32 height(void){return info.biHeight;}

	//Load and write.
	int load(const char *path);
    int write(const char *path);

	//basic draw function.
    int SetSize(INT_32 width, INT_32 height);
    int pixel(INT_32 x,INT_32 y,Palette color)
    {
	    if(x>=0&&y>=0&&x<info.biWidth&&y<info.biHeight)
	    {
	        *(data+y*info.biWidth+x)=color;
	    }
	    return 0;
	}
    Palette getpixel(INT_32 x,INT_32 y);
    int line(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,int thickness,Palette color);
    int rectangle(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,int thickness,Palette color);
    int bar(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,Palette color);
    int circle(INT_32 x,INT_32 y,INT_32 r,Palette color);
private:
	inline void InitFileHeader(void)
	{
		file.bfType      =0x4d42;
		file.bfSize      =0;
		file.bfReserved1 =0;
		file.bfReserved2 =0;
		file.bfOffbits   =54;
	}
	inline void InitInfoHeader(void)
	{
		info.biSize          =40;
		info.biWidth         =0;
		info.biHeight        =0;
		info.biPlanes        =1;
		info.biBitCount      =24;
		info.biCompression   =0;
		info.biSizeImage     =0;
		info.biXPelsPerMeter =0;
		info.biYpelsPerMeter =0;
		info.biClrUsed       =0;
		info.biClrImportant  =0;
	}
    BmpFileHeader file;
    BmpInfoHeader info;
    Palette *data;
};

int RGB2Grey(Bmp *source, Bmp *dest);
int Grey2Binary(Bmp *source,Bmp *dest,float light);
int AntiColor(Bmp *source,Bmp *dest);
Palette AverageColor(Bmp *source,int x,int y,int pixel);
int Blurry(Bmp *source,Bmp *dest,int pixel);
int Mosaic(Bmp *source,Bmp *dest,int pixel);

#endif