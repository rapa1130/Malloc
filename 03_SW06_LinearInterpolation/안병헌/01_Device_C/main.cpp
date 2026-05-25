#include<Windows.h>
#include<tchar.h>
#include"Render.h"
#include"Device.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitWindow(int width, int height);
void ResizeWindow(HWND hWnd, int width, int height);
void MoveToCenter(HWND hWnd, int width, int height);
BOOL MessagePump();



HWND g_hWnd;
HINSTANCE g_hInstance;
BOOL g_loop = true;


TCHAR* g_className = _T("ByeongHeon");
extern TCHAR* g_windowName;


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	if (!InitWindow(g_mode.width, g_mode.height)) return 0;

	if (BH_FAILED(ByeongHeonSetUp(g_hWnd))) return 0;

	if (!DataLoading()) {
		g_loop = FALSE;
	}

	while (g_loop)
	{
		if (!MessagePump()) break;
		SceneRender();
	}

	DataRelease();
	ByeongHeonRelease();

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


BOOL InitWindow(int width, int height)
{

	WNDCLASSEX wc = { 0, };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = g_className;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(g_className, g_windowName,
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0,
		width, height,
		GetDesktopWindow(), NULL,
		wc.hInstance, NULL
	);
	if (hWnd == NULL) return FALSE;

	ResizeWindow(hWnd, width, height);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	g_hWnd = hWnd;
	return TRUE;
}


void ResizeWindow(HWND hWnd, int width, int height)
{
	RECT oldRc; GetWindowRect(hWnd, &oldRc);
	DWORD style = (DWORD)GetWindowLong(hWnd, GWL_STYLE);
	DWORD exStyle = (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE);

	RECT newRc = { 0,0,(LONG)width,(LONG)height };
	AdjustWindowRectEx(&newRc, style, NULL, exStyle);

	int newWidth = newRc.right - newRc.left;
	int newHeight = newRc.bottom - newRc.top;

	SetWindowPos(hWnd, HWND_TOPMOST, oldRc.left, oldRc.top,
		newWidth, newHeight, SWP_SHOWWINDOW);

	MoveToCenter(hWnd, newWidth, newHeight);
}


void MoveToCenter(HWND hWnd, int width, int height)
{
	RECT rc = { };
	GetClientRect(GetDesktopWindow(), &rc);
	rc.left = rc.right / 2 - width / 2;
	rc.top = rc.bottom / 2 - height / 2;

	MoveWindow(hWnd, rc.left, rc.top, width, height, TRUE);
}


BOOL MessagePump()
{
	MSG msg = {};
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return FALSE;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;

}