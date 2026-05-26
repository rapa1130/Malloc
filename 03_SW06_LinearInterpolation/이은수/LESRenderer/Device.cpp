#include "windows.h"
#include "tchar.h"
#include "mmsystem.h"
#include "stdio.h"
#include "Device.h"




// 디바이스 관련 전역 데이터 
LPYENA			g_pYena = NULL;			
LPYENADEVICE9	g_pDevice = NULL;		

//백버퍼
#define  hRT   g_pDevice->GetRT()

YENADISPLAYMODE g_Mode = { 800, 600 };		//4:3 

HFONT		g_hSysFont	 = NULL;
COLORREF	g_SysFnColor = RGB(0, 255, 0);


BOOL g_ShowFrame = FALSE;



            
int YenaSetup(HWND hwnd)
{

	// 예나 생성.(IDirect3D 대응)
	g_pYena = YenaCreate9(YENA_VERSION);
	if(YN_INVALIED(g_pYena))
	{
		//Error...
		return YN_FAIL; 
	}

 
	// Yena SWR 장치 정보 구성 
	YENAPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.Width  = g_Mode.Width;		//해상도 결정.(백버퍼 크기)
	pp.Height = g_Mode.Height;
	pp.BackBuffercnt = 1;			//백버퍼 개수. (현재는 1개만 가능)
	pp.Windowed = TRUE;				//풀스크린, 창모드 선택.(현재는 창모드만 가능)
	
	// Yena SWR 장치 생성 : ID3DDevice9 대응	
	g_pYena->CreateDevice(g_hWnd,								//디바이스의 렌더링 목표 윈도우 핸들
						&pp,									//디바이스 화면 구성 정보
						YENACREATE_SOFTWARE_VERTEXPROCESSING,   //정점연산 방법 결정 (현재는 SW 만 가능)
						&g_pDevice								//성공시 리턴받을 디바이스 개체 포인터
						);							
	if(YN_INVALIED(g_pDevice))
	{
		return YN_FAIL;
	}

	//폰트 생성
	g_hSysFont = CreateFont(
		12, 6,
		0, 0, 1, 0, 0, 0,   
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,		 
		CLIP_DEFAULT_PRECIS,  
		DEFAULT_QUALITY,
		FF_DONTCARE, 
		_T("굴림") 
		);
	if(g_hSysFont==NULL)
	{
		return YN_FAIL;	 
	}
 	SelectObject(hRT, g_hSysFont);


	return YN_OK;
}




void YenaRelease()
{ 	
	// 폰트 제거.
	DeleteObject(g_hSysFont);


	// 렌더링 객체 제거. 
	SafeRelease(g_pDevice);		//디바이스 제거 
	SafeRelease(g_pYena);		//예나 제거 
} 




void PutFPS(int x, int y)
{
	static UINT  frm = 0;
	static float fps = 0.0f;
	++frm;
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG		 nowtime = GetTickCount64();

	UINT time = (UINT)(nowtime - oldtime);
	if (time >= 1000)
	{
		fps = (float)(frm * 1000) / (float)time;
		frm = 0;
		oldtime = nowtime;
	}

	DrawText(x, y, RGB(255, 255, 255), _T("FPS=%.1f/%d"), fps, time);
}




float GetEngineTime()
{	 
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG nowtime = GetTickCount64();
	float dTime = (nowtime - oldtime) * 0.001f;
	oldtime = nowtime;

 	return dTime; 
}




void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...)
{
	TCHAR buff[2048]=_T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);
	RECT rc = { x, y, (LONG)(x + g_Mode.Width), (LONG)(y + g_Mode.Height) };

 	SetTextColor(hRT, col);
	DrawText(hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(hRT, g_SysFnColor);
}