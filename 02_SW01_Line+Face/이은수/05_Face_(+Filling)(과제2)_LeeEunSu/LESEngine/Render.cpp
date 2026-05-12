

#pragma warning(disable:4267 4996)
#include "Windows.h"
#include "stdio.h"
#include "tchar.h"
#include "Render.h"
#include <vector>
#include <iostream>

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

		TriangleDraw(g_p1, g_p2, g_p3);
		break;
	}
}

void LineDraw(POINT sp, POINT ep)
{
	MoveToEx(g_hRT, sp.x, sp.y, NULL);
	LineTo(g_hRT, ep.x, ep.y);
}

void LineDashDraw(POINT sp, POINT ep)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenDash);
	MoveToEx(g_hRT, sp.x, sp.y, NULL);
	LineTo(g_hRT, ep.x, ep.y);
	SelectObject(g_hRT, hOldPen);
}

void SortPoints(std::vector<POINT> &pts)
{
	int n = (int)pts.size();
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1 - i; j++)
		{
			// y좌표 비교 (Win32 좌표계는 아래로 갈수록 y가 큼)
			if (pts[j].y > pts[j + 1].y)
			{
				// POINT 구조체 값 교환
				POINT temp = pts[j];
				pts[j] = pts[j + 1];
				pts[j + 1] = temp;
			}
		}
	}
}

float CalculateInverseSlope(POINT p1, POINT p2)
{
	float dtx = (float)p2.x - p1.x;
	float dty = (float)p2.y - p1.y;

	if (dty == 0) return 0.0f;
	return dtx / dty;          // y가 1 변할 때 x의 변화량
}

void TriangleDraw(POINT p1, POINT p2, POINT p3)
{
	// 위에서 아래로, 왼쪽에서 오른쪽으로 렌더링 할 것이기 때문에
	// 우선, y좌표를 기준으로 점들을 오름차순 정렬
	// 0번 : 화면 기준 최상단
	// 1번 : 중간점
	// 2번 : 화면 기준 최하단
	std::vector<POINT> pts = { p1, p2, p3 };
	SortPoints(pts);

	// 역기울기 계산
	float invSlope1 = CalculateInverseSlope(pts[0], pts[1]); // Top -> Mid
	float invSlope2 = CalculateInverseSlope(pts[0], pts[2]); // Top -> Bottom
	float invSlope3 = CalculateInverseSlope(pts[1], pts[2]); // Mid -> Bottom

	// 가로 보간 시작점 설정
	float curX1 = (float)pts[0].x;
	float curX2 = (float)pts[0].x;

	for (int y = pts[0].y; y <= pts[1].y; y++)
	{
		// 좌우 경계 결정  어느 기울기가 왼쪽인지 판단
		int startX = (int)std::ceil(min(curX1, curX2));
		int endX = (int)std::ceil(max(curX1, curX2));

		for (int x = startX; x <= endX; x++)
		{
			SetPixel(g_hRT, x, y, RGB(0, 255, 0));
		}

		// y가 1 증가할 때마다 x 위치를 역기울기만큼 누적
		curX1 += invSlope1;
		curX2 += invSlope2;
	}

	// float 계산 오차 때문에 삼각형이 틀어짐
	// 삼각형 틀어짐 방지 시작점 재설정
	curX1 = (float)pts[1].x;
	curX2 = (float)pts[0].x + invSlope2 * (float)(pts[1].y - pts[0].y);

	for (int y = pts[1].y; y <= pts[2].y; y++)
	{
		int startX = (int)std::ceil(min(curX1, curX2));
		int endX = (int)std::ceil(max(curX1, curX2));

		for (int x = startX; x <= endX; x++)
		{
			SetPixel(g_hRT, x, y, RGB(0, 255, 0));
		}

		curX1 += invSlope3;
		curX2 += invSlope2;
	}
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
	DrawText(x, y += 16, col, _T("과제 2 : SetPixel로 삼각형 채워보기"));

	x = 1, y = 1;
	DrawText(x, y += 16, col, _T("도움말: F1"));

	y += 16;
	DrawText(x, y += 16, col, _T("[SetPixel Rasterization]"));
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