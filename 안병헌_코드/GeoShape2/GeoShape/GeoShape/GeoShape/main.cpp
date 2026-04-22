#include"INC_Windows.h"
#include<Windows.h>
#include<iostream>
#include"GeoShapeManager.h"
//Wnd Proc 정의
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
	{
	}break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
		simpleGeo::g_geoShapeManager.Draw(hdc);
		EndPaint(hwnd, &ps);
	}break;
		
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lp);
		int y = HIWORD(lp);
		simpleGeo::g_geoShapeManager.AddCircle(x, y, 30, RGB(255, 0, 0));
		::InvalidateRect(hwnd, NULL, TRUE);
	}break;
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lp);
		int y = HIWORD(lp);
		int length = 30;
		simpleGeo::g_geoShapeManager.AddRectangle(x - length, x + length, y - length, y + length, RGB(255,0,0));
		::InvalidateRect(hwnd, NULL, TRUE);
	}break;
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}break;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
	return NULL;
}

//CreateMyWindow 함수 정의
HWND CreateMyWindow(const wchar_t* className,const wchar_t* windowName, int width,int height)
{
	HINSTANCE hInst = GetModuleHandle(nullptr);
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = className;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;

	ATOM classId = 0;
	if (!GetClassInfoEx(hInst, className, &wc))
	{
		classId = RegisterClassEx(&wc);
		if (classId == 0) return NULL;
	}
	RECT rc = { 0,0,width,height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	HWND hwnd = CreateWindowEx(NULL, className, windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL);

	if (hwnd == NULL) return (HWND)NULL;
	//::SetWindowText(hwnd, windowName);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return hwnd;
}

void GameLoop(HWND hwnd)
{
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}
}

int main()
{
	//윈도우 생성
	HWND hwnd = CreateMyWindow(L"NzWindowClass", L"나전칠기", 1280, 720);

	if (hwnd == NULL)
	{
		return -1;
	}

	GameLoop(hwnd);

	DestroyWindow(hwnd);
	return 0;
}