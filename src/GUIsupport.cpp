#include "GUIsupport.h"

std::vector<HwndRef> Window::HwndList;

int Window::initWindow(void)
{
	wc.lpszClassName=ClassName;
	if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        ClassName,
        WindowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width,height,
        NULL,
        NULL,
        hInstance,
        NULL
        );
    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    CreateD2DResource();
    HwndList.push_back(HwndRef(hwnd,this));

    ShowWindow(hwnd,10);
    UpdateWindow(hwnd);
    ZeroMemory(&msg,sizeof(msg));

    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	int i=0;
    int numWindow=Window::HwndList.size();
    for(i=0;i<numWindow;i++)
    {
    	if(Window::HwndList[i].handle==hwnd)
    	{
    		(Window::HwndList[i].windows)->SpecProc(hwnd,msg,wParam,lParam);
    	}
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::SpecProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch(msg)
    {
        case WM_MOVE:
        case WM_SIZE:
            DrawBitmap();
            break;
    	case WM_PAINT:
        case WM_PRINT:
            if(g_pRenderTarget==NULL)
            {
                CreateD2DResource();
            }
            DrawBitmap();
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return 0;
}

int Window::CreateD2DResource(void)
{
    if (!g_pRenderTarget)
    {
        HRESULT hr ;

        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &g_pD2DFactory) ;
        if (FAILED(hr))
        {
            MessageBox(hwnd, "Create D2D factory failed!", "Error", 0) ;
            return 1;
        }

        // Obtain the size of the drawing area
        RECT rc ;
        GetClientRect(hwnd, &rc) ;

        // Create a Direct2D render target
        hr = g_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
            hwnd, 
            D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top)
            ), 
            &g_pRenderTarget
            ) ;
        if (FAILED(hr))
        {
            MessageBox(hwnd, "Create render target failed!", "Error", 0) ;
            return 2;
        }
    }

    return 0;
}

int Window::ShowPicture(Bmp &pic)
{
    BitMapPic=&pic;
    if((pic.width()>width)|(pic.height()>height))
    {
        MessageBox(hwnd, "Bitmap file is larger than the window!", "Error", 0) ;
    }

    int x=0;
    int y=0;
    if(BitMapData==NULL)
    {
        BitMapData=new INT8[width*height*4];
    }
    for(int i=0;i<pic.width()*pic.height()*4;i+=4)
    {
        BitMapData[i]=pic.getpixel(x,y).Blue;
        BitMapData[i+1]=pic.getpixel(x,y).Green;
        BitMapData[i+2]=pic.getpixel(x,y).Red;
        BitMapData[i+3]=0x5f;

        if(x<pic.width()-1) x+=1;
        else
        {
            x=0;
            y+=1;
        }
    }
    CreateD2D1FomatBitmap();
    SendMessage(hwnd,WM_PRINT,0,0);

    return 0;
}

int Window::CreateD2D1FomatBitmap(void)
{
    if(bitmap!=NULL) return 0;

    if(g_pRenderTarget==NULL)
    {
        CreateD2DResource();
    }
    int picwidth=BitMapPic->width();
    int picheight=BitMapPic->height();

    D2D1_BITMAP_PROPERTIES pro;
    D2D1_PIXEL_FORMAT PixFormat;
    PixFormat.format=DXGI_FORMAT_B8G8R8A8_UNORM;
    PixFormat.alphaMode=D2D1_ALPHA_MODE_IGNORE;
    pro.pixelFormat=PixFormat;
    pro.dpiX=150;
    pro.dpiY=150;

    g_pRenderTarget->CreateBitmap(D2D1::SizeU(picwidth,picheight),pro,&bitmap);

    return 0;
}

int Window::DrawBitmap(void)
{
    if((BitMapData==NULL)|(g_pRenderTarget==NULL)) return 0;

    g_pRenderTarget->BeginDraw() ;
    // Clear background color to dark cyan
    g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
    
    D2D1_RECT_U area;
    area.left=0;
    area.right=BitMapPic->width();
    area.top=0;
    area.bottom=BitMapPic->height();
    bitmap->CopyFromMemory(&area,(void*) BitMapData,(UINT32) width*4);

    // Draw bitmap
    g_pRenderTarget->DrawBitmap(bitmap,D2D1::RectF(0,0,BitMapPic->width(),BitMapPic->height()));

    HRESULT hr = g_pRenderTarget->EndDraw();
    if (FAILED(hr))
    {
        MessageBox(NULL, "Draw failed!", "Error", 0) ;
        return 1;
    }

    return 0;
}