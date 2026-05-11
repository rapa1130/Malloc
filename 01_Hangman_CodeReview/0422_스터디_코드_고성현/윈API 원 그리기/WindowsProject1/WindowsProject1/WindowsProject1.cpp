
#include <iostream>
#include "INC_Windows.h"
#include "GeoShapeManager.h"	
#include "Enum.h"


namespace
{
	// @breif  윈도우 프로시저
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case WM_CREATE:
		{
			std::cout << "WM_CREATE" << std::endl;
		}break;

		case WM_CLOSE:
		{
			std::cout << "WM_CLOSE" << std::endl;
			// 윈도우를 종료하기 위한 메시지
			PostQuitMessage(0); // WM_QUIT
		}break;

		case WM_PAINT:
		{
			std::cout << "WM_PAINT" << std::endl;

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));

			simplegeo::g_GeoShapeManager.Draw(hdc);

			EndPaint(hwnd, &ps);

		}break;


		case WM_LBUTTONDOWN:
		{
			std::cout << "WM_LBUTTONDBLCLK" << std::endl;
			// 좌표를 가져온다.
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			/* 마우스 위치 기준 충돌 판정
			if (!simplegeo::g_GeoShapeManager.IsOverlapping(x, y))
			{
				simplegeo::g_GeoShapeManager.AddCircle(x, y, 10, RGB(255, 0, 0));
			}*/

			//마우스 위치에 그리는 도형 기준 충돌 판정
			if (!simplegeo::g_GeoShapeManager.IsOverlapping(x, y, ShapeType::Circle))
			{
				simplegeo::g_GeoShapeManager.AddCircle(x, y, 10, RGB(255, 0, 0));
			}
			//simplegeo::g_GeoShapeManager.AddCircle(x, y, 10, RGB(255, 0, 0));
			::InvalidateRect(hwnd, NULL, TRUE);
		}break;

		case WM_RBUTTONDOWN:
		{
			std::cout << "WM_RBUTTONDOWN" << std::endl;
			int x = LOWORD(lparam);
			int y = HIWORD(lparam);
			/*마우스 위치 기준 충돌 판정
			if (!simplegeo::g_GeoShapeManager.IsOverlapping(x, y))
			{
				simplegeo::g_GeoShapeManager.AddRectangle(x - 10, y - 10, x + 10, y + 10, RGB(0, 255, 0));
			}*/

			//마우스 위치에 그리는 도형 기준 충돌 판정
			if (!simplegeo::g_GeoShapeManager.IsOverlapping(x, y, ShapeType::Rectangle))
			{
				simplegeo::g_GeoShapeManager.AddRectangle(x - 10, y - 10, x + 10, y + 10, RGB(0, 255, 0));
			}
			//simplegeo::g_GeoShapeManager.AddRectangle(x - 10, y - 10, x + 10, y + 10, RGB(0, 255, 0));
			::InvalidateRect(hwnd, NULL, TRUE);
		}break;

		default:
			return::DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return NULL;
	}

	// @breif  윈도우 클래스 등록
	HWND CreateWidow(const wchar_t* className, const wchar_t* windowName, int width, int height)
	{
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpszClassName = className;
		wc.lpfnWndProc = WndProc; // 윈도우 프로시저(함수)의 포인터 등록


		ATOM classId = 0;
		if (!GetClassInfoEx(HINSTANCE(), L"NzWindowClass", &wc))
		{
			classId = RegisterClassEx(&wc);

			if (0 == classId) return NULL;
		}

		RECT rc = { 0, 0, width, height };

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

		std::cout << "width: " << rc.right - rc.left << " height: " << rc.bottom - rc.top << std::endl;

		HWND hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

		if (NULL == hWnd) return (HWND)(NULL);

		::SetWindowText((HWND)hWnd, windowName);

		ShowWindow((HWND)hWnd, SW_SHOW);
		UpdateWindow((HWND)hWnd);

		return hWnd;
	}

	void GameLoop()
	{
		MSG msg = {};
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, HWND(), 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				///HERE: GAME LOGIC UPATE & RENDERING
			}
		}
	}
}//namespace

int main()
{
	// GUI 창 생성
	HWND hWnd = CreateWidow(L"NzWindowClass", L"나전칠기", 1280, 720);

	if (NULL == hWnd)
	{
		std::cout << "윈도우 생성 실패" << std::endl;
		system("pause");
		return -1;
	}

	std::cout << "hWnd: " << hWnd << std::endl;
	//std::cout << (*hWnd).unused << std::endl; 런타임 에러!

	GameLoop();

	DestroyWindow(hWnd);

	return 0;

}