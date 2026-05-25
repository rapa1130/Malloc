#include"Device.h"
#include"bhDefine.h"
#include<tchar.h>
#include"BH.h"

DisplayMode g_mode = { 800,600 };

LPBH g_pBH = NULL;
LPBHDEVICE9 g_pBHDevice = NULL;

#define hRT g_pBHDevice->GetRT() 

HFONT g_hSysFont = NULL;
COLORREF g_BkColor = RGB(0, 0, 255);
COLORREF g_sysFnColor = RGB(0, 255, 0);



void PutFPS(int x, int y)
{
	static UINT frm = 0;
	static float fps = 0.0f;
	frm++;

	static ULONGLONG oldTime = GetTickCount64();
	ULONGLONG nowTime = GetTickCount64();
	UINT time = (UINT)(nowTime - oldTime);
	
	if (time > 1000)
	{
		fps = ((float)frm * 1000) / (float)time;
		frm = 0;
		oldTime = nowTime;
	}
	DrawText(x, y, g_sysFnColor, _T("fps:%.1f/%d"), fps, frm);
}

int ByeongHeonSetUp(HWND hWnd)
{
	BHPRESENT_PARAMETERS bhParams = {};
	bhParams.bufferCount = 1;
	bhParams.width = g_mode.width;
	bhParams.height = g_mode.height;
	bhParams.windowed = TRUE;

	g_pBH = BHCreate9(BH_VERSION);
	if (BH_INVALIED(g_pBH)) return BH_FAIL;


	g_pBH->CreateDevice(
		hWnd,
		&bhParams,
		YENACREATE_SOFTWARE_VERTEXPROCESSING,
		&g_pBHDevice
	);
	if (BH_INVALIED(g_pBHDevice)) return BH_FAIL;


	g_hSysFont = CreateFont(
		12, 6,
		0, 0, 1, 0, 0, 0,
		DEFAULT_CHARSET,	
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE,
		_T("±¼¸²")
	);

	if (g_hSysFont == NULL)
	{
		return BH_FAIL;
	}

	SelectObject(hRT, g_hSysFont);
	return BH_OK;
}

void ByeongHeonRelease()
{
	DeleteObject(g_hSysFont);

	SafeRelease(g_pBH);
	SafeRelease(g_pBHDevice);
}

void DrawText(int x, int y, COLORREF col, TCHAR* fmt, ...)
{
	TCHAR buff[2048] = _T("");
	va_list vl;
	va_start(vl, fmt);
	_vstprintf(buff, fmt, vl);
	va_end(vl);

	RECT rc = { x,y,(LONG)(x + g_mode.width),(LONG)(y + g_mode.height)};
	SetTextColor(hRT, col);
	DrawText(hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(hRT, g_sysFnColor);
}
