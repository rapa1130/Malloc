#include"Render.h"
#include<iostream>
#include<tchar.h>
#include<windows.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

BOOL PumpMessage();
BOOL InitializeWindow(int width, int height);
void MoveCenter(HWND hwnd,int width,int height);
void ResizeWindow(HWND hwnd, int width, int height);

HWND g_Hwnd = NULL;
HINSTANCE g_hInstance = NULL;
TCHAR* g_ClassName = _T("BH");
TCHAR* g_WindowName = _T("Byeong heon");

extern bool g_bVertical;
extern bool g_bHorizontal;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,LPSTR param , int nShowCnt)
{
	if (!InitializeWindow(950,600))
	{
		return 0;
	}
	LoadData();
	CreateRenderTarget();

	while (true)
	{
		if (!PumpMessage()) break;

		RenderScene();
	}

	ReleaseRenderTarget();
	ReleaseData();

	return 0;
}

BOOL InitializeWindow(int width,int height)
{
	WNDCLASSEX wc = { 
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		::GetModuleHandle(NULL),
		NULL,
		NULL,
		(HBRUSH)GetStockObject(DKGRAY_BRUSH),
		NULL,
		g_ClassName,
		NULL
	};
	
	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			g_ClassName,
			g_WindowName,
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			0,
			0,
			width,
			height,
			GetDesktopWindow(),
			NULL,
			wc.hInstance,
			NULL
		);

	if (hWnd == NULL) return FALSE;

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	ResizeWindow(hWnd, width, height);

	g_Hwnd = hWnd;
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		SetCursor(LoadCursor(g_hInstance, IDC_ARROW));
		return 0;
	case WM_LBUTTONDOWN:
	{
		POINT pt = { LOWORD(lParam),HIWORD(lParam) };
		UpdateLine(pt);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		LineClear();
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		POINT pt = { LOWORD(lParam),HIWORD(lParam) };
		g_TempVtx = pt;
		return 0;
	}
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(NULL);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL PumpMessage()
{
	MSG msg = {0,}; 

	while (PeekMessage(&msg,NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return FALSE;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return TRUE;
}

void ResizeWindow(HWND hwnd, int width, int height)
{
	RECT oldrc = { 0, }; 
	GetWindowRect(hwnd, &oldrc);
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	DWORD exstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	
	RECT newrc = { 0,0, (LONG)width, (LONG)height };
	AdjustWindowRectEx(&newrc, style, NULL, exstyle);
	
	int newWidth = newrc.right - newrc.left;
	int newHeight = newrc.bottom - newrc.top;
	SetWindowPos(hwnd, HWND_NOTOPMOST, oldrc.left, oldrc.top, newWidth, newHeight, SWP_SHOWWINDOW);

	MoveCenter(hwnd, newWidth, newHeight);
}

void MoveCenter(HWND hwnd, int width, int height)
{
	RECT rc = {0,};
	GetClientRect(GetDesktopWindow(), &rc);	

	rc.left = rc.right / 2 - width / 2;
	rc.top = rc.bottom / 2 - height / 2;

	MoveWindow(hwnd, rc.left, rc.top, width, height, TRUE);
}