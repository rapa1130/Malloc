#pragma once


extern HWND g_hWnd;
extern POINT g_Sp, g_Ep;
extern POINT g_Vtx[3];
extern POINT g_VtxTemp;

#define IsKeyDown(k) ((GetAsyncKeyState(k) & 0x8000) == 0x8000)
#define IsKeyUp(k) ((GetAsyncKeyState(k) & 0x8001) == 0x8001)

#ifndef SafeRelease
#define SafeRelease(p) if((p)){ (p)->Release(); (p) = NULL;}
#define SafeDelete(p) if((p)){ delete (p);(p) = NULL;}
#define SafeDelArry(p) if((p)){delete[](p);	(p) = NULL;}
#endif

int DataLoading();
void DataRelease();
void SceneRender();
void LineUpdate(POINT pt);
void LineDraw();
void LineDraw(POINT sp, POINT ep);
void LineDashDraw(POINT sp, POINT ep);
void LineClear();
void FaceDraw();
void CrossDraw(POINT pt, TCHAR * name, ...);


float cross(POINT p1, POINT p2, POINT p3);
void FunctionToFillTriangle(POINT p1, POINT p2, POINT p3);
bool isInTriangle(POINT p1, POINT p2, POINT p3, POINT Target, float t);

void DrawText(int x, int y, COLORREF col, TCHAR * msg, ...);

int  RenderTargetCreate(HWND hwnd);
void RenderTargetRelease();

