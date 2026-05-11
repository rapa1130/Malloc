#pragma warning(disable:4996)
#include"Render.h"
#include"tchar.h"
#include<iostream>

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



//POINT g_SP, g_EP;
DWORD g_PtState = PT_NONE;
HDC g_BackHdc;
HBITMAP g_BackBitmap;

HPEN g_GreenPen;
HPEN g_DashedPen;
HPEN g_BlackPen;

const POINT g_ClearVtx = { 0,0 };
POINT g_Vtx[3] = {};
POINT g_TempVtx = {};
static bool g_bShowVtxInfo = true;

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
	for (int i = 0; i < 3; i++)
	{
		DrawText(x, y += 17, color, _T("vtx[%d] = {%d, %d}"),i, g_Vtx[i].x, g_Vtx[i].y);
	}

	x = 200;
	y = 10;

	DrawText(x, y, color, _T("%s:SW-Renderer T00 : Line 03 [+Face]"),g_WindowName);
	
	color = RGB(180, 180, 0);
	DrawText(x, y += 17, color, _T("1. 점 2개를 이용한 직선 그리기"));
	DrawText(x, y += 17, color, _T("2. 더블버퍼링 : \"BackBuffer\" 추가."));
	DrawText(x, y += 17, color, _T("  + 속도/깜빡거림 비교."));
	DrawText(x, y += 17, color, _T("3. 점 3개를 이용한 삼각형 그리기"));

	color = RGB(255, 255, 0);
	DrawText(x, y += 17, color, _T("4. 삼각형 그리기[사용자 함수 버전]"));
	DrawText(x, y += 17, color, _T("+ GDI와 속도/품질 비교"));

}

int LoadData()
{
	g_PtState = PT_NONE;

	g_GreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	g_BlackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	g_DashedPen = CreatePen(PS_DASH, 1, RGB(255, 255, 255));

	return true;
}

void ReleaseData()
{
	DeleteObject(g_GreenPen);
	DeleteObject(g_BlackPen);
	DeleteObject(g_DashedPen);
}

void BeginScene()
{
	//g_Hdc = GetDC(g_Hwnd);
	SetBkMode(g_BackHdc, TRANSPARENT);
}
void EndScene()
{
	//ReleaseDC(g_Hwnd, g_Hdc);
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



void UpdateLine(POINT pt)
{
	switch (g_PtState)
	{
	case PT_NONE:
	case PT_COMPLETE:
		g_Vtx[0] = pt;
		g_Vtx[1] = g_Vtx[2] = g_ClearVtx;
		g_PtState = PT_1;
		break;
	case PT_1:
		g_Vtx[1] = pt;
		g_PtState = PT_2;
		break;
	case PT_2:
		g_Vtx[2] = pt;
		g_PtState = PT_COMPLETE;
		break;
	
	}
}

void DrawLine()
{
	switch (g_PtState)
	{
	case PT_NONE:
		break;
	case PT_1:
		DrawCrossLine(g_Vtx[0],_T("v%d"), 0);
		DrawDashedLine(g_Vtx[0], g_TempVtx);
		break;
	case PT_2:
		DrawCrossLine(g_Vtx[0], _T("v%d"), 0);
		DrawCrossLine(g_Vtx[1], _T("v%d"), 1);
		DrawLine(g_Vtx[0], g_Vtx[1]);
		DrawDashedLine(g_Vtx[0], g_TempVtx);
		DrawDashedLine(g_Vtx[1], g_TempVtx);
		break;
	case PT_COMPLETE:
		DrawFace();
		break;
	}
}


void DrawLineXIncrement(POINT sp, POINT ep)
{
	HPEN hCurrentPen = (HPEN)GetCurrentObject(g_BackHdc, OBJ_PEN);
	LOGPEN logPen;
	GetObject(hCurrentPen, sizeof(LOGPEN), &logPen);
	
	int nowY = sp.y;
	for (int nowX = sp.x; nowX < ep.x; nowX++)
	{
		SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
		if (2 * (ep.y - sp.y) * (nowX + 1 - sp.x) + (2 * sp.y - 2 * nowY - 1) * (ep.x - sp.x) > 0) nowY++;
	}
}
void DrawLineXDecrement(POINT sp, POINT ep)
{
	HPEN hCurrentPen = (HPEN)GetCurrentObject(g_BackHdc, OBJ_PEN);
	LOGPEN logPen;
	GetObject(hCurrentPen, sizeof(LOGPEN), &logPen);

	int nowY = sp.y;
	for (int nowX = sp.x; nowX < ep.x; nowX++)
	{
		SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
		if (2 * (ep.y - sp.y) * (nowX + 1 - sp.x) + (2 * sp.y - 2 * nowY + 1) * (ep.x - sp.x) < 0) nowY--;
	}
}
void DrawLineYIncrement(POINT sp, POINT ep)
{
	HPEN hCurrentPen = (HPEN)GetCurrentObject(g_BackHdc, OBJ_PEN);
	LOGPEN logPen;
	GetObject(hCurrentPen, sizeof(LOGPEN), &logPen);

	int nowX = sp.x;
	for (int nowY = sp.y; nowY < ep.y; nowY++)
	{
		SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
		if (2 * (ep.x - sp.x) * (nowY + 1 - sp.y) + (2 * sp.x - 2 * nowX - 1) * (ep.y - sp.y) > 0) nowX++;
	}
}
void DrawLineYDecrement(POINT sp, POINT ep)
{
	HPEN hCurrentPen = (HPEN)GetCurrentObject(g_BackHdc, OBJ_PEN);
	LOGPEN logPen;
	GetObject(hCurrentPen, sizeof(LOGPEN), &logPen);

	int nowX = sp.x;
	for (int nowY = sp.y; nowY < ep.y; nowY++)
	{
		SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
		if (2 * (ep.x - sp.x) * (nowY + 1 - sp.y) + (2 * sp.x - 2 * nowX + 1) * (ep.y - sp.y) < 0) nowX--;
	}
}

float Absflt(float f)
{
	return (f > 0) ? f : -f;
}


void DrawLineDDA(POINT sp, POINT ep)
{
	if (sp.x == ep.x)
	{
		DrawVerLine(sp, ep);
		return;
	}
	if (sp.y == ep.y)
	{
		DrawHorLine(sp, ep);
		return;
	}

	HPEN hCurrentPen = (HPEN)GetCurrentObject(g_BackHdc, OBJ_PEN);
	LOGPEN logPen;
	GetObject(hCurrentPen, sizeof(LOGPEN), &logPen);

	int xDelta = ep.x - sp.x;
	int yDelta = ep.y - sp.y;
	int xAbsDelta = std::abs(xDelta);
	int yAbsDelta = std::abs(yDelta);

	if (xAbsDelta > yAbsDelta)
	{
		if (sp.x > ep.x) std::swap(sp, ep);

		float m = (float)yDelta / xDelta;
		float nowY = sp.y;
		for (int nowX = sp.x; nowX < ep.x; nowX++)
		{
			SetPixel(g_BackHdc, nowX, (int)(nowY + 0.5f), logPen.lopnColor);
			nowY += m;
		}
	}
	else
	{
		if (sp.y > ep.y) std::swap(sp, ep);
		float m = (float)xDelta / yDelta;

		float nowX = sp.x;
		for (int nowY = sp.y; nowY < ep.y; nowY++)
		{
			SetPixel(g_BackHdc, (int)(nowX + 0.5f), nowY, logPen.lopnColor);
			nowX += m;
		}
	}
}

void DrawHorLine(POINT sp, POINT ep)
{
	HPEN hCurrentPen = (HPEN)GetCurrentObject(g_BackHdc, OBJ_PEN);
	LOGPEN logPen;
	GetObject(hCurrentPen, sizeof(LOGPEN), &logPen);

	if (sp.x > ep.x) std::swap(sp, ep);
	for (int nowX = sp.x; nowX < ep.x; nowX++)
	{
		SetPixel(g_BackHdc, nowX, sp.y, logPen.lopnColor);
	}
}

void DrawVerLine(POINT sp, POINT ep)
{
	HPEN hCurrentPen = (HPEN)GetCurrentObject(g_BackHdc, OBJ_PEN);
	LOGPEN logPen;
	GetObject(hCurrentPen, sizeof(LOGPEN), &logPen);

	if (sp.y > ep.y) std::swap(sp, ep);
	for (int nowY = sp.y; nowY < ep.y; nowY++)
	{
		SetPixel(g_BackHdc, sp.x, nowY, logPen.lopnColor);
	}
}
void DrawLineBresenhum(POINT sp, POINT ep)
{
	if (sp.x == ep.x)
	{
		DrawVerLine(sp, ep);
		return;
	}
	if (sp.y == ep.y)
	{
		DrawHorLine(sp, ep);
		return;
	}

	HPEN hCurrentPen = (HPEN)GetCurrentObject(g_BackHdc, OBJ_PEN);
	LOGPEN logPen;
	GetObject(hCurrentPen, sizeof(LOGPEN), &logPen);

	int dx = ep.x - sp.x;
	int dy = ep.y - sp.y;
	int dxAbs = std::abs(dx);
	int dyAbs = std::abs(dy);

	if (dx > 0 && dy > 0)
	{
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = sp.y;
			for (int nowX = sp.x; nowX < ep.x; ++nowX)
			{
				SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
				if (f > 0)
				{
					++nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = sp.x;
			for (int nowY = sp.y; nowY < ep.y; ++nowY)
			{
				SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
				if (f > 0)
				{
					++nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else if (dx > 0 && dy < 0)
	{
		dy = std::abs(dy);
		if (dx > dy)
		{



			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = sp.y;
			for (int nowX = sp.x; nowX < ep.x; ++nowX)
			{
				SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
				if (f > 0)
				{
					--nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = sp.x;
			for (int nowY = sp.y; nowY > ep.y; --nowY)
			{
				SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
				if (f > 0)
				{
					++nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else if (dx < 0 && dy > 0)
	{
		dx = std::abs(dx);
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = sp.y;
			for (int nowX = sp.x; nowX > ep.x; --nowX)
			{
				SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
				if (f > 0)
				{
					++nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = sp.x;
			for (int nowY = sp.y; nowY < ep.y; ++nowY)
			{
				SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
				if (f > 0)
				{
					--nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}

	else
	{
		dx = std::abs(dx);
		dy = std::abs(dy);
		if (dx > dy)
		{
			int f = 2 * dy - dx;
			int up = 2 * (dy - dx);
			int same = 2 * dy;

			int nowY = sp.y;
			for (int nowX = sp.x; nowX > ep.x; --nowX)
			{
				SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
				if (f > 0)
				{
					--nowY;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
		else
		{
			int f = 2 * dx - dy;
			int up = 2 * (dx - dy);
			int same = 2 * dx;

			int nowX = sp.x;
			for (int nowY = sp.y; nowY > ep.y; --nowY)
			{
				SetPixel(g_BackHdc, nowX, nowY, logPen.lopnColor);
				if (f > 0)
				{
					--nowX;
					f += up;
				}
				else
				{
					f += same;
				}
			}
		}
	}
}

void DrawLine(POINT sp, POINT ep)
{
	DrawLineBresenhum(sp, ep);
}

void DrawLineLineFunc(POINT sp, POINT ep)
{
	
	int deltaX = ep.x - sp.x;
	int deltaY = ep.y - sp.y;
	float slope = Absflt(deltaY / (float)deltaX);

	if (slope < 1)
	{
		if (deltaX < 0 && deltaY < 0)
		{
			DrawLineXIncrement(ep, sp);
		}
		else if (deltaX < 0 && deltaY >= 0)
		{
			DrawLineXDecrement(ep, sp);
		}
		else if (deltaX >= 0 && deltaY < 0)
		{
			DrawLineXDecrement(sp, ep);
		}
		else // deltaX >=0 0 && deltaY >= 0
		{
			DrawLineXIncrement(sp, ep);
		}
	}
	else // slope >= 1
	{
		if (deltaX < 0 && deltaY < 0)
		{
			DrawLineYIncrement(ep, sp);
		}
		else if (deltaX < 0 && deltaY >= 0)
		{
			DrawLineYDecrement(sp, ep);
		}
		else if (deltaX >= 0 && deltaY < 0)
		{
			DrawLineYDecrement(ep, sp);
		}
		else // deltaX >= 0 && deltaY >= 0
		{
			DrawLineYIncrement(sp, ep);
		}
	}
	
}



void DrawCrossLine(POINT pt,TCHAR* fmt, ...)
{
	if (!g_bShowVtxInfo) return;

	HPEN hOldPen =(HPEN) SelectObject(g_BackHdc, g_GreenPen);

	POINT horSP = pt; horSP.x -= 5;
	POINT horEP = pt; horEP.x += 5;
	POINT verSP = pt; verSP.y -= 5;
	POINT verEP = pt; verEP.y += 5;
	DrawLine(horSP, horEP);
	DrawLine(verSP, verEP);

	SelectObject(g_BackHdc, hOldPen);

	TCHAR buff[128] = {0,};
	va_list vl;
	va_start(vl, fmt);
	_vstprintf(buff, fmt, vl);
	va_end(vl);

	DrawText(pt.x, pt.y, RGB(255, 255, 255), buff);
}

void DrawDashedLine(POINT sp, POINT ep)
{
	HPEN hOldPen = (HPEN) SelectObject(g_BackHdc, g_DashedPen);
	DrawLine(sp, ep);
	SelectObject(g_BackHdc, hOldPen);
}

void DrawFace()
{
	DrawLine(g_Vtx[0], g_Vtx[1]);
	DrawLine(g_Vtx[2], g_Vtx[1]);
	DrawLine(g_Vtx[2], g_Vtx[0]);
	DrawCrossLine(g_Vtx[0], _T("v%d"), 0);
	DrawCrossLine(g_Vtx[1], _T("v%d"), 1);
	DrawCrossLine(g_Vtx[2], _T("v%d"), 2);
}

void LineClear()
{
	for (int i = 0; i < 3; i++)
	{
		g_Vtx[i] = g_ClearVtx;
	}
	g_PtState = PT_NONE;
	g_TempVtx = g_ClearVtx;
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
