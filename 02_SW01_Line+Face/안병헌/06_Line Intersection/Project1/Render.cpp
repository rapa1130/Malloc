#pragma warning(disable:4996)
#include"Render.h"
#include"tchar.h"
#include <atomic>
#include<iostream>
#include<algorithm>
extern HWND g_Hwnd;
extern TCHAR* g_WindowName;

enum
{
	PT_NONE = 0x00,
	PT_V1 = 0x01,
	PT_V2 = 0x02,
	PT_V3 = 0x04,

	PT_1 = (PT_V1),
	PT_2 = (PT_V1 | PT_V2),
	PT_3 = (PT_V1 | PT_V2 | PT_V3),

	PT_COMPLETE = (PT_V1 | PT_V2 | PT_V3)
};

struct Line
{
	int a;
	int b;
	int c;
};

//POINT g_SP, g_EP;
DWORD g_PtState = PT_NONE;
HDC g_BackHdc;
HBITMAP g_BackBitmap;

HPEN g_GreenPen;
HPEN g_DashedPen;
HPEN g_BlackPen;
HPEN g_WhitePen;
HPEN g_BlueDashedPen;
HPEN g_RedDashedPen;

const POINT g_ClearVtx = { 0,0 };
POINT g_Vtx[2] = {};
POINT g_TempVtx = {};

POINT g_IntersectionVtx = {};
POINT g_FixedLineSP = {100,300};
POINT g_FixedLineEP = {860,300};

Line g_UserLine = {};
Line g_FixedLine = {};

const Line g_ClearLine = { 0,0,0 };

static bool g_bShowVtxInfo = true;
static bool g_bIntersect = false;

void DrawText(int x, int y, COLORREF color, TCHAR* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TCHAR buff[128] = _T("");
	_vstprintf(buff, fmt, args);
	va_end(args);

	SetTextColor(g_BackHdc, color);
	TextOut(g_BackHdc, x, y, buff, _tcslen(buff));
}

void putFPS(int x,int y)
{
	static int frame = 0;
	static float fps = 0.0f;
	frame++;

	static ULONGLONG oldTime = GetTickCount64();
	ULONGLONG		 nowTime = GetTickCount64();

	ULONGLONG time = nowTime - oldTime;
	if (time >= 1000)
	{
		fps = (float)(frame * 1000)/ (float)time;
		oldTime = nowTime;
		frame = 0;
	}
	DrawText(x, y, RGB(255, 255, 255), _T("FPS=%.1f/%d"), fps, frame);
}

void ShowInfo()
{
	COLORREF color = RGB(255, 255, 255);
	int x = 10;
	int y = 10;

	putFPS(x, y);

	static bool bShow = true;
	if (IfKeyUp(VK_F1)) bShow = !bShow;


	if (!bShow) return;

	DrawText(x, y+=17, color, _T("도움말: F1"));
	DrawText(x, y+=17, color, _T("정점정보: F2 [%s]"),(g_bShowVtxInfo?_T("ON") : _T("OFF")));

	x = 10;
	y = 70;
	DrawText(x, y, color, _T("[Line]"));
	DrawText(x, y+=17, color, _T("좌표입력 : L-Button"));
	DrawText(x, y+=17, color, _T("정점 리셋 : R-Button"));
	for (int i = 0; i < 2; i++)
	{
		DrawText(x, y += 17, color, _T("vtx[%d] = {%d, %d}"),i, g_Vtx[i].x, g_Vtx[i].y);
	}

	if (g_PtState == PT_COMPLETE && !g_bIntersect) color = RGB(180, 0, 0);
	else color = RGB(100, 255, 255);
	DrawText(x, y += 17, color, _T("P = {%d, %d}"), g_IntersectionVtx.x, g_IntersectionVtx.y);
	color = RGB(255, 255, 255);

	x = 200;
	y = 10;

	DrawText(x, y, color, _T("%s:SW-Renderer T00 : Line 06 Intersection"),g_WindowName);
	
	color = RGB(180, 180, 0);
	DrawText(x, y += 17, color, _T("1. 점 2개를 이용한 직선 그리기"));
	
	color = RGB(255, 255, 0);
	DrawText(x, y += 17, color, _T("6. 직선의 교차/교점 구하기"));
	DrawText(x, y += 17, color, _T(" + 직선-직선 교차 [일반형]: ax+by+c=0"));

	x = 750;
	y = 70;

	if (g_PtState == PT_COMPLETE && !g_bIntersect) color = RGB(180, 0, 0);
	else color = RGB(100, 255, 255); 

	DrawText(x, y += 17, color, _T("P= {%d,%d}"), g_IntersectionVtx.x, g_IntersectionVtx.y);
	DrawText(x, y += 17, color, _T("L#1 = {%d, %d, %d, %d}"),
		g_Vtx[0].x,
		g_Vtx[0].y,
		g_Vtx[1].x,
		g_Vtx[1].y
		);

	float m = 0.0f;
	if (g_PtState == PT_COMPLETE) m = (float)-g_UserLine.a / g_UserLine.b;
	DrawText(x, y += 17, color, _T("   = m: %.2f"),m);

	y += 20;
	DrawText(x, y += 17, color, _T("L#2 = {%d, %d, %d, %d}"),
		g_FixedLineSP.x,
		g_FixedLineSP.y,
		g_FixedLineEP.x,
		g_FixedLineEP.y
	);

	m = (float)-g_FixedLine.a / g_FixedLine.b;
	DrawText(x, y += 17, color, _T("   = m: %.2f"), m);

	if (g_PtState == PT_COMPLETE && !g_bIntersect)
	{
		DrawText(450, 330, color, _T("교점 오류!!!"));
	}

}

int LoadData()
{
	g_PtState = PT_NONE;

	g_GreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	g_BlackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	g_DashedPen = CreatePen(PS_DASH, 1, RGB(255, 255, 255));
	g_WhitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	g_BlueDashedPen = CreatePen(PS_DASH, 1, RGB(50, 50, 255));
	g_RedDashedPen = CreatePen(PS_DASH, 1, RGB(255, 0, 0));

	g_FixedLine = GetLineFromTwoPoint(g_FixedLineSP, g_FixedLineEP);
	return true;
}

void ReleaseData()
{
	DeleteObject(g_GreenPen);
	DeleteObject(g_BlackPen);
	DeleteObject(g_DashedPen);
	DeleteObject(g_WhitePen);
	DeleteObject(g_BlueDashedPen);
	DeleteObject(g_RedDashedPen);
}

void BeginScene()
{
	SetBkMode(g_BackHdc, TRANSPARENT);
}
void EndScene()
{
}

void Clear(COLORREF color)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	RECT rc = {};
	GetClientRect(g_Hwnd, &rc);
	FillRect(g_BackHdc, &rc, hBrush);
	DeleteObject(hBrush);
}

void Present()
{
	HDC hdc = GetDC(g_Hwnd);
	RECT rc = { 0, };
	GetClientRect(g_Hwnd, &rc);
	BitBlt(hdc,0, 0, rc.right,rc.bottom, g_BackHdc, 0, 0, SRCCOPY);
	ReleaseDC(g_Hwnd, hdc);
}

void RenderScene()
{
	if (IfKeyUp(VK_F2)) g_bShowVtxInfo ^= true;

	BeginScene();
		Clear(RGB(128, 128, 128));
		DrawLine();
		ShowInfo();
	EndScene();

	Present();
}

bool IsVertical(Line line1, Line line2)
{
	return line1.a * line2.a + line1.b * line2.b == 0;
}

bool IsParallel(Line line1, Line line2)
{
	return line1.a * line2.b - line2.a * line1.b == 0;
}


bool GetIntersectionPoint(Line line1,Line line2,POINT& intersect)
{
	int det = line1.a * line2.b - line2.a * line1.b;
	if (det == 0)
	{
		intersect = g_ClearVtx;
		return false;
	}
	int Xintersect = (line1.b * line2.c - line2.b * line1.c) / det;
	int Yintersect = (line2.a * line1.c -line1.a * line2.c) / det;
	intersect = { Xintersect,Yintersect };
	return true;
}

Line GetLineFromTwoPoint(POINT pt1, POINT pt2)
{
	Line ret = {};
	ret.a = pt1.y - pt2.y;
	ret.b = pt2.x - pt1.x;
	ret.c = -pt1.x * ret.a - pt1.y * ret.b;
	return ret;
}

void UpdateLine(POINT pt)
{
	switch (g_PtState)
	{
	case PT_NONE:
	case PT_COMPLETE:
		g_Vtx[0] = pt;
		g_Vtx[1] = g_ClearVtx;
		g_IntersectionVtx = g_ClearVtx;
		g_UserLine = g_ClearLine;
		g_PtState = PT_1;
		break;
	case PT_1:
		g_Vtx[1] = pt;
		g_UserLine = GetLineFromTwoPoint(g_Vtx[0], g_Vtx[1]);
		g_bIntersect = GetIntersectionPoint(g_UserLine, g_FixedLine, g_IntersectionVtx);
		g_PtState = PT_COMPLETE;
		break;
	}
}

void DrawLine()
{
	DrawLine(g_FixedLineSP, g_FixedLineEP, g_WhitePen);

	switch (g_PtState)
	{
	case PT_NONE:
		break;
	case PT_1:
	{
		DrawCrossLine(g_Vtx[0], _T("v%d"), 0);
		Line tempLine = GetLineFromTwoPoint(g_Vtx[0], g_TempVtx);
		if (IsParallel(tempLine,g_FixedLine) || IsVertical(tempLine,g_FixedLine))  
			DrawLine(g_Vtx[0], g_TempVtx, g_RedDashedPen);
		else DrawLine(g_Vtx[0], g_TempVtx, g_DashedPen);
	}break;
	case PT_COMPLETE:
	{
		DrawExtendedLine(g_Vtx[0], g_Vtx[1]);
		DrawLine(g_Vtx[0], g_Vtx[1]);
		DrawCrossLine(g_Vtx[0], _T("v%d"), 0);
		DrawCrossLine(g_Vtx[1], _T("v%d"), 1);
		DrawCrossLine(g_IntersectionVtx, _T("P"));
	}break;
	}
}



void DrawLine(POINT sp, POINT ep)
{
	MoveToEx(g_BackHdc, sp.x, sp.y, NULL);
	LineTo(g_BackHdc, ep.x, ep.y);
}

void DrawLine(POINT sp, POINT ep,HPEN hPen)
{
	HPEN hOldPen = (HPEN)SelectObject(g_BackHdc, hPen);
	DrawLine(sp, ep);
	SelectObject(g_BackHdc, hOldPen);
}

void DrawCrossLine(POINT pt,TCHAR* fmt, ...)
{
	if (!g_bShowVtxInfo) return;

	HPEN hOldPen =(HPEN) SelectObject(g_BackHdc, g_GreenPen);

	MoveToEx(g_BackHdc, pt.x - 5, pt.y, NULL);
	LineTo(g_BackHdc, pt.x + 5, pt.y);
	MoveToEx(g_BackHdc, pt.x, pt.y + 5, NULL);
	LineTo(g_BackHdc, pt.x, pt.y - 5);

	SelectObject(g_BackHdc, hOldPen);

	TCHAR buff[128] = {0,};
	va_list vl;
	va_start(vl, fmt);
	_vstprintf(buff, fmt, vl);
	va_end(vl);

	DrawText(pt.x, pt.y, RGB(255, 255, 255), buff);
}


void DrawExtendedLine(POINT sp, POINT ep)
{
	DrawExtendedLine(sp, ep, g_BlueDashedPen);
}

void DrawExtendedLine(POINT sp, POINT ep, HPEN hPen)
{
	RECT rc = {};
	GetClientRect(g_Hwnd, &rc);
	Line leftMostLine = GetLineFromTwoPoint({ rc.left,rc.top }, { rc.left,rc.bottom });
	Line rightMostLine = GetLineFromTwoPoint({ rc.right,rc.top }, { rc.right,rc.bottom });

	Line topMostLine = GetLineFromTwoPoint({ rc.left,rc.top }, { rc.right,rc.top });
	Line bottomMostLine = GetLineFromTwoPoint({ rc.left,rc.bottom }, { rc.right,rc.bottom });

	POINT leftIntersect;
	POINT rightIntersect;
	POINT topIntersect;
	POINT bottomIntersect;
	bool bLeft = GetIntersectionPoint(g_UserLine, leftMostLine, OUT leftIntersect);
	bool bRight = GetIntersectionPoint(g_UserLine, rightMostLine, OUT rightIntersect);
	bool bTop = GetIntersectionPoint(g_UserLine, topMostLine, OUT topIntersect);
	bool bBottom = GetIntersectionPoint(g_UserLine, bottomMostLine, OUT bottomIntersect);

	if (!bLeft || !bRight)
	{
		if (sp.y > ep.y) std::swap(sp, ep);
		DrawLine(sp, topIntersect, g_RedDashedPen);
		DrawLine(ep, bottomIntersect, g_RedDashedPen);
		return;
	}
	if (!bTop && !bBottom)
	{
		if (sp.x > ep.x) std::swap(sp, ep);
		DrawLine(sp, leftIntersect, g_RedDashedPen);
		DrawLine(ep, rightIntersect, g_RedDashedPen);
		return;
	}


	int x = 0;
	int y = 500;
	COLORREF color = RGB(255, 100, 100);

	POINT lowerPoint;
	POINT topPoint;

	if (sp.y < ep.y) std::swap(sp, ep);

	if (topIntersect.x < rc.left) lowerPoint = leftIntersect;
	else if (topIntersect.x > rc.right) lowerPoint = rightIntersect;
	else lowerPoint = topIntersect;

	if (bottomIntersect.x < rc.left) topPoint = leftIntersect;
	else if (bottomIntersect.x > rc.right) topPoint = rightIntersect;
	else topPoint = bottomIntersect;

	DrawLine(sp, topPoint, hPen);
	DrawLine(ep, lowerPoint, hPen);
}

void LineClear()
{
	for (int i = 0; i < 2; i++)
	{
		g_Vtx[i] = g_ClearVtx;
	}
	g_PtState = PT_NONE;
	g_TempVtx = g_ClearVtx;
	g_IntersectionVtx = g_ClearVtx;
	g_UserLine = g_ClearLine;
}

void CreateRenderTarget()
{
	HDC hdc = GetDC(g_Hwnd);
	g_BackHdc = CreateCompatibleDC(hdc);
	RECT rc = { 0 };
	GetClientRect(g_Hwnd, &rc);
	g_BackBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	SelectObject(g_BackHdc, g_BackBitmap);
	ReleaseDC(g_Hwnd, hdc);
}

void ReleaseRenderTarget()
{
	DeleteObject(g_BackBitmap);
	DeleteDC(g_BackHdc);
}
