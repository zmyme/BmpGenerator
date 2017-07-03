#include <vector>
#include "bmp.h"
#include "errorcode.h"
#include <iostream>

using std::cout;
using std::endl;

int FindContours(Bmp &source,Bmp &dest,int type)
{
    int width = source.width();
    int height = source.height();
    Bmp Temp(width,height);
    RGB2Grey(&source,&Temp);
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            int list[9];
            int k=0;
            for(k=0;k<9;k++) list[k]=0;
            k=0;
            for(int x=i-1;x<=i+1;x++)
            {
                for(int y=j-1;y<=j+1;y++)
                {
                    if(x>=0 && y>=0 && x<width && y<height)
                    {
                        list[k]=Temp.getpixel(i,j).Blue - Temp.getpixel(x,y).Blue;
                        if(list[k]<0) list[k]=-list[k];
                    }
                    k++;
                }
            }
            int max=0;
            for(k=0;k<9;k++)
            {
                if(list[k]>max)
                {
                    max=list[k];
                }
            }

            dest.pixel(i,j,Palette(255-max,255-max,255-max));
        }
    }
    return 0;
}

int RGB2Grey(Bmp *source, Bmp *dest)
{
    for(int i=0;i<source->width();i++)
    {
        for(int j=0;j<source->height();j++)
        {
            //0.30R + 0.59G + 0.11B
            Palette temp=source->getpixel(i,j);
            int color=temp.Blue*0.11+temp.Green*0.59+temp.Red*0.3;
            dest->pixel(i,j,Palette(color,color,color));
        }
    }
    return 0;
}

int Grey2Binary(Bmp *source,Bmp *dest,float light)
{
    int average=0;

    for(int i=0;i<source->width();i++)
    {
        for(int j=0;j<source->height();j++)
        {
            average+=source->getpixel(i,j).Blue;
        }
    }

    average=average/(source->width()*source->height());

    for(int i=0;i<source->width();i++)
    {
        for(int j=0;j<source->height();j++)
        {
            Palette temp=source->getpixel(i,j);
            if(temp.Blue>average/light)
            {
                dest->pixel(i,j,Palette(255,255,255));
            }
            else
            {
                dest->pixel(i,j,Palette(0,0,0));
            }
            
        }
    }

    return average;
}

int Grey2Binary(Bmp *source,Bmp *dest,int Threshold)
{
    for(int i=0;i<source->width();i++)
    {
        for(int j=0;j<source->height();j++)
        {
            Palette temp=source->getpixel(i,j);
            if(temp.Blue>Threshold)
            {
                dest->pixel(i,j,Palette(255,255,255));
            }
            else
            {
                dest->pixel(i,j,Palette(0,0,0));
            }
            
        }
    }

    return 0;
}

int FindAroundPixelNum(Bmp *pic, int x,int y,Palette color)
{
    int num=0;
    for(int i=x-1;i<=x+1;i++)
    {
        for(int j=y-1;j<=y+1;j++)
        {
            if(x!=i||y!=j)
            {
                if(pic->getpixel(i,j).TellColor()==color.TellColor())
                {
                    num++;
                }
            }
        }
    }
    return num;
}

int AntiColor(Bmp *source,Bmp *dest)
{
    int width=source->width();
    int height=source->height();

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            Palette temp=source->getpixel(i,j);
            dest->pixel(i,j,Palette(~temp.Blue,~temp.Green,~temp.Red));
        }
    }

    return 0;
}

Palette AverageColor(Bmp *source,int x,int y,int pixel)
{
    int NumEffective=0;
    int width=source->width();
    int height=source->height();
    int Red=0,Blue=0,Green=0;

    for(int i=x-pixel;i<=x+pixel;i++)
    {
        for(int j=y-pixel;j<y+pixel;j++)
        {
            if(i>=0&&i<width&&j>=0&&j<height)
            {
                NumEffective++;
                Palette temp;
                temp=source->getpixel(i,j);
                Red+=temp.Red;
                Green+=temp.Green;
                Blue+=temp.Blue;
            }
        }
    }

    Red/=NumEffective;
    Green/=NumEffective;
    Blue/=NumEffective;

    return Palette(Blue,Green,Red);
}

int Blurry(Bmp *source,Bmp *dest,int pixel)
{
    int width=source->width();
    int height=source->height();

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            dest->pixel(i,j,AverageColor(source,i,j,pixel));
        }
    }

    return 0;
}

int Mosaic(Bmp *source,Bmp *dest,int pixel)
{
    int width=source->width();
    int height=source->height();

    for(int i=pixel;i<width+pixel;i+=2*pixel)
    {
        for(int j=pixel;j<height+pixel;j+=2*pixel)
        {
            Palette color=AverageColor(source,i,j,pixel);
            for(int x=i-pixel;x<=i+pixel;x++)
            {
                for(int y=j-pixel;y<=j+pixel;y++)
                {
                    dest->pixel(x,y,color);
                }
            }
        }
    }
    return 0;
}

int AdjustLight(Bmp *source,Bmp *dest,float light)
{
    int width=source->width();
    int height=source->height();
    float LightKeep=light;

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            light=LightKeep;
            Palette temp=source->getpixel(i,j);
            int Blue=temp.Blue*light;
            int Green=temp.Green*light;
            int Red=temp.Red*light;
            if(Blue>255||Green>255||Red>255)
            {
                int max;
                max=temp.Blue>temp.Green?temp.Blue:temp.Green;
                max=max>temp.Red?max:temp.Red;
                light=255.0/(max*1.0);
                dest->pixel(i,j,Palette(temp.Blue*light,temp.Green*light,temp.Red*light));
            }
            else
            {
                dest->pixel(i,j,Palette(Blue,Green,Red));
            }
        }
    }
    return 0;
}

int ImageFusion(Bmp *pic1,float weight1,Bmp *pic2,float weight2,Bmp *dest)
{
    float RealWeight1=weight1/(weight1+weight2);
    float RealWeight2=weight2/(weight1+weight2);

    int width=pic1->width()>pic2->width()?pic2->width():pic1->width();
    int height=pic1->height()>pic2->height()?pic2->height():pic1->height();

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            Palette Pixel1=pic1->getpixel(i,j);
            Palette Pixel2=pic2->getpixel(i,j);
            int Blue=Pixel1.Blue*RealWeight1+Pixel2.Blue*RealWeight2;
            int Green=Pixel1.Green*RealWeight1+Pixel2.Green*RealWeight2;
            int Red=Pixel1.Red*RealWeight1+Pixel2.Red*RealWeight2;
            dest->pixel(i,j,Palette(Blue,Green,Red));
        }
    }

    return 0;
}

float GetPixelPercentAround(Bmp *pic,int x,int y,int pixel,Palette color)
{
    int NumEffective=0;
    int NumPixel=0;
    int width=pic->width();
    int height=pic->height();

    for(int i=x-pixel;i<=x+pixel;i++)
    {
        for(int j=y-pixel;j<=y+pixel;j++)
        {
            if(i>=0&&i<width&&j>=0&&j<height)
            {
                NumEffective++;
                if(pic->getpixel(i,j).TellColor()==color.TellColor())
                {
                    NumPixel++;
                }
            }
        }
    }

    return NumPixel*1.0/(NumEffective*1.0);
}

int EliminateNoiseInBinaryPic(Bmp *source,Bmp *dest,int pixel,float noise)
{
    int width=source->width();
    int height=source->height();

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            if(source->getpixel(i,j).TellColor()==0xffffff)
            {
                if(GetPixelPercentAround(source,i,j,pixel,0xffffff)<noise)
                {
                    dest->pixel(i,j,0x000000);
                }
                else
                {
                    dest->pixel(i,j,0xffffff);
                }
            }
            else
            {
                if(GetPixelPercentAround(source,i,j,pixel,0x000000)<noise)
                {
                    dest->pixel(i,j,0xffffff);
                }
                else
                {
                    dest->pixel(i,j,0x000000);
                }
            }
        }
    }

    return 0;
}

int ShowHistogram(Bmp &source,Bmp &output)
{
    int Red[256];
    int Green[256];
    int Blue[256];

    int width = source.width();
    int height = source.height();

    for(int i=0;i<255;i++)
    {
        Red[i] = 0;
        Green[i] = 0;
        Blue[i] = 0;
    }

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            Palette temp=source.getpixel(i,j);
            Red[temp.Red]++;
            Blue[temp.Blue]++;
            Green[temp.Green]++;
        }
    }

    int RedMax = 0;
    int BlueMax = 0;
    int GreenMax = 0;

    for(int i=0;i<255;i++)
    {
        if(Red[i]>RedMax)
        {
            RedMax = Red[i];
        }

        if(Blue[i]>BlueMax)
        {
            BlueMax = Blue[i];
        }

        if(Green[i]>GreenMax)
        {
            GreenMax = Green[i];
        }

    }

    for(int i=0;i<255;i++)
    {
        Red[i] = Red[i]*255/RedMax;
        Blue[i] = Blue[i]*255/BlueMax;
        Green[i] = Green[i]*255/GreenMax;
    }

    int GraphWidth = output.width();
    int GraphHeight = output.height();

    if(GraphHeight<256)
    {
        return PIC_GIVEN_TOO_SMALL;
    }
    if(GraphWidth<256)
    {
        return PIC_GIVEN_TOO_SMALL;
    }

    int Dwidth = GraphWidth/256;
    int Dheight = GraphHeight/256;

    int LastX = 0;
    int LastY = Red[0]*Dheight;

    for(int i=1;i<256;i++)
    {
        output.line(LastX,GraphHeight-LastY,LastX+Dwidth,GraphHeight-Red[i]*Dheight,Dheight,Palette(0,0,255));
        LastX+=Dwidth;
        LastY=Red[i]*Dheight;
    }
    LastX = 0;
    LastY = Blue[0]*Dheight;

    for(int i=1;i<256;i++)
    {
        output.line(LastX,GraphHeight-LastY,LastX+Dwidth,GraphHeight-Blue[i]*Dheight,Dheight,Palette(255,0,0));
        LastX+=Dwidth;
        LastY=Blue[i]*Dheight;
    }
    LastX = 0;
    LastY = Green[0]*Dheight;

    for(int i=1;i<256;i++)
    {
        output.line(LastX,GraphHeight-LastY,LastX+Dwidth,GraphHeight-Green[i]*Dheight,Dheight,Palette(0,255,0));
        LastX+=Dwidth;
        LastY=Green[i]*Dheight;
    }

    return 0;
}

// int ImageSegmentation(Bmp *pic,Bmp *markers,Bmp *mask)
// {
//     int width=pic->width();
//     int height=pic->height();
//     std::vector<Point> seed;

//     for(int i=0;i<width;i++)
//     {
//         for(int j=0;j<height;j++)
//         {
//             if(markers.getpixel(i,j).TellColor()!=0x000000)
//             {
//                 seed.push_back(Point(i,j));
//             }
//         }
//     }

//     return 0;
// }