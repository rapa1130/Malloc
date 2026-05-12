#include "windows.h"
#include "tchar.h"
#include "Render.h"	

HWND g_hWnd = NULL;
BOOL g_bLoop = TRUE;
HINSTANCE g_hInstance = NULL;

//E0144오류 -> ?
TCHAR* g_ClassName = _T("Yena");
extern TCHAR* g_WindowName;

BOOL InitWindow(int width, int height);
void ResizeWindow(HWND hWnd, UINT NewWidth, UINT NewHeight);
BOOL MessagePump();
void MoveToCenter(HWND hwnd, int width, int height);
LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		::SetCursor(LoadCursor(g_hInstance, IDC_ARROW));
		break;

	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		LineUpdate(pt);
	}
	break;

	case WM_MOUSEMOVE:
	{
		g_VtxTemp.x = LOWORD(lParam);
		g_VtxTemp.y = HIWORD(lParam);
	}
	break;

	case WM_RBUTTONDOWN:
	{
		LineClear();
	}
	break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmfShow)
{
	if (!InitWindow(960, 600)) return 0;

	RenderTargetCreate(g_hWnd);
	if (!DataLoading())
	{
		g_bLoop = FALSE;
	}

	//메인 루프
	while (g_bLoop)
	{
		if (!MessagePump()) break;
		SceneRender();	 
	}

	//종료
	DataRelease();
	RenderTargetRelease();
}

BOOL InitWindow(int width, int height)
{
	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,::GetModuleHandle(NULL), NULL, NULL, 
		(HBRUSH)GetStockObject(DKGRAY_BRUSH), NULL,g_ClassName, NULL
	};
	RegisterClassEx(&wc);

	HWND hWnd = ::CreateWindow(g_ClassName, g_WindowName,WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0,
		width, height, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	if (hWnd == NULL) return FALSE;

	ResizeWindow(hWnd, width, height);
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	g_hWnd = hWnd;

	return TRUE;
}

int MessagePump()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(msg));
	while (1)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) return FALSE;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

void ResizeWindow(HWND hWnd, UINT width, UINT height)
{
	RECT oldrc;	::GetWindowRect(hWnd, &oldrc);
	DWORD sytle = (DWORD) ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(hWnd, GWL_EXSTYLE);
	RECT newrc = { 0, 0, (LONG)width, (LONG)height };

	::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);

	::SetWindowPos(hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top, newWidth, newHeight, SWP_SHOWWINDOW);

	MoveToCenter(hWnd, newWidth, newHeight);

}

void MoveToCenter(HWND hwnd, int width, int height)
{
	RECT rc = {};
	::GetClientRect(GetDesktopWindow(),&rc);
	rc.left = rc.right / 2 - width/2;
	rc.top = rc.bottom /2 - height / 2;

	::MoveWindow(hwnd, rc.left, rc.top, width, height, TRUE);
}