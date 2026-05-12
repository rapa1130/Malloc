

#pragma warning(disable:4267 4996)
#include "Windows.h"
#include "stdio.h"
#include "tchar.h"
#include "Render.h"
#include <vector>
#include <cmath>

const TCHAR* g_WindowName = L"LESEngine::SW Renderer";
extern HWND g_hWnd;

POINT g_p1, g_p2, g_p3, g_currentPoint;
POINT g_clear{ g_clear.x = 0, g_clear.y = 0 };

enum 
{
	PT_NONE_ = 0,		// 점 없음, 초기상태.
	PT_AWAIT = 1,		// 입력 대기 상태.
	PT_P1_ = 2,			// 1번째 점 입력 완료 시
	PT_P2_ = 3,			// 2번째 점 입력 완료 시 
	PT_COMPLETED_ = 4   // 3번째 점 까지 모든 점 입력 완료 시
};

DWORD g_PtCheck = PT_NONE_;

// 렌더타겟 및 백버퍼용 변수
HBITMAP  g_hBmpRT = NULL;				
HDC		 g_hSurfaceRT = NULL;			
COLORREF g_BkColor = RGB(0, 0, 255);
#define  g_hRT   g_hSurfaceRT

// 라인 그리는 펜
HPEN g_hPenDash;
HPEN g_hPenWhite;
HPEN g_hPenGreen;

void ShowInfo();

int DataLoading()
{
	LineClear();
	// 펜 생성
	g_hPenDash = CreatePen(PS_DASH, 1, RGB(255, 255, 255));
	g_hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	g_hPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    return TRUE;
}

void DataRelease()
{

}



void LineUpdate(POINT pt)
{
	switch (g_PtCheck)
	{
	case PT_NONE_:
		g_PtCheck++;
	// 초기 실행 시 바로 AWAIT 으로 대기
	case PT_COMPLETED_:
		g_PtCheck = PT_AWAIT;
	// 세번째 점 입력시 바로 첫번쨰 부터 입력 받을 수 있게
	case PT_AWAIT:
		g_p1 = pt;
		g_PtCheck = PT_P1_;
		break;
	case PT_P1_:
		g_p2 = pt;
		g_PtCheck = PT_P2_;
		break;
	case PT_P2_:
		g_p3 = pt;
		g_PtCheck = PT_COMPLETED_;
		break;
	}
}

void LineDraw()
{
	switch (g_PtCheck)
	{
	case PT_NONE_ || PT_AWAIT:
		LineClear();
		break;
	case PT_P1_:
		CrossDraw(g_p1);
		LineDashDraw(g_p1, g_currentPoint);
		break;
	case PT_P2_:
		CrossDraw(g_p1);
		CrossDraw(g_p2);

		LineDraw(g_p1, g_p2);

		LineDashDraw(g_p1, g_currentPoint);
		LineDashDraw(g_p2, g_currentPoint);
		break;
	case PT_COMPLETED_:
		CrossDraw(g_p1);
		CrossDraw(g_p2);
		CrossDraw(g_p3);

		LineDraw(g_p1, g_p2);
		LineDraw(g_p2, g_p3);
		LineDraw(g_p3, g_p1);
		break;
	}
}


void LineDraw(POINT sp, POINT ep)
{
	//MoveToEx(g_hRT, sp.x, sp.y, NULL);
	//LineTo(g_hRT, ep.x, ep.y);
	int x0 = sp.x, y0 = sp.y;
	int x1 = ep.x, y1 = ep.y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	// 같은 점이면 리턴
	if (dy == 0 && dx == 0) return;

	if (dx >= dy)
	{
		// 수평에 가까운 선은 x를 주축으로 => y가 1증가할때마다 가로로  2픽셀 이상 찍어야 할때

		// 작은 X 좌표를 x0로 오게 정렬
		if (x0 > x1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		// x가 하나 증가할때마다 y는 얼만큼 증가해야 하는지 계산
		float invSlope = (dx == 0) ? 0.0f : (float)(y1 - y0) / (float)(x1 - x0);
		float curY = (float)y0;

		for (int x = x0; x <= x1; x++)
		{
			SetPixel(g_hRT, x, (int)std::floor(curY), RGB(0, 255, 0));
			curY += invSlope;
		}
	}
	else
	{
		// 수직에 가까운 선은 y를 주축으로 => x가 1증가할때마다 세로로 2픽셀 이상 찍어야 할때

		// 작은 Y 좌표를 y0로 오게 정렬
		if (y0 > y1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		// y가 하나 증가할때마다 x는 얼만큼 증가해야 하는지 계산
		float invSlope = (dy == 0) ? 0.0f : (float)(x1 - x0) / (float)(y1 - y0);
		float curX = (float)x0;

		for (int y = y0; y <= y1; y++)
		{
			SetPixel(g_hRT, (int)std::floor(curX), y, RGB(0, 255, 0));
			curX += invSlope;
		}
	}
}

void LineDashDraw(POINT sp, POINT ep)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenDash);
	MoveToEx(g_hRT, sp.x, sp.y, NULL);
	LineTo(g_hRT, ep.x, ep.y);
	SelectObject(g_hRT, hOldPen);
}

void LineClear()
{
	// 점 데이터 초기화
	g_p1.x = 0; g_p1.y = 0;
	g_p2.x = 0; g_p2.y = 0;
	g_p3.x = 0; g_p3.y = 0;
	g_currentPoint.x = 0; g_currentPoint.y = 0;

	// 몇번째 점까지 입력 되었는지 여부 초기화
	g_PtCheck = PT_NONE_;
}

// 십자선 점 그리기
void CrossDraw(POINT pt)
{
	MoveToEx(g_hRT, pt.x - 5, pt.y, NULL);
	LineTo(g_hRT, pt.x + 5, pt.y);

	MoveToEx(g_hRT, pt.x, pt.y - 5, NULL);
	LineTo(g_hRT, pt.x, pt.y + 5);
}

// 씬 렌더링
void BeginScene()
{
	SetBkMode(g_hRT, TRANSPARENT);
}

// 씬 렌더링 종료
void EndScene()
{
	//ReleaseDC(g_hWnd, g_hDC);
}

// 렌더타겟 비우기
void Clear(COLORREF col)
{
	HBRUSH hBrush = CreateSolidBrush(col);
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	FillRect(g_hRT, &rc, hBrush);
	DeleteObject(hBrush);
}

// 씬 출력
void Present()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	HDC hdc = GetDC(g_hWnd);		// 프론트버퍼 가져옴
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_hRT, 0, 0, SRCCOPY);	// 백버퍼를 프론트버퍼에 복사
	ReleaseDC(g_hWnd, hdc);
}

// 백버퍼용 렌더타겟을 생성
int RenderTargetCreate(HWND hwnd)
{
	RECT rc;  GetClientRect(hwnd, &rc);
	HDC hdc = GetDC(hwnd);
	g_hRT = CreateCompatibleDC(hdc);
	g_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	SelectObject(g_hRT, g_hBmpRT);
	ReleaseDC(hwnd, hdc);

	return TRUE;
}

// 렌더타겟 제거
void RenderTargetRelease()
{
	DeleteObject(g_hBmpRT);
	DeleteDC(g_hRT);
}

void PutFPS(int x, int y)
{
	static UINT  frm = 0;
	static float fps = 0.0f;
	++frm;
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG nowtime = GetTickCount64();

	UINT time = (UINT)(nowtime - oldtime);
	if (time >= 1000)
	{
		fps = (float)(frm * 1000) / (float)time;
		frm = 0;
		oldtime = nowtime;
	}

	DrawText(x, y, RGB(255, 255, 255), _T("FPS=%.1f/%d      "), fps, time);
}

void DrawText(int x, int y, COLORREF col, const TCHAR* msg, ...)
{
	TCHAR buff[128] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);

	//HDC hdc = GetDC(g_hWnd);
	//col = GetSysColor(COLOR_BACKGROUND+1);
	//SetBkColor(hdc, RGB(128, 128, 128));
	//SetBkMode(hdc, TRANSPARENT);
	//SetBkColor(g_hDC, col);
	SetTextColor(g_hRT, col);
	TextOut(g_hRT, x, y, buff, (int)_tcslen(buff));
	//ReleaseDC(g_hWnd, hdc);
}
void ShowInfo()
{
	static bool bShow = true;
	if (IsKeyUp(VK_F1)) bShow ^= true;

	PutFPS(1, 1);

	if (!bShow) return;

	// Today's Topic.	 
	int x = 300, y = 1;
	COLORREF col = RGB(255, 255, 255);
	COLORREF col2 = RGB(255, 255, 0);
	DrawText(x, y, col, _T("■ %s"), g_WindowName);
	DrawText(x, y += 16, col, _T("과제 1 : 삼각형 만들기"));

	x = 1, y = 1;
	DrawText(x, y += 16, col, _T("도움말: F1"));

	y += 16;
	DrawText(x, y += 16, col, _T("[Line]"));
	DrawText(x, y += 16, col, _T("좌표입력 : L-Button"));
	DrawText(x, y += 16, col, _T("현재 포인터의 좌표 = {%d, %d}"), g_currentPoint.x, g_currentPoint.y);
	DrawText(x, y += 16, col, _T("Point 1 = {%d, %d}"), g_p1.x, g_p1.y);
	DrawText(x, y += 16, col, _T("Point 2 = {%d, %d}"), g_p2.x, g_p2.y);
	DrawText(x, y += 16, col, _T("Point 3 = {%d, %d}"), g_p3.x, g_p3.y);
}

void SceneRender()
{
	// TODO : 렌더링 상태, 조명, 카메라 조절
	// TODO : 주인공, 몬스터, 오브젝트, 충돌
	// TODO : 씬 렌더링 - 주인공, 몬스터, 배경 등


	BeginScene();
		Clear(RGB(128, 128, 128));

		LineDraw();

		ShowInfo();
	EndScene();

	Present();
}