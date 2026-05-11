//콘솔 출력용
#include "windows.h"
#include "tchar.h"
#include "Render.h"

////////////////////////////////////////////////////////////
//
// 전역 데이터 정의 영역
//
#define MC_WIN_WIDTH 960
#define MC_WIN_HEIGTH 600

HWND g_hWnd = NULL;
BOOL g_bIsLoop = TRUE;
HINSTANCE g_hInstance = NULL;

TCHAR* g_ClassName = _T("Mia");
extern TCHAR* g_WindowName;


////////////////////////////////////////////////////////////
//
// 함수 선언 영역
//
BOOL InitWindow(int width, int height);
void ResizeWindow(HWND hWnd, UINT width, UINT height);
BOOL MessagePump();
void MoveToCenter(HWND hwnd, int width, int height);

LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);


////////////////////////////////////////////////////////////
//
// 메인 시작 함수
//
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	//메인 윈도우 생성
	if (!InitWindow(MC_WIN_WIDTH, MC_WIN_HEIGTH)) return 0;

	CreateRenderTarget(g_hWnd);  //Back-Buffer : Render Target 생성

	//데이터 로딩 진행 - 실패하면 루프 진입 안 함
	if (!DataLoading())
	{
		g_bIsLoop = FALSE;
	}

	//프로그램 메인 루프
	while (g_bIsLoop)
	{
		if (!MessagePump())  //메세지 펌프
			break;

		SceneRender();			//장면 렌더링	 
	}

	//프로그램 종료
	DataRelease();				//데이터 제거
	ReleaseRenderTarget();  //Back-Buffer : Render Target 제거

	return 0;
}



////////////////////////////////////////////////////////////
//
// 메시지 콜백 함수
//
LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		::SetCursor(LoadCursor(g_hInstance, IDC_ARROW));
		//::SetCursorPos(g_Mode.cx/2, g_Mode.cy/2); <- 뭔지 모르겠음
		break;

	case WM_MOUSEMOVE:
		{
			g_VtxTemp.x = LOWORD(lParam);
			g_VtxTemp.y = HIWORD(lParam);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			UpdateLine(pt);
		}
		break;

	case WM_RBUTTONDOWN:
		{
			ClearLine();
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



////////////////////////////////////////////////////////////
//
// 윈도우 생성 함수
//
BOOL InitWindow(int width, int height)
{
	// 윈도우 창의 설정 및 클래스 등록
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
		::GetModuleHandle(NULL), NULL, NULL,
		(HBRUSH)GetStockObject(DKGRAY_BRUSH), NULL,
		g_ClassName, NULL
	};
	RegisterClassEx(&wc);

	// 실제 창을 생성
	HWND hWnd = ::CreateWindow(g_ClassName, g_WindowName,
							WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
							0, 0,
							width, height,
							GetDesktopWindow(), NULL,
							wc.hInstance, NULL);
	if (hWnd == NULL) return FALSE;

	// 윈도우 크기 조정
	ResizeWindow(hWnd, width, height);

	::ShowWindow(hWnd, SW_SHOWDEFAULT);  //창 화면에 표시
	::UpdateWindow(hWnd);  //창 즉시 다시 그림 (WM_PAINT 메시지 강제 발송)
	::SetCursor(LoadCursor(NULL, IDC_ARROW));  //마우스 커서 설정

	// 윈도우 핸들 저장
	g_hWnd = hWnd;

	return TRUE;
}



////////////////////////////////////////////////////////////
//
// 윈도우 크기 조정 함수
//
void ResizeWindow(HWND hWnd, UINT width, UINT height)
{
	// 현재 윈도우 정보 저장
	RECT oldrc;
	::GetWindowRect(hWnd, &oldrc);
	DWORD style = (DWORD)::GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD)::GetWindowLong(hWnd, GWL_EXSTYLE);

	// 윈도우의 클라이언트 영역
	RECT newrc = { 0, 0, (LONG)width, (LONG)height };

	// newrc 만큼의 클라이언트 영역을 포함해서 윈도우 전체 크기를 계산
	// newrc에 다시 저장
	::AdjustWindowRectEx(&newrc, style, NULL, exstyle);

	// 보정된 윈도우의 너비와 폭 구함
	int newWidth = (newrc.right - newrc.left);
	int newHeigth = (newrc.bottom - newrc.top);

	// 보정된 크기로 윈도우 새로 설정
	::SetWindowPos(hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top,
		newWidth, newHeigth, SWP_SHOWWINDOW);

	// 화면 중앙에 위치
	MoveToCenter(hWnd, newWidth, newHeigth);
}



////////////////////////////////////////////////////////////
//
// 윈도우 위치 조정 함수
//
void MoveToCenter(HWND hwnd, int width, int height)
{
	RECT rc = {};
	::GetClientRect(GetDesktopWindow(), &rc);
	rc.left = rc.right / 2 - width / 2;
	rc.top = rc.bottom / 2 - height / 2;

	::MoveWindow(hwnd, rc.left, rc.top, width, height, TRUE);

	// Z순서(앞뒤 레이어), 플래그 등 더 세밀한 제어 가능
	//::SetWindowPos(hwnd, HWND_NOTOPMOST, rc.left, rc.top, width, height, SWP_SHOWWINDOW);
}






////////////////////////////////////////////////////////////
//
// 메시지 펌핑 함수
// 메인 루프에서 프레임 마다 호출됨
// 종료 이벤트(ESC) 발생하면 루프를 빠져나온다
// 
// return : 프로그램 현재 동작 상태 - WM_QUIT일 때 FALSE나옴
//
BOOL MessagePump()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(msg));

	while (TRUE)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return FALSE;
			}

			// 여기서 메시지 처리
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			// 메시지가 없을 땐 렌더링 진행
			return TRUE;
		}
	}

	return FALSE;
}


