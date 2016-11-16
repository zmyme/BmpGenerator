#include <iostream>

#include "bmp.h"

int main()
{
    Bmp test(4096,4096);
    for(int i=0;i<4096;i++)
    {
        for(int j=0;j<4096;j++)
        {
            test.pixel(j,i,Palette(0xffffff));
        }
    }
    test.line(0,0,4096,4096,50,Palette(0x0000));
    test.rectangle(100,100,500,500,100,Palette(0x00000000));
    test.bar(1000,1000,1500,1500,Palette(0x00000000));
    test.circle(100,200,100,Palette(0x000000));
    cout<<"pixel at 1000,1000 is "<<(test.getpixel(1000,1000)).TellColor()<<endl;
    test.write("test.bmp");
    system("start test.bmp");
    return 0;
} 