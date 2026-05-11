#pragma warning(disable:4996)
#include "Windows.h"
#include "stdio.h"
#include "tchar.h"
#include "Render.h"

////////////////////////////////////////////////////////////
//
// 데이터 정의 영역
//

TCHAR* g_WindowName = _T("Mia::SW Renderer Tutorial T00 : Line 04");

// 전역 데이터
extern HWND g_hWnd;

enum {
	PT_NONE_ = 0x00,  // 설정된 점 없음
	PT_V0_ = 0x01,    // 첫 번쨰 점 설정 (SP에서 V0로 바뀜)
	PT_V1_ = 0x02,    // 두 번째 점 설정 (EP에서 V1로 바뀜)
	PT_V2_ = 0x04,    // 세 번째 점 설정

	PT_MAX_ = 3,   // 삼각형 점 개수
	PT_1_ = PT_V0_,   // 1개 점 입력 완료
	PT_2_ = (PT_V0_ | PT_V1_ ),   // 2개 점 입력 완료
	PT_3_ = (PT_V0_ | PT_V1_ | PT_V2_),   // 3개 점 입력 완료

	PT_COMPLETED_ = (PT_V0_ | PT_V1_ | PT_V2_)  //모든 점 입력 완료 -> 삼각형 그리기
};

DWORD g_PtCheck = PT_NONE_;  //라인 구성 정점, 설정 완료 여부 저장

//Line_02 렌더타겟 구성용 핸들
HBITMAP g_hBmpRT = NULL;
HDC g_hSurfaceRT = NULL;
COLORREF g_BkColor = RGB(0, 0, 255);
#define g_hRT g_hSurfaceRT

// Line_03 입력 된 삼각형 정점 저장
POINT g_Vtx[PT_MAX_];
POINT g_VtxClear = { 0, 0 };
POINT g_VtxTemp = { 0, 0 };

// Line_03 라인 그리기 펜
HPEN g_hPenDash;
HPEN g_hPenWhite;
HPEN g_hPenGreen;

// Line_03 정점정보 보기
BOOL g_bShowVtxInfo = TRUE;




////////////////////////////////////////////////////////////
//
// 함수 정의 영역
//

//////////////////////////////////////////////
// 
// 렌더링 데이터 불러오기 및 해제
// 

///////////////////////////////////
// 게임 데이터 및 렌더링 자원을 불러옴
// 성공하면 TRUE, 실패하면 FALSE
int DataLoading()
{
	ClearLine();

	//라인용 펜 생성
	g_hPenDash = CreatePen(PS_DASH, 1, RGB(255, 255, 255));
	g_hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	g_hPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

	return TRUE;
}

///////////////////////////////////
// 게임 데이터 및 렌더링 자원을 해제
void DataRelease()
{
	// 라인용 펜 제거
	DeleteObject(g_hPenDash);
	DeleteObject(g_hPenWhite);
	DeleteObject(g_hPenGreen);
}


//////////////////////////////////////////////
// 
// 오브젝트 렌더링
// 
///////////////////////////////////
// 라인 초기화
void ClearLine()
{
	for (int i = 0; i < PT_MAX_; i++)
	{
		g_Vtx[i] = g_VtxClear;  // 정점 초기화
	}
	g_VtxTemp = g_VtxClear;
	g_PtCheck = PT_NONE_;
}

///////////////////////////////////
// 점 업데이트
void UpdateLine(POINT pt)
{
	switch (g_PtCheck)
	{
	case PT_NONE_:
		//초기에 새로 시작하는 경우
	case PT_COMPLETED_:
		//삼각형이 있는 상태에서 또 찍을 경우
		g_Vtx[0] = pt; //점 1개 입력
		g_Vtx[1] = g_VtxClear;
		g_Vtx[2] = g_VtxClear;
		g_PtCheck = PT_1_;
		break;
	case PT_1_:
		g_Vtx[1] = pt;
		g_PtCheck = PT_2_;
		break;
	case PT_2_:
		g_Vtx[2] = pt;
		g_PtCheck = PT_COMPLETED_;
		break;
	}
}

///////////////////////////////////
// 픽셀로 라인 찍기
// Bresenham을 진행하기 전 그리기 시작할 위치, 종료할 위치, 방향 정의
void DrawLinePixel(POINT sp, POINT ep)
{
	// 팔분면 구분을 위해 기울기 계산
	float a = float(ep.y - sp.y) / (ep.x - sp.x);

	// w>h이도록 x나 y방향 기준 정의
	// h가 증가하는 방향이도록 w방향 기준 정의
	if (a > 1)
	{
		if (ep.x - sp.x > 0) //2팔분면
		{
			LONG w = ep.y - sp.y;
			LONG h = ep.x - sp.x;

			Bresenham(h, w, sp.y, sp.x, ep.y, 1, FALSE);
		}
		else  //6팔분면
		{
			LONG w = sp.y - ep.y;
			LONG h = sp.x - ep.x;

			Bresenham(h, w, ep.y, ep.x, sp.y, 1, FALSE);
		}
	}
	else if (a > 0)
	{
		if (ep.x - sp.x > 0)  //1팔분면
		{
			LONG w = ep.x - sp.x;
			LONG h = ep.y - sp.y;

			Bresenham(h, w, sp.x, sp.y, ep.x, 1, TRUE);
		}
		else  //5팔분면
		{
			LONG w = sp.x - ep.x;
			LONG h = sp.y - ep.y;

			Bresenham(h, w, ep.x, ep.y, sp.x, 1, TRUE);
		}
	}
	else if (a > -1)
	{
		if (ep.x - sp.x > 0)  //8팔분면
		{
			LONG w = ep.x - sp.x;
			LONG h = sp.y - ep.y;

			Bresenham(h, w, ep.x, ep.y, sp.x, -1, TRUE);
		}
		else  //4팔분면
		{
			LONG w = sp.x - ep.x;
			LONG h = ep.y - sp.y;

			Bresenham(h, w, sp.x, sp.y, ep.x, -1, TRUE);
		}
	}
	else
	{
		if (ep.x - sp.x > 0)  //7팔분면
		{
			LONG w = sp.y - ep.y;
			LONG h = ep.x - sp.x;

			Bresenham(h, w, sp.y, sp.x, ep.y, -1, FALSE);
		}
		else  //3팔분면
		{
			LONG w = ep.y - sp.y;
			LONG h = sp.x - ep.x;

			Bresenham(h, w, ep.y, ep.x, sp.y, -1, FALSE);
		}
	}
}

///////////////////////////////////
// Bresenham알고리즘 사용 + SetPixel 호출
void Bresenham(LONG h, LONG w, LONG pW, LONG pH, LONG endW, int AddW, BOOL isWX)
{
	LONG d = 2 * h - w;
	const int dashGap = 5;
	int dashPixelCount = -4;

	while (pW * AddW < endW * AddW)
	{
		HPEN currentPen = (HPEN)GetCurrentObject(g_hRT, OBJ_PEN);
		LOGPEN penProperty;
		GetObject(currentPen, sizeof(LOGPEN), &penProperty);
		BOOL penStyle = penProperty.lopnStyle;  //SOLID : 1, DASH : 0
		COLORREF penColor = penProperty.lopnColor;

		if (penStyle == PS_SOLID)
		{
			if (isWX)
			{
				SetPixel(g_hRT, pW, pH, penColor);
			}
			else
			{
				SetPixel(g_hRT, pH, pW, penColor);
			}
		}
		else if (penStyle == PS_DASH)
		{
			if (dashPixelCount > 0)
			{
				if (isWX)
				{
					SetPixel(g_hRT, pW, pH, penColor);
				}
				else
				{
					SetPixel(g_hRT, pH, pW, penColor);
				}
			}

			if (dashPixelCount == 5)
			{
				dashPixelCount -= 10;
			}
			dashPixelCount++;
		}

		if (d > 0)
		{
			d -= 2 * w;
			pH += 1;
		}
		d += 2 * h;
		pW += AddW;
	}
}

///////////////////////////////////
// 라인 그리기
void DrawLines()
{
	switch(g_PtCheck)
	{
	case PT_NONE_:
		break;

	case PT_1_:
		DrawCrossLine(g_Vtx[0], _T("v0")); // 첫 번쨰 점 표시

		DrawDashLine(g_Vtx[0], g_VtxTemp); //임시 라인 표시 (점선)
		break;

	case PT_2_ :
		DrawCrossLine(g_Vtx[0], _T("v0")); // 첫 번쨰 점 표시
		DrawCrossLine(g_Vtx[1], _T("v1")); // 첫 번쨰 점 표시

		DrawDashLine(g_Vtx[0], g_VtxTemp); //임시 라인 표시 (점선)
		DrawDashLine(g_Vtx[01], g_VtxTemp); //임시 라인 표시 (점선)

		DrawSolidLine(g_Vtx[0], g_Vtx[1]);

		break;

	case PT_COMPLETED_:
		if (g_bShowVtxInfo)
		{
			for (int i = 0; i < PT_MAX_; i++)
				DrawCrossLine(g_Vtx[i], _T("v%d"), i);
		}

		DrawFace();
		break;
	}
}

///////////////////////////////////
// 십자 라인 그리기
void DrawCrossLine(POINT pt, TCHAR* name, ...)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenGreen);

	for (int i = pt.x - 5; i < pt.x + 5; i++)
	{
		SetPixel(g_hRT, i, pt.y, RGB(0,255,0));
	}

	for (int i = pt.y - 5; i < pt.y + 5; i++)
	{
		SetPixel(g_hRT, pt.x, i, RGB(0, 255, 0));
	}

	va_list vl;
	TCHAR buff[80] = _T("");
	va_start(vl, name);
	_vstprintf(buff, name, vl);
	TextOut(g_hRT, pt.x, pt.y, buff, _tcslen(buff));

	SelectObject(g_hRT, hOldPen);
}

///////////////////////////////////
// 임시 라인 그리기 (점선)
void DrawDashLine(POINT sp, POINT ep)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenDash);
	DrawLinePixel(sp, ep);
	SelectObject(g_hRT, hOldPen);
}


///////////////////////////////////
// 라인 그리기 (직선)
void DrawSolidLine(POINT sp, POINT ep)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenGreen);
	DrawLinePixel(sp, ep);
	SelectObject(g_hRT, hOldPen);
}

///////////////////////////////////
// 삼각형 그리기
void DrawFace()
{
	//라인 세 개 그리기
	DrawSolidLine(g_Vtx[0], g_Vtx[1]);
	DrawSolidLine(g_Vtx[0], g_Vtx[2]);
	DrawSolidLine(g_Vtx[1], g_Vtx[2]);
}








//////////////////////////////////////////////
// 
// 게임 장면 렌더링
//
///////////////////////////////////
// 백버퍼용 렌더타겟을 생성
int CreateRenderTarget(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	HDC hdc = GetDC(hwnd);
	g_hRT = CreateCompatibleDC(hdc);  //렌더타겟,DC 핸들 생성(Surface)
	g_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, rc.right, rc.bottom);  //렌더타겟,비트멥 생성
	SelectObject(g_hRT, g_hBmpRT);
	ReleaseDC(hwnd, hdc);

	return TRUE;
}


void SceneRender()
{
	// 정점 정보 보기 끄고 키기
	if (IsKeyUp(VK_F2)) g_bShowVtxInfo ^= TRUE;

	BeginScene();
	
	Clear(RGB(128, 128, 128));

	DrawLines();

	ShowInfo();

	EndScene();

	Present();
}

///////////////////////////////////
// 장면 그리기 시작
void BeginScene()
{
	//g_hDC = GetDC(g_hWnd);  // 화면DC 사용안함. 렌더타겟 사용
	SetBkMode(g_hRT, TRANSPARENT);
}

///////////////////////////////////
// 작업영역 "렌더타겟" 클리어
void Clear(COLORREF color)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	FillRect(g_hRT, &rc, hBrush);
	DeleteObject(hBrush);
}

///////////////////////////////////
// 도움말 출력
void ShowInfo()
{
	static bool bShow = true;
	if (IsKeyUp(VK_F1)) bShow ^= true;

	PutFPS(1, 1);

	if (!bShow) return;

	int x = 300, y = 1;
	COLORREF col = RGB(255, 255, 255);
	COLORREF col2 = RGB(255, 255, 0);
	COLORREF col3 = RGB(180, 180, 0);
	DrawText(x, y, col, _T("■ %s"), g_WindowName);
	DrawText(x, y += 16, col3, _T("1. 점 2개를 이용한 직선 그리기"));
	DrawText(x, y += 16, col3, _T("2. 더블버퍼링 : \"BackBuffer\" 추가."));
	DrawText(x, y += 16, col3, _T("   + 속도/깜박거림 비교."));
	DrawText(x, y += 16, col3, _T("3. 점 3개를 이용한 삼각형 그리기"));
	DrawText(x, y += 16, col2, _T("4. 삼각형 그리기[사용자 함수 버전]"));

	x = 1, y = 1;
	DrawText(x, y += 16, col, _T("도움말: F1"));
	DrawText(x, y += 16, col, _T("정점정보: F2 (%s)"), g_bShowVtxInfo ? _T("ON") : _T("OFF"));

	y += 16;
	DrawText(x, y += 16, col, _T("[Line]"));
	DrawText(x, y += 16, col, _T("정점 입력 : L-Button"));
	DrawText(x, y += 16, col, _T("정점 리셋 : R-Button"));

	for (int i = 0; i < PT_MAX_; i++)
	{
		DrawText(x, y += 16, col, _T("v%d = {%d, %d}"), i, g_Vtx[i].x, g_Vtx[i].y);
	}
}

///////////////////////////////////
// 타이머의 초당 프레임률(fps)을 출력
// params : 출력할 화면 좌표 (2d x,y)
void PutFPS(int x, int y)
{
	static UINT frm = 0;
	static float fps = 0.0f;
	++frm;
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG nowtime = GetTickCount64();

	UINT time = (UINT)(nowtime - oldtime);
	if (time >= 1000)
	{
		fps = (float)(frm * 1000) / (float)time;
		frm = 0;
		oldtime = nowtime;
	}

	//DrawText(x, y, _T("FPS=%.1f/%d      "), fps, time);
	DrawText(x, y, RGB(255, 255, 255), _T("FPS=%.1f/%d      "), fps, time);
}

///////////////////////////////////
// 텍스트 출력
void DrawText(int x, int y, COLORREF color, TCHAR* msg, ...)
{
	TCHAR buff[128] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);
	
	RECT rc = { x, y, x + 800, y + 600 };

	SetTextColor(g_hRT, color);
	DrawText(g_hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(g_hRT, RGB(255, 255, 255));
}

///////////////////////////////////
// 장면 그리기 종료
void EndScene()
{
	//ReleaseDC(g_hWnd, g_hDC);    // 화면DC 사용안함. 렌더타겟 사용
}

///////////////////////////////////
// 장면 출력
void Present()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	HDC hdc = GetDC(g_hWnd);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_hRT, 0, 0, SRCCOPY);
	ReleaseDC(g_hWnd, hdc);
}

///////////////////////////////////
// 렌더타겟 제거
void ReleaseRenderTarget()
{
	DeleteObject(g_hBmpRT);
	DeleteDC(g_hRT);
}