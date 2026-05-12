


#include "windows.h"
#include "tchar.h"
#include "Render.h"


HWND g_hWnd = NULL;
BOOL g_bLoop = TRUE;
HINSTANCE g_hIntance = NULL;

const TCHAR* g_ClassName = L"LESEngine";
const extern TCHAR* g_WindowName;

// 윈도우 설정 함수
BOOL InitWindow(int width, int height);
void ResizeWindow(HWND hWnd, UINT newWidth, UINT newHeight);
BOOL MessagePump();
void MoveToCenter(HWND hWnd, int width, int height);

LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);


// 애플리케이션 메인 함수.
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,	int nCmdShow)
{

	//기본 윈도 생성.
	if (!InitWindow(960, 600)) return 0;

	// 백버퍼 생성
	RenderTargetCreate(g_hWnd);

	//데이터 로딩.
	if (!DataLoading())
	{
		g_bLoop = FALSE;
	}

	//메인 게임 루프.
	while (g_bLoop)
	{
		if (!MessagePump())		//메세지 펌프.
			break;

		SceneRender();			//장면 렌더링.	 
	}

	//애플리케이션 종료.
	DataRelease();				//데이터 제거.
	RenderTargetRelease();		// 백버퍼 제거

	return 0;
}

// 윈도 메시지 콜백 함수
LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		::SetCursor(LoadCursor(g_hIntance, IDC_ARROW));
		break;
	case WM_MOUSEMOVE:
		g_currentPoint.x = LOWORD(lParam);
		g_currentPoint.y = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		LineUpdate(pt);
		break;
	}
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

// 윈도우 생성 함수
BOOL InitWindow(int width, int height)
{
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
			::GetModuleHandle(NULL), NULL, NULL,
			(HBRUSH)GetStockObject(DKGRAY_BRUSH), NULL,
			g_ClassName, NULL
	};
	RegisterClassEx(&wc);

	HWND hWnd = ::CreateWindow(g_ClassName, g_WindowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0,
		width, height,
		GetDesktopWindow(), NULL,
		wc.hInstance, NULL
	);
	if (hWnd == NULL) return FALSE;
	ResizeWindow(hWnd, width, height);

	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
	::SetCursor(LoadCursor(NULL, IDC_ARROW));

	//윈도우 핸들 저장.
	g_hWnd = hWnd;

	return TRUE;
}

// 메세지 펌핑 함수.
int MessagePump()
{
	MSG msg;	::ZeroMemory(&msg, sizeof(msg));

	while (1)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return FALSE;

			//메세지 처리...
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			//특별한 메세지가 없다면 리턴후 게임(렌더링)을 진행합니다..
			return TRUE;
		}
	}

	return FALSE;
}

//윈도우 크기를 조정 
void ResizeWindow(HWND hWnd, UINT width, UINT height)
{
	//현재 윈도 정보 획득. 
	RECT oldrc;	::GetWindowRect(hWnd, &oldrc);
	DWORD sytle = (DWORD) ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(hWnd, GWL_EXSTYLE);
	RECT newrc = { 0, 0, (LONG)width, (LONG)height };

	::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);

	::SetWindowPos(hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top,
		newWidth, newHeight, SWP_SHOWWINDOW);

	//화면 중앙으로 이동.
	MoveToCenter(hWnd, newWidth, newHeight);
}

// 윈도우를 화면 중앙으로 이동
void MoveToCenter(HWND hwnd, int width, int height)
{
	RECT rc = {};
	::GetClientRect(GetDesktopWindow(), &rc);
	rc.left = rc.right / 2 - width / 2;
	rc.top = rc.bottom / 2 - height / 2;

	::MoveWindow(hwnd, rc.left, rc.top, width, height, TRUE);
}