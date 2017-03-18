#ifndef _RAND_H_
#define _RAND_H_

void SetRand(int Drop);
double GetDoubleRand(void);
int GetRandInInterval(int min,int max);
int ReturnTrueWithRate(double rate);
int FindItemInList(int *List,int num,int item);
int GetRandList(int *List,int ListNum,int Max,int Min);

#endif