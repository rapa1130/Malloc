#include "windows.h"
#include "tchar.h"
#include "Device.h"		
#include "Render.h"

// 전역 데이터
HWND g_hWnd = NULL;
BOOL g_bLoop = TRUE;
HINSTANCE g_hInstance = NULL;

TCHAR* g_ClassName = _T("Yena");
extern TCHAR* g_WindowName;


//윈도우 설정 관련 함수
BOOL InitWindow		(int width, int height);
void ResizeWindow	(HWND hWnd, UINT NewWidth, UINT NewHeight);
BOOL MessagePump	();
void MoveToCenter	(HWND hwnd, int width, int height);

LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	if(!InitWindow(g_Mode.Width, g_Mode.Height)) return 0;

	if(FAILED(YenaSetup(g_hWnd)))
		return 0;

	if(!DataLoading())
	{
		g_bLoop = FALSE;  
	}

	while(g_bLoop) 
	{
		if(!MessagePump())		
			break;
 
		SceneRender();			 
	}

	DataRelease();				
	YenaRelease();			

	return 0;
}


LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{ 
	case WM_KEYDOWN:
		switch(wParam)
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


BOOL InitWindow(int width, int height)
{
	WNDCLASSEX wc = { 
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
			::GetModuleHandle(NULL), NULL, NULL, 
			(HBRUSH)GetStockObject(DKGRAY_BRUSH), NULL, 
			g_ClassName, NULL 
	};
	RegisterClassEx(&wc);
 
	HWND hWnd = ::CreateWindow( g_ClassName, g_WindowName, 
								WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
								//WS_OVERLAPPEDWINDOW, 
								0, 0, 
								width, height, 
								GetDesktopWindow(), NULL, 
								wc.hInstance, NULL
								);
	if(hWnd == NULL) return FALSE; 

	//윈도우 크기 조정
	ResizeWindow(hWnd, width, height);

	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
	::SetCursor(LoadCursor(NULL, IDC_ARROW));

	//윈도우 핸들 저장
	g_hWnd = hWnd;

	return TRUE;
}


int MessagePump()
{
	MSG msg;	::ZeroMemory(&msg, sizeof(msg));

	while(1)
	{
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				return FALSE;		
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
	DWORD sytle   = (DWORD) ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(hWnd, GWL_EXSTYLE);
	
	RECT newrc = { 0, 0, (LONG)width, (LONG)height };
	
	::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);

	::SetWindowPos(hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top, 
 					newWidth, newHeight, SWP_SHOWWINDOW);

	MoveToCenter(hWnd, newWidth, newHeight);
}


void MoveToCenter(HWND hwnd, int width, int height)
{
	RECT rc = {};
	::GetClientRect(GetDesktopWindow(), &rc);
	rc.left = rc.right/2 - width/2;
	rc.top  = rc.bottom/2 - height/2;
		
	::MoveWindow(hwnd, rc.left, rc.top, width, height, TRUE);
}