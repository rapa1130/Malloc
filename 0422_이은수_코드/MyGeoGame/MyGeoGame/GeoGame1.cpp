#include "GeoGame1.h"
#include "DrawManager.h"
#include "Circle.h"
#include "RectangleBox.h"
#include <iostream>

constexpr int MAX_GAME_OBJECT_COUNT = 100;


bool GeoGame1::Initialize()
{
    const wchar_t* className = L"MyGeoGame1";
    const wchar_t* windowName = L"MyGeoGame1";

    if (false == __super::Create(className, windowName, 1424, 720)) return false;

    RECT rcClient = {};
    GetClientRect(m_hWnd, &rcClient);
    m_width = rcClient.right - rcClient.left;
    m_height = rcClient.bottom - rcClient.top;

    // 더블버퍼링
    m_hFrontDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hFrontDC);
    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    // 드로우 매니저 생성
    m_drawManager = new DrawManager();
    m_drawManager->Draw(m_hBackDC);
    return true;
}

void GeoGame1::Run()
{
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                GeoGame1::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_RBUTTONDOWN)
            {
                GeoGame1::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }
}

void GeoGame1::Finalize()
{
    // TODO : 필요시 메모리 해제 로직 추가
    __super::Destroy();
}

void GeoGame1::Update()
{
    LogicUpdate();

    // TODO : 필요시 FixedUpdate 추가
}

void GeoGame1::Render()
{
    // 백버퍼 깨끗이 비우기
    ::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);
    // 실제 렌더 작업
    m_drawManager->Draw(m_hBackDC);
    // 메모리 DC 에 그려진 결과를 실제 프론트 DC에 복사
    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

void GeoGame1::OnResize(int width, int height)
{
    std::cout << "__FUNCTION__" << std::endl;

    // TODO : 유틸에 있는 셋 스크린 사이즈 실행

    __super::OnResize(width, height);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    HANDLE hPrecBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    DeleteObject(hPrecBitmap);
}

void GeoGame1::OnClose()
{
    std::cout << "__FUNCTION__" << std::endl;

    SelectObject(m_hBackDC, m_hDefaultBitmap);

    DeleteObject(m_hBackBitmap);
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hFrontDC);
}

void GeoGame1::OnLButtonDown(int x, int y)
{
    std::cout << __FUNCTION__ << std::endl;
    std::cout << "x: " << x << ", y: " << y << std::endl;
    Vector2 vec = { (float)x, (float)y };
    Circle* c = new Circle(vec, 50);
    m_drawManager->CheckIntesectAndAdd(c);
}

void GeoGame1::OnRButtonDown(int x, int y)
{
    std::cout << __FUNCTION__ << std::endl;
    std::cout << "x: " << x << ", y: " << y << std::endl;
    Vector2 vec = { (float)x, (float)y };
    RectangleBox* r = new RectangleBox(vec, 100, 50);
    m_drawManager->CheckIntesectAndAdd(r);
}

void GeoGame1::LogicUpdate()
{
    // TODO : 개별 오브젝트 로직 업데이트
}