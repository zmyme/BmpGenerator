#include <iostream>
#include "bmp.h"

int main(void)
{
	Bmp pic(767,767);
	int counter=0;
	for(int i=0;i<767;i++)
	{
		for(int j=0;j<767;j++)
		{
			pic.pixel(i,j,Palette(255*(counter%3==0?1:0),255*(counter%3==1?1:0),255*(counter%3==2?1:0)));
			counter++;
		}
	}
	pic.write("demo3.bmp");
	return 0;
}