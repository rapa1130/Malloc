#pragma warning(disable:4267 4996)
#include "Windows.h"
#include "stdio.h"
#include "tchar.h"
#include "Render.h"
#include <algorithm>
#include <cmath>

TCHAR* g_WindowName = _T("SWClass_Seonghyeon");
extern HWND g_hWnd;

//Line정보
POINT g_Sp, g_Ep;

struct Vec2
{
	float x;
	float y;
};

//삼각형 정보
enum {
	PT_NONE_=0x00,						
	PT_V0_=0x01,
	PT_V1_ = 0x02,
	PT_V2_=0x04,

	PT_MAX_ = 3,							
	PT_1_ = PT_V0_,						
	PT_2_ = (PT_V0_ | PT_V1_),				
	PT_3_ = (PT_V0_ | PT_V1_ | PT_V2_),		


	PT_COMPLETED_ = (PT_V0_ | PT_V1_ | PT_V2_)
};
POINT g_Vtx[PT_MAX_];
POINT g_VtxClear = {0,0};
POINT g_VtxTemp = {0,0};
//설정완료?
DWORD g_PtCheck = PT_NONE_;

//BackBuffer관련
HBITMAP g_hBmpRT = NULL;
HDC g_hSurfaceRT = NULL;
COLORREF g_BkColor = RGB(0, 0, 255);
#define g_hRT g_hSurfaceRT

HPEN g_hPenDash;
HPEN g_hPenWhite;
HPEN g_hPenGreen;

//버텍스 정보?
BOOL g_bShowVtxInfo = TRUE;

int DataLoading()
{ 
	LineClear(); 	
	g_hPenDash = CreatePen(PS_DASH, 1, RGB(255, 255, 255));   
	g_hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));  
	g_hPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));   
	return TRUE;
}

void DataRelease()
{
	DeleteObject(g_hPenDash);
	DeleteObject(g_hPenWhite);
	DeleteObject(g_hPenGreen);
}

void LineClear()
{
	for (int i = 0; i < PT_MAX_; i++)
	{
		g_Vtx[i] = g_VtxClear;
	}
	g_VtxTemp = g_VtxClear;
	g_PtCheck = PT_NONE_;
}

void LineUpdate(POINT pt)
{
	//화면에 점 3개를 찍는걸 switch로 관리
	switch (g_PtCheck)
	{
	case PT_NONE_:
	case PT_COMPLETED_:
		g_Vtx[0] = pt;
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
	//삼각형이라 버텍스3개
}
void LineDraw()
{
	switch (g_PtCheck)
	{
	case PT_NONE_:					
	case PT_1_:
		CrossDraw(g_Vtx[0], _T("v0"));		

		//LineDashDraw(g_Vtx[0], g_VtxTemp);
		LineDraw(g_Vtx[0], g_VtxTemp);
		break;

	case PT_2_:
		CrossDraw(g_Vtx[0], _T("v0"));	
		CrossDraw(g_Vtx[1], _T("v1"));	


		LineDraw(g_Vtx[0], g_VtxTemp);
		LineDraw(g_Vtx[1], g_VtxTemp);

		/*LineDashDraw(g_Vtx[0], g_VtxTemp);
		LineDashDraw(g_Vtx[1], g_VtxTemp);*/

		LineDraw(g_Vtx[0], g_Vtx[1]);
		break;
	case PT_COMPLETED_:
		if (g_bShowVtxInfo)						
		{
			for (int i = 0; i < PT_MAX_; i++)
				CrossDraw(g_Vtx[i], _T("v%d"), i);
		}
		FaceDraw();
		break;
	}
}

// https://m.blog.naver.com/oohyeat05/222137716947 직선의 방정식
// 2026-05-02 - 모든 직선이 수평으로만 그려짐 -> 직선의 방정식에 값을 잘못대입함
// 2026-05-04 - 몇몇 직선의 픽셀이 심하게 듬성듬성 그려짐;; -> 기울기가 0에 가까워질 수록 이런 현상 발생.
//															-> Y높이가 짧으면 순회 도는게 짧아서 픽셀이 적게 찍히는 현상이었음
//			  - 맨 처음 시작시 원점에서 마우스로 선이 그려짐 PT_NONE_이 아닌가?
void LineDraw(POINT sp, POINT ep)
{
	//과제 구현부
	//직선의 경우의 수
	int x1 = sp.x;
	int y1 = sp.y;
	int x2 = ep.x;
	int y2 = ep.y;

	//기울기 구하기
	float a = (float)(y2 - y1) /(float)(x2 - x1);

	//기울기에 따라 x로 순회할지 y로 순회할지 결정
	//기울기값 바꿔보면서 제일 자연스러운거 찾기
	if (std::abs(a) <= 1)
	{
		int minX = min(x1, x2);
		int maxX = max(x1, x2);
		for (int x = maxX; x >= minX; x--)
		{
			//hdc가 필요함
			float y = a * (x - x1) + y1;
			SetPixel(g_hSurfaceRT, x, (int)y , RGB(0, 0, 0));
		}
	}
	else
	{
		int minY = min(y1, y2);
		int maxY = max(y1, y2);

		for (int y = maxY; y >= minY; y--)
		{
			//hdc가 필요함
			float x = (y - y1) / a + x1;
			SetPixel(g_hSurfaceRT, (int)x, y, RGB(0, 0, 0));
		}
	}

}

void LineDashDraw(POINT sp, POINT ep)
{
	/*HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenDash);
	MoveToEx(g_hRT, sp.x, sp.y, NULL);
	LineTo(g_hRT, ep.x, ep.y);
	SelectObject(g_hRT, hOldPen);*/
}


//여기서 삼각형을 그림
void FaceDraw()
{
	//삼각형 외부에 검은 선
	/*LineDraw(g_Vtx[0], g_Vtx[1]);
	LineDraw(g_Vtx[0], g_Vtx[2]);
	LineDraw(g_Vtx[1], g_Vtx[2]);*/

	//삼각형 내부 픽셀 채우기
	FunctionToFillTriangle(g_Vtx[0], g_Vtx[1], g_Vtx[2]);
}



//한 점이 삼각형 내부일 때 : 점을 기준으로 쪼갠 삼각형 3개의 넓이 = 전체 삼각형의 넓이
//두 벡터의 외적크기 / 2 = 삼각형의 넓이
//삼각형 크게 그리면 프레임이 5...?
//int로 형변환 해서 줄여보기 -> 삼각형이 바둑판 같음
//전체 삼각형 넓이를 for문 밖으로 빼서 계산 줄여보기 -> 똑같음
// 넓이 /2하던걸 빼보기 -> 1프레임 증가;;
float cross(POINT p1, POINT p2, POINT p3)
{
	Vec2 vec1 = { (p1.x - p2.x) , (p1.y - p2.y) }; //p2 -> p1 벡터
	Vec2 vec2 = { (p1.x - p3.x), (p1.y - p3.y) };
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

void FunctionToFillTriangle(POINT p1, POINT p2, POINT p3)
{
	int minX = min(p1.x, min(p2.x, p3.x));
	int maxX = max(p1.x, max(p2.x, p3.x));
	int minY = min(p1.y, min(p2.y, p3.y));
	int maxY = max(p1.y, max(p2.y, p3.y));

	//전체 삼각형의 넓이
	float Triangle = abs(cross(p1, p2, p3));

	for (int i = minX; i <= maxX; i++)
	{
		for (int j = maxY; j >= minY; j--)
		{
			POINT target;
			target.x = i;
			target.y = j;
			if (isInTriangle(p1, p2, p3, target, Triangle))
			{
				SetPixel(g_hSurfaceRT, i, j, RGB(255, 0, 0));
			}
		}
	}
}
bool isInTriangle(POINT p1, POINT p2, POINT p3, POINT Target, float t)
{
	//임의의 점 p를 기준으로 나눈 각각의 삼각형의 넓이
	float miniTriangle1 = abs(cross(p1, p2, Target) );
	float miniTriangle2 = abs(cross(p1, p3, Target)) ;
	float miniTriangle3 = abs(cross(p2, p3, Target)) ;


	//3개의 작은 삼각형의 넓이 == 큰 삼각형 넓이
	return (t == miniTriangle1 + miniTriangle2 + miniTriangle3);
}


void CrossDraw(POINT pt, TCHAR* name, ...)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenGreen);

	/*MoveToEx(g_hRT, pt.x - 5, pt.y, NULL);
	LineTo(g_hRT, pt.x + 5, pt.y);
	MoveToEx(g_hRT, pt.x, pt.y - 5, NULL);
	LineTo(g_hRT, pt.x, pt.y + 5);
	이것도 직접 구현?
	*/

	va_list vl;
	TCHAR buff[80] = _T("");
	va_start(vl,name);
	_vstprintf(buff,name,vl);
	TextOut(g_hRT, pt.x, pt.y, buff,_tcslen(buff));
	SelectObject(g_hRT, hOldPen);
}

void BeginScene()
{
	SetBkMode(g_hRT, TRANSPARENT);
}

void EndScene()
{

}

void Clear(COLORREF col)
{
	HBRUSH hBrush = CreateSolidBrush(col);
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	FillRect(g_hRT, &rc, hBrush);
	DeleteObject(hBrush);
}

void Present()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	HDC hdc = GetDC(g_hWnd);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_hRT, 0, 0, SRCCOPY);
	ReleaseDC(g_hWnd, hdc);
}

int RenderTargetCreate(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd,&rc);
	HDC hdc = GetDC(hwnd);
	g_hRT = CreateCompatibleDC(hdc);										//렌더타겟,DC 핸들 생성(Surface)
	g_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, rc.right, rc.bottom);	//렌더타겟,비트멥 생성.
	SelectObject(g_hRT, g_hBmpRT);
	ReleaseDC(hwnd, hdc);

	return TRUE;
}

void RenderTargetRelease()
{
	DeleteObject(g_hBmpRT);
	DeleteDC(g_hRT);
}

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
	DrawText(x, y, RGB(255, 255, 255), _T("FPS=%.1f/%d"), fps, time);
}

void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...)
{
	TCHAR buff[128] = _T("");

	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);

	RECT rc = { x, y, x + 800, y + 600 };
	SetTextColor(g_hRT, col);
	DrawText(g_hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(g_hRT, RGB(255, 255, 255));
}

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
	DrawText(x,y += 16, col3, _T("1. 점 2개를 이용한 직선 그리기"));
	DrawText(x, y += 16,col3, _T("2. 더블버퍼링 : \"BackBuffer\" 추가."));
	DrawText(x, y += 16, col3, _T("   + 속도/깜박거림 비교."));
	DrawText(x, y += 16, col2, _T("3. 점 3개를 이용한 삼각형 그리기"));
	x = 1, y = 1;
	DrawText(x, y += 16, col, _T("도움말: F1"));
	DrawText(x,y += 16, col, _T("정점정보: F2 (%s)"), g_bShowVtxInfo ? _T("ON") : _T("OFF"));
	y += 16;
	DrawText(x, y += 16, col, _T("[Line]"));
	DrawText(x, y += 16, col,_T("정점 입력 : L-Button"));
	DrawText(x, y += 16, col, _T("정점 리셋 : R-Button"));
	for (int i = 0; i < PT_MAX_; i++)
	{
		DrawText(x, y += 16, col, _T("v%d = {%d, %d}"), i, g_Vtx[i].x, g_Vtx[i].y);
	}

}

void SceneRender()
{
	if (IsKeyUp(VK_F2)) g_bShowVtxInfo ^= TRUE;

	BeginScene();

	Clear(RGB(128, 128, 128));
	LineDraw();
	ShowInfo();

	EndScene();


	Present();
}