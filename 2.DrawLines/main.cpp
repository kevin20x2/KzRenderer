#include <windows.h>
#include "Device.h"



struct ScreenContext
{

	HWND ScreenHandle;
	int ScreenWidth;
	int ScreenHeight;
	HDC ScreenDC;
	int ScreenKeys[512];

};

ScreenContext GScreenContext;


LRESULT WINAPI MsgProc(HWND hWnd , UINT Msg, WPARAM wParam, LPARAM lParam )
{
    switch(Msg)
    {
        case WM_DESTROY :
        PostQuitMessage(0);
        exit(-1);
        return 0;
        default :
        return DefWindowProc(hWnd, Msg,wParam,lParam );
    }
    return 0;
}


// OutPut framebuffer
int InitWindow(int Width, int Height, HINSTANCE hInstance, void*& OutBuffer)
{
	WNDCLASS wc = {};

	wc.lpfnWndProc = MsgProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "KzRenderer";

	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	BITMAPINFO bi = { {sizeof(BITMAPINFOHEADER), Width,-Height ,1,32 ,BI_RGB, Width * Height * 4 ,0,0,0,0  } };

	LPVOID Ptr;

	if (!RegisterClass(&wc))
	{
		return -1;
	}
	HWND& ScreenHandle = GScreenContext.ScreenHandle;
	GScreenContext.ScreenWidth = Width;
	GScreenContext.ScreenHeight = Height;
	memset(GScreenContext.ScreenKeys, 0, sizeof(GScreenContext.ScreenKeys));

	ScreenHandle = CreateWindow("KzRenderer", "KzRendererTest",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);

	HDC hDC;
	hDC = GetDC(ScreenHandle);

	HDC& ScreenDC = GScreenContext.ScreenDC;

	ScreenDC = CreateCompatibleDC(hDC);
	ReleaseDC(ScreenHandle, hDC);

	HBITMAP ScreenHb = NULL;
	ScreenHb = CreateDIBSection(ScreenDC,&bi,DIB_RGB_COLORS,&Ptr,0,0);
	if (ScreenHb == nullptr)  return -1;

	HBITMAP  ScreenOB = NULL;
	ScreenOB = (HBITMAP)SelectObject(ScreenDC, ScreenHb);
	unsigned char* ScreenFrameBuffer =(unsigned char *) Ptr;

	RECT rect = {0,0,Width ,Height};

	AdjustWindowRect(&rect, GetWindowLong(ScreenHandle,GWL_STYLE),0);
	int wx, wy, sx, sy;

	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	
	SetWindowPos(ScreenHandle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(ScreenHandle);

	ShowWindow(ScreenHandle, SW_NORMAL);
	

	memset(ScreenFrameBuffer, 0xff, Width * Height * 4);

	OutBuffer = ScreenFrameBuffer;

	return 0;


}


void ScreenDispatch()
{
	MSG Msg;
	while (true)
	{
		if (!PeekMessage(&Msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&Msg, NULL, 0, 0)) break;
		DispatchMessage(&Msg);
	}
}

void ScreenUpdate()
{
	HDC hDC = GetDC(GScreenContext.ScreenHandle);
	BitBlt(hDC, 0, 0, GScreenContext.ScreenWidth, GScreenContext.ScreenHeight,GScreenContext.ScreenDC , 0, 0, SRCCOPY);
	ReleaseDC(GScreenContext.ScreenHandle, hDC);
	ScreenDispatch();
}

INT WINAPI WinMain(
    HINSTANCE hInstance ,
    HINSTANCE hPrevInstance ,
    LPSTR lpCmdLine ,
    int nShowCmd
)
{
	void* FrameBuf = nullptr;
	int Width = 800;
	int Height = 600;
	InitWindow(Width,Height, hInstance,FrameBuf);

	Device* pDevice = new Device(FrameBuf, Width, Height);

	while (true)
	{
		ScreenDispatch();
		pDevice->DrawLine(FVector2(0, 0), FVector2(1, 1),FColor(1,0,0));

		ScreenUpdate();

	}
	return 0;

}
