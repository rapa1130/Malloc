//
// Device.h :  기본 렌더링 및 디바이스 설정 헤더 파일 
//
// B3Yena S/W Renderer v1.x
//  
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2004.05.07. Update
// 2005.09.23. Update (.Net)
//
#pragma once
#pragma warning(disable:4996)


///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 장치 인터페이스/클래스 정의
//
///////////////////////////////////////////////////////////////////////////////
// Project Property > C/C++ > Additional Include Directories 에 등록..  
#include "Yena.h"	// Yena SWR v1.2 
//#include "Yena2.h"	// Yena SW Renderer v2




///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 인터페이스 재정의.											
// 
///////////////////////////////////////////////////////////////////////////////

typedef IYenaDevice9* LPDEVICE;





///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 장치 운용 함수들
//
///////////////////////////////////////////////////////////////////////////////

int		YenaSetup		(HWND hwnd);
void	YenaRelease		();

/* -> YenaDevice 로 이동..
int		RenderTargetCreate	(HWND hwnd);
void	RenderTargetRelease	();

int		BeginScene		();
int		EndScene		();
int		Clear			(COLORREF col);
int		Present			();
*/

void	PutFPS			(int x, int y); 
float	GetEngineTime	();

//void	DrawText	(int x, int y, TCHAR* msg, ...);
void	DrawText	(int x, int y, COLORREF col, TCHAR* msg, ...);


extern BOOL g_ShowFrame;
extern HWND g_hWnd;
extern YENADISPLAYMODE g_Mode;

extern LPYENADEVICE9 g_pDevice;




///////////////////////////////////////////////////////////////////////////////
//
// SWR 시스템/전역/공통 상수 정의
//
///////////////////////////////////////////////////////////////////////////////

#ifndef YN_OK
#define YN_OK	0
#define YN_FAIL	-1
#define YN_FAILED(v)	(V<0)
#endif

//비동기 키보드 입력 처리.
#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

#ifndef SafeRelease
#define SafeRelease(p)  if((p)){ (p)->Release(); (p) = NULL; }
#define SafeDelete(p)   if((p)){ delete (p);	(p) = NULL;	 }
#define SafeDelArry(p)  if((p)){ delete[](p);	(p) = NULL;	 }
#endif






/**************** end of "Device.h" ***********************************/