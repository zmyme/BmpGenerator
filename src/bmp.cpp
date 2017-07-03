#include <vector>
#include "bmp.h"

inline INT_32 Palette::GetColor(INT_8 b,INT_8 g,INT_8 r)
{
    INT_32 temp=0x00000000;
    temp=(temp|(INT_32) b);
    temp=(temp<<8|(INT_32) g);
    temp=(temp<<8|(INT_32) r);
    return temp;
}

// inline Palette Palette::GetPalette(INT_32 color)
// {
//     Palette temp(color);
//     return temp;
// }
// inline Palette Palette::GetPalette(INT_8 b,INT_8 g,INT_8 r)
// {
//     Palette temp(b,g,r);
//     return temp;
// }

int Bmp::SetSize(INT_32 width, INT_32 height)
{
    if(data==NULL)
    {
        InitFileHeader();
        InitInfoHeader();
        file.bfSize=((width*3+3)/4)*4*height+file.bfOffbits;
        info.biWidth=width;
        info.biHeight=height;
        data = new Palette[width*height];
        return 0;
    }
    else
    {
        return -1;
    }
}

int Bmp::load(const char *path)
{
    if(data!=NULL)
    {
        return -1;
    }
    std::fstream pic;
    pic.open(path,std::ios::binary|std::ios::in);
    if(pic.is_open()==0)
    {
        std::cout<<"Open file "<<path<<" failed"<< std::endl;
        return -1;
    }
    pic.read((char *) &file,sizeof(BmpFileHeader));
    pic.read((char *) &info,sizeof(BmpInfoHeader));
    data = new Palette[info.biWidth*info.biHeight];
    for(INT_32 i=info.biHeight-1;i>=0;i--)
    {
        INT_32 SizePerLine=((info.biWidth*3+3)/4)*4;
        char *buffer = new char[SizePerLine];
        pic.read(buffer,SizePerLine);
        std::memcpy(data+info.biWidth*i,buffer,sizeof(Palette)*info.biWidth);
    }
    pic.close();
    return 0;
}

int Bmp::write(const char *path)
{
    std::fstream temp;
    temp.open(path,std::ios::binary|std::ios::out|std::ios::trunc);
    if(temp.is_open()==0)
    {
        std::cout<<"Open File "<<path<<" failed!"<< std::endl;
        return -1;
    }
    temp.write((char *) &file,sizeof(BmpFileHeader));
    temp.write((char *) &info,sizeof(BmpInfoHeader));
    for(INT_32 i=info.biHeight-1;i>=0;i--)
    {
        INT_32 SizePerLine=((info.biWidth*3+3)/4)*4;
        char *buffer = new char[SizePerLine];
        std::memcpy(buffer,data+info.biWidth*i,sizeof(Palette)*info.biWidth);
        temp.write(buffer,SizePerLine);
    }
    temp.close();
    return 0;
}

Palette Bmp::getpixel(INT_32 x,INT_32 y)
{
    Palette color;
    if(x>=0&&y>=0&&x<info.biWidth&&y<info.biHeight)
    {
        color=*(data+y*info.biWidth+x);
    }
    return color;
}

INT_32 Palette::TellColor(void)
{
    return GetColor(Blue,Green,Red);
}

int Bmp::line(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,int thickness,Palette color)
{
    INT_32 dx=x2-x1;
    INT_32 dy=y2-y1;
    INT_32 ux=((dx>0)<<1)-1;
    INT_32 uy=((dy>0)<<1)-1;
    INT_32 x=x1,y=y1,eps=0;
    dx=dx>0?dx:0-dx;
    dy=dy>0?dy:0-dy;

    if(dx>dy)
    {
        for(x=x1;x!=x2;x+=ux)
        {
            for(int i=0-thickness/2;i<=thickness/2;i++)
            {
                pixel(x,y+i,color);
            }
            eps+=dy;
            if((eps<<1)>=dx)
            {
                y+=uy;
                eps-=dx;
            }
        }
    }
    else
    {
        for(y=y1;y!=y2;y+=uy)
        {
            for(int i=0-thickness/2;i<=thickness/2;i++)
            {
                pixel(x+i,y,color);
            }
            eps+=dx;
            if((eps<<1)>=dy)
            {
                x+=ux;
                eps-=dy;
            }
        }
    }
    return 0;
}

int Bmp::rectangle(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,int thickness,Palette color)
{
    line(x1-thickness/2,y1,x2+thickness/2,y1,thickness,color);
    line(x1-thickness/2,y2,x2+thickness/2,y2,thickness,color);
    line(x1,y1-thickness/2,x1,y2+thickness/2,thickness,color);
    line(x2,y1-thickness/2,x2,y2+thickness/2,thickness,color);
    return 0;
}

int Bmp::bar(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,Palette color)
{
    for(INT_32 i=x1;i<=x2;i++)
    {
        line(i,y1,i,y2,1,color);
    }
    return 0;
}

int Bmp::circle(INT_32 x,INT_32 y,INT_32 r,Palette color)
{
    int x0=0,y0=r;
    int d=1-r;

    while(y0>x0)
    {
        pixel(x0+x,y0+y,color);
        pixel(y0+x,x0+y,color);
        pixel(-x0+x,y0+y,color);
        pixel(-y0+x,x0+y,color);
        pixel(-x0+x,-y0+y,color);
        pixel(-y0+x,-x0+y,color);
        pixel(x0+x,-y0+y,color);
        pixel(y0+x,-x0+y,color);
        if(d<0)
            d=d+2*x0+3;
        else
        {
            d=d+2*(x0-y0)+5;
            y0--;
        }
        x0++;
    }
    return 0;
}

int Bmp::resize(int NewWidth,int NewHeight)
{
    int OldWidth=width();
    int OldHeight=height();

    Palette *newdata = NULL;
    newdata = new Palette[NewWidth*NewHeight];

    for(int i=0;i<NewWidth;i++)
    {
        for(int j=0;j<NewHeight;j++)
        {
            PixelOnPointer(i,j,getpixel((i*OldWidth)/NewWidth,(j*OldHeight)/NewHeight),newdata,NewWidth,NewHeight);
        }
    }

    delete[] data;
    data = newdata;
    info.biWidth = NewWidth;
    info.biHeight = NewHeight;
    return 0;
}

int Bmp::resize(float percent)
{
    int NewWidth = width()*percent;
    int NewHeight = height()*percent;
    resize(NewWidth,NewHeight);
    return 0;
}

