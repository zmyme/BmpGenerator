#include "bmp.h"

#include <iostream>

int main(void)
{
	Bmp fullpixel(4096,4096);
	for(int i=0;i<4096;i++)
	{
		for(int j=0;j<4096;j++)
		{
			fullpixel.pixel(i,j,Palette(j*4096+i));
		}
	}
	fullpixel.write("demo1.bmp");
	return 0;
}
