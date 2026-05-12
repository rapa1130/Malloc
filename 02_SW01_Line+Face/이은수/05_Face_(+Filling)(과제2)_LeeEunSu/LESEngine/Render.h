

#pragma once

extern HWND g_hWnd;
extern POINT g_p1, g_p2, g_p3, g_currentPoint;

// 비동기 키보드 입력 처리
#define IsKeyDown(k) ((GetAsyncKeyState(k) & 0x8000) == 0x8000)
#define IsKeyUp(k) ((GetAsyncKeyState(k) & 0x8001) == 0x8001)

#ifndef SafeRelease
#define SafeRelease(p) if((p))	{ (p)->Release(); (p) = NULL; }
#define SafeDelete(p) if((p))	{ delete (p);	  (p) = NULL; }
#define SafeDelArry(p) if((p))	{ delete[] (p);	  (p) = NULL; }
#endif // !SafeRelease


int DataLoading();
void DataRelease();
void SceneRender();

// GDI 문자열 출력
//void DrawText(int x, int y, const TCHAR* msg, ...);
void DrawText(int x, int y, COLORREF col, const TCHAR* msg, ...);

void LineUpdate(POINT pt);
void LineDraw();
void LineDraw(POINT sp, POINT ep);
void LineDashDraw(POINT sp, POINT ep);
void TriangleDraw(POINT p1, POINT p2, POINT p3);
void LineClear();
void CrossDraw(POINT pt);

int RenderTargetCreate(HWND hwnd);
void RenderTargetRelease();