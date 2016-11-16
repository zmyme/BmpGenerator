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

// using std::cin;
// using std::cout;
// using std::endl;
// using std::ios;

using namespace std;

class BmpFileHeader
{
public:
    BmpFileHeader(void)
    {
        bfType=0x4d42;
        bfSize=0;
        bfReserved1=0;
        bfReserved2=0;
        bfOffbits=54;
    }
    BmpFileHeader(INT_32 width,INT_32 height)
    {
        bfType=0x4d42;
        bfSize=0;
        bfReserved1=0;
        bfReserved2=0;
        bfOffbits=54;
        bfSize=((width*3+3)/4)*4*height+bfOffbits;
    }
    int SetBmpSize(INT_32 size);
    int SetDataOffbits(INT_32 offbits);
    INT_32 GetbfOffbits(void);
    int write(const char *path);
private:
    INT_16 bfType;
    INT_32 bfSize;
    INT_16 bfReserved1;
    INT_16 bfReserved2;
    INT_32 bfOffbits;
};

class BmpInfoHeader
{
public:
    BmpInfoHeader()
    {
        biSize=40;
        biWidth=0;
        biHeight=0;
        biPlanes=1;
        biBitCount=24;
        biCompression=0;
        biSizeImage=0;
        biXPelsPerMeter=0;
        biYpelsPerMeter=0;
        biClrUsed=0;
        biClrImportant=0;
    }
    BmpInfoHeader(INT_32 width,INT_32 height)
    {
        biSize=40;
        biWidth=width;
        biHeight=height;
        biPlanes=1;
        biBitCount=24;
        biCompression=0;
        biSizeImage=((width*3+3)/4)*4*height;
        biXPelsPerMeter=0;
        biYpelsPerMeter=0;
        biClrUsed=0;
        biClrImportant=0;
    }
    int SetSize(INT_32 width,INT_32 height);
    INT_32 GetWidth(void);
    INT_32 GetHeight(void);
    int write(const char *path);
private:
    INT_32 biSize;
    INT_32 biWidth;
    INT_32 biHeight;
    INT_16 biPlanes;
    INT_16 biBitCount;
    INT_32 biCompression;
    INT_32 biSizeImage;
    INT_32 biXPelsPerMeter;
    INT_32 biYpelsPerMeter;
    INT_32 biClrUsed;
    INT_32 biClrImportant;
};

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
        Blue=color>>16&0x000000ff;
        Green=color>>8&0x000000ff;
        Red=color&0x000000ff;
    }
    Palette(INT_8 b, INT_8 g, INT_8 r)
    {
        Blue=b;
        Green=g;
        Red=r;
    }
    static INT_32 GetColor(INT_8 b,INT_8 g,INT_8 r);
    static Palette GetPalette(INT_32 color);
    static Palette GetPalette(INT_8 b,INT_8 g,INT_8 r);
    INT_32 TellColor(void);
private:
    INT_8 Blue;
    INT_8 Green;
    INT_8 Red;
};

class Bmp : public BmpFileHeader, public BmpInfoHeader
{
public:
    Bmp(INT_32 width,INT_32 height) : BmpFileHeader(width,height),BmpInfoHeader(width,height)
    {
         data = new Palette[width*height];
    }
    Bmp(void)
    {
        data=NULL;
    }
    ~Bmp(void)
    {
        delete []data;
    }
    int SetSize(INT_32 width, INT_32 height);
    int write(const char *path);
    int pixel(INT_32 x,INT_32 y,Palette color);
    Palette getpixel(INT_32 x,INT_32 y);
    int line(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,int thickness,Palette color);
    int rectangle(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,int thickness,Palette color);
    int bar(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,Palette color);
    int circle(INT_32 x,INT_32 y,INT_32 r,Palette color);
private:
    Palette *data;
};

#endif
