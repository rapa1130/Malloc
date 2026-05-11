#pragma once

#include "Windows.h"
#include "tchar.h"

// 
extern HWND g_hWnd;
extern POINT g_Sp, g_Ep;
extern POINT g_Vtx[3];
extern POINT g_VtxTemp;

//비동기 키보드 입력 처리
#define IsKeyDown(k) ((GetAsyncKeyState(k) & 0x8000) == 0x8000)
#define IsKeyUp(k) ((GetAsyncKeyState(k) & 0x8001) == 0x8001)

#ifndef SafeRelease
#define SafeRelease(p) if((p)){ (p)->Release(); (p) = NULL; }
#define SafeDelete(p) if((p)){ delete (p); (p) = NULL; }
#define SafeDelArry(p) if((p)){ delete[](p); (p) = NULL; }
#endif

int DataLoading();
void DataRelease();

int RenderTargetCreate(HWND hwnd);
void RenderTargetRelease();

//void MoveCursor(HDC hdc, int x, int y, LPPOINT lppt);
//void Line(HDC hdc, int x, int y);
void LineFilling(HDC hdc, POINT g_Vtx[3], COLORREF col);

void LineClear();
void LineUpdate(POINT pt);
void LineDashDraw(POINT sp, POINT ep);
void LineDraw();
void LineDraw(POINT sp, POINT ep);
void FaceDraw();
void CrossDraw(POINT pt, const TCHAR* name, ...);

void DrawText(int x, int y, COLORREF col, const TCHAR * msg, ...);

void SceneRender();

