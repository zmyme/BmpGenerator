#include "bmp.h"

int BmpFileHeader::SetBmpSize(INT_32 size)
{
    bfSize=size;
    return 0;
}

int BmpFileHeader::SetDataOffbits(INT_32 offbits)
{
    bfOffbits=offbits;
    return 0;
}

inline INT_32 BmpFileHeader::GetbfOffbits(void){return bfOffbits;}

int BmpFileHeader::write(const char *path)
{
    fstream temp;
    temp.open(path,ios::binary|ios::out|ios::trunc);
    if(temp.is_open()==0)
    {
        cout<<"Open File "<<path<<" failed!"<<endl;
        return -1;
    }
    temp.write((char *) this, sizeof(BmpFileHeader));
    temp.close();
    return 0;
}

int BmpInfoHeader::SetSize(INT_32 width,INT_32 height)
{
    biWidth=width;
    biHeight=height;
    return 0;
}

inline INT_32 BmpInfoHeader::GetWidth(void){return biWidth;}
inline INT_32 BmpInfoHeader::GetHeight(void){return biHeight;}

int BmpInfoHeader::write(const char *path)
{
    fstream temp;
    temp.open(path,ios::binary|ios::out|ios::app);
    if(temp.is_open()==0)
    {
        cout<<"Open File "<<path<<" failed!"<<endl;
        return -1;
    }
    temp.write((char *) this, sizeof(BmpInfoHeader));
    temp.close();
    return 0;
}

inline INT_32 Palette::GetColor(INT_8 b,INT_8 g,INT_8 r)
{
    INT_32 temp=0x00000000;
    temp=(temp|(INT_32) b);
    temp=(temp<<8|(INT_32) g);
    temp=(temp<<8|(INT_32) r);
    return temp;
}
inline Palette Palette::GetPalette(INT_32 color)
{
    Palette temp(color);
    return temp;
}
inline Palette Palette::GetPalette(INT_8 b,INT_8 g,INT_8 r)
{
    Palette temp(b,g,r);
    return temp;
}

int Bmp::SetSize(INT_32 width, INT_32 height)
{
    if(data==NULL)
    {
        SetBmpSize(((width*3+3)/4)*4*height+GetbfOffbits());
        BmpInfoHeader::SetSize(width,height);
        data=new Palette[width*height];
        return 0;
    }
    else
    {
        return -1;
    }
}

int Bmp::write(const char *path)
{
    fstream temp;
    BmpFileHeader::write(path);
    BmpInfoHeader::write(path);
    temp.open(path,ios::binary|ios::out|ios::app);
    if(temp.is_open()==0)
    {
        cout<<"Open File "<<path<<" failed!"<<endl;
        return -1;
    }if(temp.is_open()==0)
    {
        cout<<"Open File "<<path<<" failed!"<<endl;
        return -1;
    }
    int height=GetHeight();
    int width=GetWidth();
    for(INT_32 i=height-1;i>=0;i--)
    {
        INT_32 SizePerLine=((width*3+3)/4)*4;
        char *buffer = new char[SizePerLine];
        std::memcpy(buffer,data+width*i,sizeof(Palette)*width);
        temp.write(buffer,SizePerLine);
    }
    temp.close();
    return 0;
}

inline int Bmp::pixel(INT_32 x,INT_32 y,Palette color)
{
    if(x>=0&&y>=0&&x<GetWidth()&&y<GetHeight())
    {
        *(data+y*GetWidth()+x)=color;
    }
    return 0;
}

Palette Bmp::getpixel(INT_32 x,INT_32 y)
{
    Palette color;
    if(x>=0&&y>=0&&x<GetWidth()&&y<GetHeight())
    {
        color=*(data+y*GetWidth()+x);
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
