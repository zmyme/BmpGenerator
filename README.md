# Welcom to BmpGenerator!



## 1. What's this and what can it do?

This is an tiny C++ library which can be used to draw something on a bmp picture. It can be used in algorithm visualization and maybe computer vision (this may cost sometime but i will try my best to make it ^_^).

## 2. How to install it?

This might be the most easy to use library you may have ever seen since it is so small. Just add the bmp.cpp in the ./src directory to your source code directory and bmp.h in the ./include directory to your include directory and then you can wirte you own programme. Here is an example.

	#include "bmp.h"

	#include <iostream>

	int main(void)
	{
		Bmp fullpixel(4096,4096);//creat an bmp file at the width of 4096 and height of 4096.
		for(int i=0;i<4096;i++)
		{
			for(int j=0;j<4096;j++)
			{
				fullpixel.pixel(i,j,Palette(j*4096+i));//paint each pixel with a different color.
			}
		}
		fullpixel.write("demo1.bmp");//save the bmp picture to file.
		return 0;
	}

This code can be found in ./demo/demo1.cpp, you can just compile it with command `g++ dem1.cpp bmp.cpp` if you have bmp.cpp bmp.h and demo1.cpp in your current working directory. use `./a.out`(or `a.exe` if you are using windows) to run this programme ,after that , you wil see a demo1.bmp is created. you can open it to see the output.

if it doesn't work, the you may be using a different compiler or under a diferent platform. you can open the file bmp.h and change the code in the begining to match you enviroment:

	typedef short INT_16;
	typedef int INT_32;
	typedef unsigned char INT_8;

INT_NUMBER means it should be defined as a integar which takes up NUMBER bit. change this to your computer and then compile it again, if you still can't solve the problem ,you can open a issue or you can just email me.

## 3. How to use it?

Currently I only provided a few function for you to use. here they are:

    int SetSize(INT_32 width, INT_32 height);//set a bmp file size if you didn't give a size when yo create it.
    int write(const char *path);//save bmp to file.
    int pixel(INT_32 x,INT_32 y,Palette color);//paint a pixel
    Palette getpixel(INT_32 x,INT_32 y);//get the color of a give pixel.
    int line(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,int thickness,Palette color);// draw a line
    int rectangle(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,int thickness,Palette color);// draw a rectangle
    int bar(INT_32 x1,INT_32 y1,INT_32 x2,INT_32 y2,Palette color);//draw a bar.
    int circle(INT_32 x,INT_32 y,INT_32 r,Palette color);// draw a circle.

As to the detail document, please return somedays later and i will wirite a full document.

## 4. Which operating system will it support?

Only windows 7 above is supported. But if you do not use graphic support,it is likely to support most openating systems with g++ or others compilers that support C++. 

## 5. Somethings else

### How did this started?

I always wanted to start a porject which can help me to draw some picture and i think this is useful. I know there is some really really huge and perfect project like OpenCV, but it is just so big and sometimes hard to install(To be honest, It took me some effort to install OpenCV to my computer and make it work at the first time.), So I want to write a tiny small library myself.

But I am only a student who is new to programme, and I know nothing abou t how to start this until our C course finished and we are asked to write a graphics program. We are forced to use graphics.h with BorlandC++ 3.1 compiler. To make our programme looks a little more breatiful, I forced to learn the struct of a bmp file so that I can put a bmp picture on the screen. At that time, I think it is the time to start this project.

Then our C++ course finished, We are also asked to write something as our homework, I choose to write this and submitted it. This is how the programme really started.

thank you for your reading!.
