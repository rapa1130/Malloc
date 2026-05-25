#pragma once
#pragma warning(disable:4996)
#include<Windows.h>
#include"BH.h"


extern LPBHDEVICE9 g_pBHDevice;

extern DisplayMode g_mode;
extern HWND g_hWnd;


void PutFPS(int x, int y);

int ByeongHeonSetUp(HWND hWnd);
void ByeongHeonRelease();
void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...);



#ifndef SafeRelease
#define SafeRelease(p) ((p)->Release(); (p) = NULL;)
#define SafeDelete(p) (delete(p); (p) = NULL;)
#define SafeDelArr(p) (delete[] (p); (p) = NULL;)
#endif


#ifndef IsKeyDown
#define IsKeyDown(k) ((GetAsyncKeyState(k)&0x8000) == 0x8000)
#define IsKeyUp(k)   ((GetAsyncKeyState(k)&0x8001) == 0x8001)
#endif 

#ifndef BH_OK
#define BH_OK 0
#define BH_Fail -1
#define BH_Failed(v) (v<0)
#endif 
