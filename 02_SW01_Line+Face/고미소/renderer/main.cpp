
#include "Render.h"

// 전역 데이터들
HWND g_hWnd = NULL;
BOOL g_bLoop = TRUE;
HINSTANCE g_hInstance = NULL;

const TCHAR* g_ClassName = _T("KohMS");
extern const TCHAR* g_WindowName;

// 윈도우 설정 관련 함수들
BOOL InitWindow(int width, int height);
void ResizeWindow(HWND hWnd, UINT NewWidth, UINT NewHeight);
BOOL MessagePump();
void MoveToCenter(HWND hWnd, int width, int height);
LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);


// 애플리케이션 메임 함수
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
    if (!InitWindow(960, 600)) return 0;

    // 렌더 타켓 생성
    RenderTargetCreate(g_hWnd);

    // 데이터 로딩
    if (!DataLoading()) { g_bLoop = FALSE; }

    // 게임 루프
    while (g_bLoop)
    {
        if (!MessagePump()) { break; }

        SceneRender();
    }

    // 애플리케이션 종료
    DataRelease(); // 데이터 제거
    RenderTargetRelease(); // 렌더 타켓 제거

    return 0;
}

//
//   윈도우 생성 함수
BOOL InitWindow(int width, int height)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
            ::GetModuleHandle(NULL), NULL, NULL,
            (HBRUSH)GetStockObject(DKGRAY_BRUSH), NULL,
            g_ClassName, NULL };
    RegisterClassEx(&wc);

    HWND hWnd = ::CreateWindow(g_ClassName, g_WindowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        //WS_OVERLAPPEDWINDOW,
        0, 0,
        width, height,
        GetDesktopWindow(), NULL,
        wc.hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ResizeWindow(hWnd, width, height); // 윈도우 크기 조정

    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);
    ::SetCursor(LoadCursor(NULL, IDC_ARROW));

    g_hWnd = hWnd; // 윈도우 핸들 저장

    return TRUE;
}

//
//  윈도우 메세지 콜백 함수
LRESULT CALLBACK MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        ::SetCursor(LoadCursor(g_hInstance, IDC_ARROW));
        //::SetCursorPos(g_Mode.cx/2, g_Mode.cy/2);
        break;
        /*case WM_PAINT:
            {
                char* msg;
                PAINTSTRUCT rc;
                HDC hdc = BeginPaint(hwnd, &rc);
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkMode(hdc, TRANSPARENT);
                msg = "Software Renderer Project :  \" B3Yena \"";
                TextOut(hdc, 270, 250, msg, strlen(msg));
                msg = "Hello, Yena!";
                TextOut(hdc, 350, 270, msg, strlen(msg));
                EndPaint(hwnd, &rc);
            }
            break;
        */
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
        //POINT pt; 
        g_VtxTemp.x = LOWORD(lParam);
        g_VtxTemp.y = HIWORD(lParam);
        //LineUpdate(pt);
    }
    break;
    case WM_RBUTTONDOWN:
    {
        LineClear();			//★
    }
    break;
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
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 
// 메세지 펌핑 함수
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

//
// 윈도우 크기를 조정하는 함수
void ResizeWindow(HWND hWnd, UINT width, UINT height)
{
    //현재 윈도 정보 획득. 
    RECT oldrc;	::GetWindowRect(hWnd, &oldrc);
    DWORD sytle = (DWORD) ::GetWindowLong(hWnd, GWL_STYLE);
    DWORD exstyle = (DWORD) ::GetWindowLong(hWnd, GWL_EXSTYLE);

    //새로 생성될 윈도우의 '클라이언트' 영역.
    RECT newrc = { 0, 0, (LONG)width, (LONG)height };

    //newrc 만큼의 클라이언트 영역을 포함하는 윈도 '전체' 크기를 구합니다.
    //현재 메뉴는 없다는 가정하에 처리되고 있습니다.
    //계산된 결과는 newrc 에 다시 들어옵니다. (스크린 좌표)
    //exstyle &= ~WS_EX_TOPMOST;
    ::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

    //보정된 윈도우의 너비와 폭을 구합니다. 
    int newWidth = (newrc.right - newrc.left);
    int newHeight = (newrc.bottom - newrc.top);

    //새로운 크기를 윈도우에 설정합니다.
    ::SetWindowPos(hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top,
        newWidth, newHeight, SWP_SHOWWINDOW);

    //화면 중앙으로 이동.
    MoveToCenter(hWnd, newWidth, newHeight);

}

//
// MoveToCenter 윈도우를 화면 중앙으로 이동시키는 함수
void MoveToCenter(HWND hwnd, int width, int height)
{
    RECT rc = {};
    ::GetClientRect(GetDesktopWindow(), &rc);
    rc.left = rc.right / 2 - width / 2;
    rc.top = rc.bottom / 2 - height / 2;

    ::MoveWindow(hwnd, rc.left, rc.top, width, height, TRUE);
    //::SetWindowPos(hwnd, HWND_NOTOPMOST, rc.left, rc.top, width, height, SWP_SHOWWINDOW);
}