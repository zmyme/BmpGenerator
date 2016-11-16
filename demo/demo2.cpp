#include <iostream>
#include <ctime>
#include "bmp.h"

double GetDoubleRand(void)
{
    int Big=0,Small=0;
    long BigRandNumber=0;
    Big=rand();
    Small=rand();
    BigRandNumber=Big*RAND_MAX+Small;
    return ((double) BigRandNumber)/((double) RAND_MAX*(RAND_MAX));
}

int main(void)
{
	Bmp RandGraph(512,512);

	for(int i=0;i<512;i++)
	{
		for(int j=0;j<512;j++)
		{
			RandGraph.pixel(i,j,Palette(GetDoubleRand()*255,GetDoubleRand()*255,GetDoubleRand()*255));
		}
	}
	RandGraph.write("demo2.bmp");
	return 0;
}
