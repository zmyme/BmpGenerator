/*********************************************************************
this file is about generating ramdon numbers under some restrictions.
*********************************************************************/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "rand.h"

void SetRand(int Drop)
{
    int i=0;
    srand((unsigned) time(NULL));
    for(i=0;i<Drop;i++) rand();
}

double GetDoubleRand(void)
{
    int Big=0,Small=0;
    long BigRandNumber=0;
    Big=rand();
    Small=rand();
    BigRandNumber=Big*RAND_MAX+Small;
    return ((double) BigRandNumber)/((double) RAND_MAX*(RAND_MAX+1));
}

int GetRandInInterval(int min,int max)
{
    return min+(max-min)*((double) GetDoubleRand());
}

int ReturnTrueWithRate(double rate)
{
    if(GetDoubleRand()<=rate)
        return 1;
    else
        return 0;
}

int FindItemInList(int *List,int num,int item)
{
    int i=0;
    for(i=0;i<num;i++)
    {
        if(List[i]==item)
        {
            return i;
        }
    }

    return -1;
}

int GetRandList(int *List,int ListNum,int Min,int Max)
{
    int i=0;
    int temp;

    for(i=0;i<ListNum;i++)
    {
        temp=GetRandInInterval(Min,Max);
        if(FindItemInList(List,i,temp)==-1)
        {
            List[i]=temp;
        }
        else
        {
            i--;
        }
    }

    return 0;
}