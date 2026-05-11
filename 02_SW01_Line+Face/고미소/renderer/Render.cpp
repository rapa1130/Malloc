#pragma warning(disable:4267 4996)

#include "stdio.h"
#include "Render.h"

#include <algorithm>

const TCHAR* g_WindowName = _T("SWR KohMS::Line04");

extern HWND g_hWnd;
enum
{
	// 점 설정 관련
	PT_NONE_ = 0x00,
	PT_V0_ = 0x01,
	PT_V1_ = 0x02,
	PT_V2_ = 0x04,

	// 삼각형 점 개수
	PT_MAX_ = 3,
	PT_1_ = PT_V0_, // 1개 입력 완료					
	PT_2_ = (PT_V0_ | PT_V1_),	// 2개 입력 완료			
	PT_3_ = (PT_V0_ | PT_V1_ | PT_V2_),	// 3개 입력 완료
	PT_COMPLETED_ = (PT_V0_ | PT_V1_ | PT_V2_)
};

// 입력된 삼각형 정점
POINT g_Vtx[PT_MAX_];
POINT g_VtxClear = { 0, 0 };
POINT g_VtxTemp = { 0, 0 };
// 삼각형 구성 정점
DWORD g_PtCheck = PT_NONE_;

// 렌더 타켓 구성용 핸들
HBITMAP g_hBmpRT = NULL;
HDC g_hSurfaceRT = NULL;
COLORREF g_BkColor = RGB(0, 0, 255);
#define g_hRT g_hSurfaceRT

// 라인 그리기 펜
HPEN g_hPenDash;
HPEN g_hPenWhite;
HPEN g_hPenGreen;

// 정점 정보 보기
BOOL g_bShowVtxInfo = TRUE;


// 게임 데이터 및 렌더링 자원 로드
int DataLoading()
{
	LineClear();

	// 라인용 펜 생성
	g_hPenDash = CreatePen(PS_DASH, 1, RGB(255, 255, 255));
	g_hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	g_hPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

	return TRUE;
}

// 게임 데이터 및 렌더링 자원 해제
void DataRelease()
{
	// 라인용 펜 제거
	DeleteObject(g_hPenDash);
	DeleteObject(g_hPenWhite);
	DeleteObject(g_hPenGreen);
}

int RenderTargetCreate(HWND hwnd)
{
	RECT rc; GetClientRect(hwnd, &rc);

	HDC hdc = GetDC(hwnd);
	g_hRT = CreateCompatibleDC(hdc);									
	
	g_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, rc.right, rc.bottom);	
	
	SelectObject(g_hRT, g_hBmpRT);
	ReleaseDC(hwnd, hdc);

	return TRUE;
}

void RenderTargetRelease()
{
	DeleteObject(g_hBmpRT);
	DeleteDC(g_hRT);
}

void BeginScene()
{
	SetBkMode(g_hRT, TRANSPARENT);
}

void Clear(COLORREF col)
{
	HBRUSH hBrush = CreateSolidBrush(col);
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	FillRect(g_hRT, &rc, hBrush);
	DeleteObject(hBrush);
}

void MoveTo(HDC hdc, int x, int y, LPPOINT lppt)
{
	if (hdc == NULL) { return; }
	
	if (lppt) 
	{ 
		lppt->x = g_VtxTemp.x; 
		lppt->y = g_VtxTemp.y; 
	}

	g_VtxTemp.x = x;
	g_VtxTemp.y = y;
}

void SetLine(HDC hdc, int x, int y)
{
	if (hdc == NULL) { return; }

	// 같은 점일 경우
	if (x == g_VtxTemp.x && y == g_VtxTemp.y) { return; }

	const float dx = static_cast<float>(x - g_VtxTemp.x);
	const float dy = static_cast<float>(y - g_VtxTemp.y);

	// 1. 수직선 예외 처리
	if (dx == 0)
	{
		int sy; int ty;
		if (y < g_VtxTemp.y) { sy = y; ty = g_VtxTemp.y; }
		else { sy = g_VtxTemp.y; ty = y; }

		for (int curY = ty; curY >= sy; --curY)
		{
			SetPixel(hdc, x, curY, RGB(255, 0, 0));
		}

		g_VtxTemp.x = x;
		g_VtxTemp.y = y;
		return;
	}

	const float gradient = dy / dx;

	// 2. 수평선에 가까울 경우 // 브레젠헴 알고리즘
	if (abs(gradient) <= 1)
	{
		POINT s, t; // x축이 주축
		if (x < g_VtxTemp.x) { s = { x, y }; t = { g_VtxTemp.x, g_VtxTemp.y }; }
		else { s = { g_VtxTemp.x, g_VtxTemp.y }; t = { x, y }; }

		float gradient = static_cast<float>(s.y - t.y) / (s.x - t.x);

		float curY = static_cast<float>(s.y);
		for (int curX = s.x; curX <= t.x; ++curX)
		{
			SetPixel(hdc, curX, roundf(curY), RGB(255, 0, 0));
			curY += gradient;
		}
	}
	// 3. 수직선에 가까울 경우
	else
	{
		POINT s, t; // y축이 주축
		if (y > g_VtxTemp.y) { s = { x, y }; t = { g_VtxTemp.x, g_VtxTemp.y }; }
		else { s = { g_VtxTemp.x, g_VtxTemp.y }; t = { x, y }; }

		float gradientR = static_cast<float>(s.x - t.x) / (s.y - t.y); 

		float curX = static_cast<float>(s.x);
		for (int curY = s.y; curY >= t.y; --curY) // 수평라인 방식
		{
			SetPixel(hdc, roundf(curX), curY, RGB(255, 0, 0));
			curX -= gradientR; 
		}
	}

	g_VtxTemp.x = x;
	g_VtxTemp.y = y;
}

int Lerp(int a, int b, float t) {
	// 시작 값 a에 a 와 b 사이 값을 더함.
	return a + static_cast<int>((b - a)/*길이*/ * t);
}

void LineFilling(HDC hdc, POINT g_Vtx[3], COLORREF col)
{
	if (g_PtCheck != PT_COMPLETED_) { return; }

	// 정점[0], 정점[1], 정점[2] 정렬 (윈도우 좌표계이므로, y축이 증가할 수록 하단)
	if (g_Vtx[0].y > g_Vtx[1].y) { std::swap(g_Vtx[0], g_Vtx[1]); }
	if (g_Vtx[0].y > g_Vtx[2].y) { std::swap(g_Vtx[0], g_Vtx[2]); }
	if (g_Vtx[1].y > g_Vtx[2].y) { std::swap(g_Vtx[1], g_Vtx[2]); }
	
	const int totalHeight = g_Vtx[2].y - g_Vtx[0].y + 1;
	const int heightA = g_Vtx[2].y - g_Vtx[1].y + 1;
	const int heightB = g_Vtx[1].y - g_Vtx[0].y + 1;

	// 1. 삼각형 분할
	for (int y = g_Vtx[1].y; y >= g_Vtx[0].y; --y)
	{
		float alpha = (float)(y - g_Vtx[0].y) / totalHeight;
		float beta = (float)(y - g_Vtx[0].y) / heightB;

		// 2. 시작점과 끝점 채우기
		int left = Lerp(g_Vtx[0].x, g_Vtx[2].x, alpha);
		int right = Lerp(g_Vtx[0].x, g_Vtx[1].x, beta);

		if (left > right) { std::swap(left, right); }

		for (int j = left; j <= right; ++j) // 
		{
			SetPixel(hdc, j, y, col);
		}
	}
	
	// 1. 삼각형 분할 
	for (int y = g_Vtx[2].y; y >= g_Vtx[1].y; --y) // 수평라인 방식 (y축)
	{
		const float alpha = (float)(y - g_Vtx[0].y) / totalHeight; 
		const float beta = (float)(y - g_Vtx[1].y) / heightA; 

		// 2. 시작점과 끝점 채우기 (x축)
		int left = Lerp(g_Vtx[0].x, g_Vtx[2].x, alpha);
		int right = Lerp(g_Vtx[1].x, g_Vtx[2].x, beta);

		if (left > right) { std::swap(left, right); }

		for (int j = left; j <= right; ++j)
		{
			SetPixel(hdc, j, y, col);
		}
	}
}

void LineClear()
{
	// 입력 정점 초기화
	for (int i = 0; i < PT_MAX_; ++i)
	{
		g_Vtx[i] = g_VtxClear;
	}
	g_VtxTemp = g_VtxClear;

	// 삼각형 구성 정점 초기화 
	g_PtCheck = PT_NONE_;
}

void LineUpdate(POINT pt)
{
	switch (g_PtCheck)
	{
		// 1번째
	case PT_NONE_:
	case PT_COMPLETED_:
		g_Vtx[0] = pt;
		g_Vtx[1] = g_VtxClear;
		g_Vtx[2] = g_VtxClear;
		g_PtCheck = PT_1_;
		break;
		// 2번째
	case PT_1_:
		g_Vtx[1] = pt;
		g_PtCheck = PT_2_;
		break;
		// 3번째
	case PT_2_:
		g_Vtx[2] = pt;
		g_PtCheck = PT_COMPLETED_;
		break;
	}
}

void LineDraw()
{
	switch (g_PtCheck)
	{
		// 그리기 없음
	case PT_NONE_:
		break;
		// 모두 그리기 완료
	case PT_COMPLETED_:
		if (g_bShowVtxInfo) // 정점 정보 출력
		{
			for (int i = 0; i < PT_MAX_; ++i)
			{
				CrossDraw(g_Vtx[i], _T("v%d"), i);
			}
		}
		FaceDraw(); // 삼각형 그리기
		break;
		// 첫번째 표시 및 그리기
	case PT_1_:
		CrossDraw(g_Vtx[0], _T("v0"));
		LineDashDraw(g_Vtx[0], g_VtxTemp);
		break;
		// 두번째 표시 및 그리기
	case PT_2_:
		CrossDraw(g_Vtx[0], _T("v0"));
		CrossDraw(g_Vtx[1], _T("v1"));
		LineDashDraw(g_Vtx[0], g_VtxTemp);
		LineDashDraw(g_Vtx[1], g_VtxTemp);

		LineDraw(g_Vtx[0], g_Vtx[1]);
		break;
	}
}

void LineDraw(POINT sp, POINT ep)
{
	MoveTo(g_hRT, sp.x, sp.y, NULL);
	SetLine(g_hRT, ep.x, ep.y);
}

/////////////
void LineDashDraw(POINT sp, POINT ep)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenDash);
	MoveTo(g_hRT, sp.x, sp.y, NULL);
	SetLine(g_hRT, ep.x, ep.y);
	SelectObject(g_hRT, hOldPen);
}

void FaceDraw()
{
	LineDraw(g_Vtx[0], g_Vtx[1]); // v0 -> v1
	LineDraw(g_Vtx[0], g_Vtx[2]); // v0 -> v2
	LineDraw(g_Vtx[1], g_Vtx[2]); // v1 -> v2
}

void CrossDraw(POINT pt, const TCHAR* name, ...)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenDash);

	MoveTo(g_hRT, pt.x - 5, pt.y, NULL);
	SetLine(g_hRT, pt.x + 5, pt.y);

	MoveTo(g_hRT, pt.x, pt.y - 5, NULL);
	SetLine(g_hRT, pt.x, pt.y + 5);

	va_list vl;
	TCHAR buff[80] = _T("");
	va_start(vl, name);
	_vstprintf(buff, name, vl);
	TextOut(g_hRT, pt.x, pt.y, buff, _tcslen(buff));

	SelectObject(g_hRT, hOldPen);
}

void DrawText(int x, int y, COLORREF col, const TCHAR* msg, ...)
{
	TCHAR buff[128] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);

	RECT rc = { x, y, x + 800, y + 600 };

	SetTextColor(g_hRT, col);
	::DrawText(g_hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(g_hRT, RGB(255, 255, 255));
}

void PutFPS(int x, int y)
{
	static UINT frm = 0;
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

void ShowInfo()
{
	static bool bShow = true;
	if (IsKeyUp(VK_F1)) { bShow ^= true; }

	PutFPS(1, 1);
	if (!bShow) { return; }

	COLORREF col = RGB(255, 255, 255);
	COLORREF col2 = RGB(255, 255, 0);
	COLORREF col3 = RGB(180, 180, 0);

	int x = 300, y = 1;
	DrawText(x, y, col, _T("■ %s"), g_WindowName);
	DrawText(x, y += 16, col3, _T("1. 점 2개를 이용한 직선 그리기"));
	DrawText(x, y += 16, col3, _T("2. 더블버퍼링 : \"BackBuffer\" 추가."));
	DrawText(x, y += 16, col3, _T("   + 속도/깜박거림 비교."));
	DrawText(x, y += 16, col2, _T("3. 점 3개를 이용한 삼각형 그리기"));

	x = 1, y = 1;
	DrawText(x, y += 16, col, _T("도움말: F1"));
	DrawText(x, y += 16, col, _T("정점정보: F2 (%s)"), g_bShowVtxInfo ? _T("ON") : _T("OFF"));

	y += 16;
	DrawText(x, y += 16, col, _T("[Line]"));
	DrawText(x, y += 16, col, _T("정점 입력 : L-Button"));
	DrawText(x, y += 16, col, _T("정점 리셋 : R-Button"));

	for (int i = 0; i < PT_MAX_; ++i)
	{
		DrawText(x, y += 16, col, _T("v%d = {%d, %d}"), i, g_Vtx[i].x, g_Vtx[i].y);
	}
}

void EndScene()
{
}

void Present()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	HDC hdc = GetDC(g_hWnd);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_hRT, 0, 0, SRCCOPY);
	ReleaseDC(g_hWnd, hdc);
}

// 게임 장면 렌더링
void SceneRender() 
{
	// 정점 정보 보기
	if (IsKeyUp(VK_F2)) { g_bShowVtxInfo ^= TRUE; }

	//
	BeginScene();

	Clear(RGB(128, 128, 128));
	LineDraw();
	LineFilling(g_hRT, g_Vtx, RGB(255, 0, 0));

	ShowInfo();
	
	EndScene();

	// 장면 출력
	Present();
}
