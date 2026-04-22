// @brief  나전칠기 프로그래밍 기초 연습장
#include "INC_Windows.h"
#include "GameTimer.h"
#include "GeoShapeManager.h"
#include <iostream>

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

			PostQuitMessage(0); // WM_QUIT
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

	void GameInit(HWND hWnd)
	{
		RECT rcClient = {};
		GetClientRect(hWnd, &rcClient);

		const int circleRadius = 50;
		const int& clientWidth = rcClient.right - rcClient.left;
		const int& clientHeight = rcClient.bottom - rcClient.top;

		simplegeo::g_GeoShapeManager.AddCircle(clientWidth / 2, clientHeight / 2, circleRadius, RGB(0, 0, 255));
	}

	void GameUpdate(float deltaTime)
	{
		static float acculatedTime = 0;

		acculatedTime += deltaTime;

		if (acculatedTime < 1000.0) return;

		acculatedTime -= 1000.0;
	}

	void GameRender(HWND hWnd)
	{
		HDC hDC = GetDC(hWnd);

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		FillRect(hDC, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));

		simplegeo::g_GeoShapeManager.Draw(hDC);

		ReleaseDC(hWnd, hDC);
	}

	void GameLoop(HWND hWnd)
	{
		GameTimer timer;
		timer.Reset();

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
				timer.Tick();
				GameUpdate(timer.DeltaTimeMS());
				GameRender(hWnd);
			}
		}
	}
}//namespace

int main()
{
	HWND hWnd = CreateWidow(L"NzWindowClass", L"원하나", 1280, 720);

	if (NULL == hWnd)
	{
		std::cout << "윈도우 생성 실패" << std::endl;
		system("pause");
		return -1;
	}

	GameInit(hWnd);

	GameLoop(hWnd);

	DestroyWindow(hWnd);

	return 0;

}