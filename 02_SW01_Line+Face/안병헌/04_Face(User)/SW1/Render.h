#pragma once
#include<Windows.h>


extern POINT g_TempVtx;
//extern const POINT g_ClearVtx;
extern POINT g_Vtx[3];
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
void DrawCrossLine(POINT pt, TCHAR* fmt, ...);
void DrawDashedLine(POINT sp, POINT ep);
void DrawFace();
void LineClear();
void DrawVerLine(POINT sp, POINT ep);
void DrawHorLine(POINT sp, POINT ep);

void CreateRenderTarget();
void ReleaseRenderTarget();


