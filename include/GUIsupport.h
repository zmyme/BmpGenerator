#ifndef _GUI_SUPPORT_H
#define _GUI_SUPPORT_H


#include <d2d1.h>
#include <windows.h>
#include <cstdio>
#include <process.h>
#include <iostream>
#include <vector>
#include "bmp.h"

#define MAX_NAME 128

#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}

struct HwndRef;
class Window;
struct HwndRef
{
	HWND handle;
	Window* windows;

	HwndRef(HWND h,Window* w)
	{
		handle=h;
		windows=w;
	}
};

class Window
{
public:
	Window(void)
	{
		init();
	}
	Window(int w,int h,const char* name)
	{
		create(w,h,name);
	}
	int init(void)
	{
		hInstance = GetModuleHandle(0);

		wc.lpszClassName = NULL;
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = 0;
		wc.lpfnWndProc   = WndProc;
		wc.hInstance     = hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION) ;
		wc.hIconSm       = NULL ;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1) ;
		wc.lpszMenuName  = NULL;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;

		BitMapData=NULL;
		BitMapPic=NULL;
		bitmap=NULL;

		g_pD2DFactory=NULL;
		g_pRenderTarget=NULL;



		return 0;
	}
	int create(int w,int h,const char* name)
	{
		init();
		width=w;
		height=h;
		strcpy(WindowName,name);
		strcpy(ClassName,name);
		threadHandle = (HANDLE)_beginthreadex(NULL, 0, EntranceFunction,this, 0, &threadID);

		return 0;
	}
	int initWindow(void);
	LRESULT CALLBACK SpecProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
	int ShowPicture(Bmp &pic);



	static std::vector<HwndRef> HwndList;
	~Window()
	{
		if(threadHandle!=NULL)
		{
			WaitForSingleObject(threadHandle,INFINITE);
			CloseHandle(threadHandle);
		}
		for(unsigned int i=0;i<HwndList.size();i++)
		{
			if(HwndList[i].handle==hwnd)
			{
				HwndList.erase(HwndList.begin()+i);
			}
		}

		SAFE_RELEASE(g_pRenderTarget) ;
		SAFE_RELEASE(g_pD2DFactory) ;

		if(BitMapData!=NULL)
		{
			delete[] BitMapData;
			BitMapData=NULL;
		}
	}
private:
	HINSTANCE hInstance;
	HWND hwnd;
	WNDCLASSEX wc;
	MSG msg;
	char ClassName[MAX_NAME];
	char WindowName[MAX_NAME];
	int width;
	int height;

	ID2D1Factory*			g_pD2DFactory;	// Direct2D factory
	ID2D1HwndRenderTarget*	g_pRenderTarget;	// Render g_pRenderTarget
	INT8* BitMapData;
	Bmp *BitMapPic;
	ID2D1Bitmap *bitmap;

	unsigned int threadID;
	HANDLE threadHandle;

	int CreateD2DResource(void);
	int DrawBitmap(void);
	int CreateD2D1FomatBitmap(void);

	static LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

	static unsigned __stdcall EntranceFunction(void* arg)
	{
		Window* p=(Window*) arg;
		p->initWindow();

		return 0;
	}
};

#endif