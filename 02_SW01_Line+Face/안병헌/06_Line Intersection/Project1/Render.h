#pragma once
#include<Windows.h>


struct Line;

extern POINT g_TempVtx;
//extern const POINT g_ClearVtx;
extern POINT g_Vtx[2];
//extern DWORD g_PtState;

void DrawText(int x, int y, COLORREF color, TCHAR* fmt, ...);

int LoadData();
void ReleaseData();
void RenderScene();

#define IfKeyDown(k) ((GetAsyncKeyState(k) & 0x8000) == 0x8000)
#define IfKeyUp(k) ((GetAsyncKeyState(k) & 0x8001) == 0x8001)

#ifndef SafeRelease
#define SafeRelease(p) {(p)->Release(); (p) = NULL; }
#define SafeDelete(p) {delete(p); (p) = NULL;}
#define SafeDelArr(p) {delete[] (p); (p) = NULL;}
#endif

void UpdateLine(POINT pt);
void DrawLine();
void DrawLine(POINT sp,POINT ep);
void DrawLine(POINT sp, POINT ep, HPEN hPen);
void DrawCrossLine(POINT pt, TCHAR* fmt, ...);
void DrawExtendedLine(POINT sp, POINT ep);
void DrawExtendedLine(POINT sp, POINT ep, HPEN hPen);
Line GetLineFromTwoPoint(POINT pt1, POINT pt2);
void LineClear();

void CreateRenderTarget();
void ReleaseRenderTarget();


