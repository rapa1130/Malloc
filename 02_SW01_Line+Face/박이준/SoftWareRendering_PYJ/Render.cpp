//! 
//! \file	Render.cpp
//! \brief	기본 렌더링 프레임워크 소스
//!			DirectX 기준 버전 : DX12
//! 
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com 
//! \date   2003.11.04. Updated. DX9.x 
//! \date	2005.09.23. Updated (.Net)
//! \date   2009.12.01. Updated.
//! \date   2010.12.01. Updated. DX11, Jun.2010
//! \date   2016.12.27. Updated. DX11/12, Window SDK 8.1 / Window 10 SDK 10.0.18362
//! \date   2018.12.30. Updated. DX12.x, Windows 10 SDK 10.0.18362
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//!

//#pragma warning(disable:4996)// 00단계. Vsstudio 경고 끄는 코드(오래된 c함수 사용시)
#pragma warning(disable:4267 4996) // 01단계 
#include "Windows.h"
#include "stdio.h"
#include "tchar.h"
#include "Render.h"
#include <cmath>
TCHAR* g_WindowName = _T("Yena::SW-Renderer 05_Face_(+Filling)(과제2)★_박이준");

/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//  
extern HWND g_hWnd;
//다른 cpp파일에 정의된 전역 윈도우 핸들을 사용하겠다고 선언
// -> extern

POINT g_MovePenLoc = { 0,0 };
// 펜을 움직이기 위해 사용되는 변수
// 구조체를 이용해서 x,y의 좌표를 저장

// 라인의 시작 / 끝.
//POINT g_Sp, g_Ep;
// POINT는 WinAPI 좌표 구조체이다 x,y(LONG) Sp는 시작점, Ep는 끝점

enum { // 점 입력 상태를 표시하기 위한 값들
	PT_NONE_ = 0x00, // 설정된 점 없을 시 
	//PT_SP_ = 0x01, // 1번째 점 설정 됐을 시,
	//PT_EP_ = 0x02, // 2번째 점 설정 됐을 시.
	// 이제 라인이아닌 삼각형을 그리기 위해 점 3개 설정하기

	PT_V0_ = 0x01, // 비트 플래그 1
	PT_V1_ = 0x02, // 2
	PT_V2_ = 0x04, // 4
	// 0001 0010 0100 이기때문에 | (or) 써서 비트마스크 방식으로 상태 저장

	PT_MAX_ = 3,
	PT_1_ = PT_V0_, // g_Vtx[0]
	PT_2_ = (PT_V0_ | PT_V1_), // g_Vtx[1] = 클릭좌표;
	PT_3_ = (PT_V0_ | PT_V1_ | PT_V2_), // 삼각형 완료 상태 0111 << g_Vtx[2]



	PT_COMPLETED_ = (PT_V0_ | PT_V1_ | PT_V2_)  // 모든 점 입력 완료 -> 삼각형 그리기

	// 점 입력 완료 -> 라인 그리기
	// 여기서 | 는 비트OR이다 -> sp ep - 0x01 0x02 -> 0001 0010
	// or결과 -> 0011 이 될때 Completed 되는 것임 (시작점,끝점 둘다있다)

	// (if) 만약 bool로해서 &&같은거로하면 어떻게 될까?
	// bool로 가능은 함근데 하나로 여러 상태를 저장하려함
	// 비트 하나하나가 상태 -> OR 사용  (조건검사 X 비트 상태합치기(목적다르다))
};

	POINT g_Vtx[PT_MAX_]; // POINT는 WinAPI 좌표 구조체 [0].x, y라는 뜻
	POINT g_VtxClear = { 0,0 }; // 마우스 움직일때 실시간 좌표 저장용
	// WM_MOUSEMOVE에서 lParam넣었던 그 변수
	POINT g_VtxTemp = { 0,0 }; // 초기화용 기준 좌표 

	DWORD g_PtCheck = PT_NONE_; // gPtcheck == 0x07이면 점 3개 입력 완료

//DWORD g_PtCheck = PT_NONE_; // 라인 구성 정점, 설정 완료 여부
// 현재 점 입력 상태를 저장하는 전역변수 (처음에는 PT_NONE_)
// |= PT_SP_ (첫점) 그다음 g_PtCheck |= PT_EP_;
// a|=b -> 아 그냥 축약이구나 a= a|b;ㅋㅋ;

//HDC g_hDC; // 출력화면 (RenderTarget)
// 전역 HDC -> 이전에는 DrawText에서 GetDC가져와서 Release했음
// 이번에는 렌더링용 DC를 전역으로 잡아두려고 쓰려한다
// 싱글버퍼 = 내가 그리는 대상 = 실제화면 (Flickering 생길 수 있음)
// 01 싱글버퍼일때 사용했었음

// 02 Double Buffer일때는 렌더 타겟 구성용 핸들
HBITMAP g_hBmpRT = NULL;
// HBITMAP [중요] 메모리에 존재하는 그림(이미지) = 픽셀 저장 공간
// 메모리 Bitmap에 먼저 그리기 위함 -> BackBuffer 역할을 한다
// RT = RenderTarget / g_hBmpRT = 렌더 타겟 비트맵 핸들이다

HDC		g_hSurfaceRT = NULL;
// 메모리용 DC 메모리 bitmap에 그리는 도구!!
// g_hSurfaceRt -> g_hBmpRT(메모리 비트맵)

COLORREF g_BkColor = RGB(0, 0, 255); // 배경색 파란색 빨 초 파 니까
#define g_hRT	g_hSurfaceRT // [중요] g_hRT 이름을 쓰면
// 실제로는 g_hSurfaceRT 사용한다 (g_hRT전처리 후 g_hSurfaceRT로 치환
// g_hRT라고 쓴거 컴파일전에 g_hSurfaceRT로 바꿔라(define)

// 라인 그리기 팬
// HPEN = Win32 GDI의 선 그리기 도구
HPEN g_hPenDash; // 점선 형태 펜
HPEN g_hPenWhite; // 흰색 펜
HPEN g_hPenGreen; // 초록색 펜

// 펜 여러개 만드는 이유 : GDI 선 스타일마다 객체를 따로 생성해야한다

//정점 정보 보기
BOOL g_bShowVtxInfo = TRUE;
// Win 32 BOOL 타입 실제로 typedef int BOOL 이다

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  
// 렌더링 데이터 불러오기 및 해제.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//! 게임 데이터 및 렌더링 자원을 불러옵니다.
//! 
//! \return 성공시 OK, 실패시 FAIL.
// ex) 백버퍼, 이미지 로딩, 텍스처 준비, 색상 버퍼 만들기 등

int DataLoading() { // 한 번 호출되는 초기화 함수
	// 데이터 로딩/생성 코드는 여기
	// ..

	//g_Sp.x = 0; g_Sp.y = 0; // 시작 점 초기화
	//g_Ep.x = 0; g_Ep.y = 0; // 끝 점 초기화
	// 초기화X -> 쓰레기값 들어갈 수 있다

	//g_PtCheck = PT_NONE_;
	// 아직 어떤 점도 입력되지 않았습니다
	
	
	//추가 코드는 여기에...
	//...
	LineClear(); // 이전 상태값, 정점 데이터가 남아있지 않도록!

	// 위에는 선 이을 때!
	// 라인용 펜 생성!

	//CreatePen (스타일, 두께, 색상)으로 객체 생성
	g_hPenDash = CreatePen(PS_DASH, 1, RGB(255, 255, 255));
	g_hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	g_hPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
//
//! 게임 데이터 및 렌더링 자원을 해제합니다.
//!
//! \param	없음.
//! \return	성공시 OK, 실패시 FAIL.
// 동적 메모리 delete, 비트맵 DeleteObject, DC ReleaseDc, 파일 닫기등

void DataRelease() {
	// 데이터 해제 코드


	// 라인용 팬 제거
	DeleteObject(g_hPenDash);
	DeleteObject(g_hPenWhite);
	DeleteObject(g_hPenGreen);
} // -> 리소스 해제 함수 (CreatePen()으로만든 GDI 객체들을 메모리에서 제거한다)
// OS 그래픽 리소스, 핸들 객체, 커널/GDI 메모리 쓰니까 제거해줘야합니다!

//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// Rendering Object.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////// 
//
// 모든 그리기 정보 초기화...
//

void LineClear() // 정점 상태 리셋 함수!
{ // 삼각형 입력 상태, 마우스 임시 좌표, 입력 플래그 초기화!!
	for (int i = 0;i < PT_MAX_;i++) {
		g_Vtx[i] = g_VtxClear;
		// g_Vtx[i].x = 0; g_Vtx[i].y = 0;이랑 같은것!
	} // g_Vtx 3개의 정점 전부 초기화한다 g_VtxClear = {0,0}이니까 x,y 0으로!
	g_VtxTemp = g_VtxClear; // 마우스 위치용 임시 좌표도 초기화(실시간 미리보기 좌표제거)

	g_PtCheck = PT_NONE_;
	// [핵심] PT_NONE_ = 0x00 -> 아무 정점도 입력 안됨 (안하면 이전 상태 남는다)
	// -> if (0111상태) -> 초기화X 이면 g_PtCheck == PT_COMPLETED -> 삼각형 그림
}


////////////////////////////////////////////////////////////////////////////// 
//
void LineUpdate(POINT pt) { // pt(새로 입력된 좌표(마우스 클릭위치))
	switch (g_PtCheck) // 초기 새로 시작할 경우 
	{ // 현재 점 입력 상태
	case PT_NONE_: // 아직 아무 점 없음. 새로운 선 시작

	case PT_COMPLETED_:
		// 삼각형시 -> 아무 점도 없거나 이전 삼각형이 완성된 상태
		// 이전 선이 완성. 새로운 선 다시 시작
		/*g_Sp = pt;	g_PtCheck = PT_SP_; // 1번째 입력 완료
		g_Ep.x = 0;	g_Ep.y = 0; // 이전 선의 2번째점 좌표 0으로 초기화
		break;*/
		g_Vtx[0] = pt; // 첫 클릭 시 저장 (정점)
		g_Vtx[1] = g_VtxClear; // 초기화
		g_Vtx[2] = g_VtxClear; // 초기화 
		g_PtCheck = PT_1_; // PtCheck를 PT_1_로 변경
		break;

	case PT_1_:
		g_Vtx[1] = pt; // 두번째 클릭시 저장 (정점)
		g_PtCheck = PT_2_; // 상태 변경 PtCheck = PT_2_
		break;

	case PT_2_: // 세번째 정점 클릭시 
		g_Vtx[2] = pt; // 정점 저장하고
		g_PtCheck = PT_COMPLETED_; //PtCheck를 변경
		break;
	}
	/*	// 2번째 입력 시 ( 이전 선 그리기 때 사용)
	case PT_SP_: // 이미 시작점이 있는 경우 g_EP 끝점 처리
		g_Ep = pt;	g_PtCheck = PT_COMPLETED_; // 입력 완료 처리
		break;
	}*/
} // 클릭전 NONE -> 1번 클릭 COMPLETED -> 2번째 SP -> 3번째 Completed

void LineDraw() // GDI로 선을 직접 그려보자!
{ //시작점 표시, 선 그리기, 끝점 표시를 담당한다
	// 무엇을 화면에 그릴지 결정하는 함수
	switch (g_PtCheck) { // 현재 상태를 검사합니다(클릭클릭)
	case PT_NONE_: // 아무런 점 없음
		break; //그리기 없음

	/*case PT_SP_: // 시작점만 찍힌 상태 (점선 그리기 시)
		CrossDraw(g_Sp); // 그린다 시작점 위치에 +를 표시
		break;
		*/

	case PT_1_ : // 첫번째 표시
		CrossDraw(g_Vtx[0], _T("v0")); // v0표시해주고
		LineDashDraw(g_Vtx[0], g_VtxTemp); // 임시라인 표시 (쇄선)
		break;

	case PT_2_:
		CrossDraw(g_Vtx[0], _T("v0")); // v0 표시
		CrossDraw(g_Vtx[1], _T("v1")); // v1 표시
		LineDashDraw(g_Vtx[0], g_VtxTemp); // 임시라인 표시 v0 - 마우스
		LineDashDraw(g_Vtx[1], g_VtxTemp); // 임시라인 표시 v1 - 마우스
		LineDraw(g_Vtx[0], g_Vtx[1]); //라인 그리기 v0 -> v1
		break;

	case PT_COMPLETED_: //완료 시
		// 시작점과 끝점 둘다 존재하는 경우
		//CrossDraw(g_Sp); // 시작점을 그린다에요
		//MoveToEx(g_hRT, g_Sp.x, g_Sp.y, NULL); // [중요]
		// 펜의 위치를 이동 시킨다(선을 그리지는 않고
		// 시작위치로 펜을 이동시킨다) = g_Sp.x,y <
		// NULL은 이전 펜 위치 저장 X

		//g_hDC에서 g_hRT로 변경 -> 실제화면DC가 아닌 백버퍼 DC로 이동

		// MoveToEx -> 펜이동만 한다
		// LineTo -> 현재 위치부터 선을 그린다

		//LineTo(g_hRT, g_Ep.x, g_Ep.y);
		// [핵심] 현재 펜 위치에서 지정 위치까지 선 그리기
		// 현재 위치 = 시작점, 목표 위치 = 끝점
		//CrossDraw(g_Ep); // 끝점을 그린다에요
		// 끝점에도 십자 표시를 해준다

		//삼각형 그리기
		if (g_bShowVtxInfo) { // 정점 정보 출력 (0,1,2) (정점 디버그 정보 보여줄지 여부)
			for (int i = 0;i < PT_MAX_;i++) { // 0,1,2 반복
				CrossDraw(g_Vtx[i], _T("v%d"), i); // 특정 위치 십자 표시 + 텍스트
			} // _T는 printf 스타일 포맷 문자열
		}
		FaceFilling();
		FaceDraw(); // 삼각형 그리기! (Rasterization)
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
//! 직선 그리기 : sp -> ep
//! 
//! \param	sp	시작점
//! \param	ep	끝점
//

void LineDraw(POINT sp, POINT ep) // 두 점 사이 실제 선을 그리는 함수
{ // 시작점sp 끝점 ep까지!
	MyMoveToEx(sp.x, sp.y); // 펜 위치만 이동합니다 (그리지 않음!)
	// NULL : 이전 펜 위치를 저장 안한다
	MyLineTo(g_hRT, ep.x, ep.y);  // 실제 선을 그립니다
	// 현재 펜 위치에서 ep.x, ep.y 까지 선을 생성(현재 펜 위치를 기준으로)
}


////////////////////////////////////////////////////////////////////////////// 
//
//! 쇄선(임시라인) 그리기 : sp -> ep
//! 
//! \param	sp	시작점
//! \param	ep	끝점
//
void LineDashDraw(POINT sp, POINT ep) // 임시 점선 그리기!!
{ // 마우스 움직일 때 확정되지 않은 선을 보여준다
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenDash);
	// SelectObject(현재 DC(g_hRT)에 점선 펜 장착) 
	// HPEN > 이전에 사용 중이던 펜 (현재 펜 -> 점선 펜으로 교체한다)
	// 현재 DC - 펜, 브러시, 폰트 등이 있음(이전 펜 저장 이유) 작업 후 되돌려야함
	MyMoveToEx(sp.x, sp.y);
	// 이동한다잇!!
	MyLineTo(g_hRT, ep.x, ep.y);
	// 점선으로 그린다
	SelectObject(g_hRT, hOldPen);
	// 현재 펜=원래 펜 
}

////////////////////////////////////////////////////////////////////////////// 
//
//! 삼각형 그리기.
//
void FaceDraw() {
	LineDraw(g_Vtx[0], g_Vtx[1]); // 0번째 점, 1번째점 잇기
	LineDraw(g_Vtx[0], g_Vtx[2]); // 0번째 점, 2번째 잇기
	LineDraw(g_Vtx[1], g_Vtx[2]); // 1번째 2번째 잇기
	// 삼각형 완성
}

//삼각형 채우기 함수!
void FaceFilling() {
	int minX; int maxX; // 맨왼쪽(minX) , 맨오른쪽(maxX)
	int minY; int maxY; // 맨위(minY), 맨아래(maxY)
	// 삼각형을 바로 하기 어려우니까
	// 네모를 만들어서 최대최대 네모를 (정점 크기)
	// 검사해서(삼각형 부위만) 삼각형 안이면 찍고
	// 아니라면 안찍도록 만드는 Filling을 해보자!

	// 삼각형의 3개의 정점을 가져와서
	POINT v0 = g_Vtx[0];
	POINT v1 = g_Vtx[1];
	POINT v2 = g_Vtx[2];
	// 정점을 편하게 쓰기 위해서 잠깐 넣기

	// 사각형을 만들어서 정점이 그 사각형에 선위에 있게 만들기
	// 그래야지 일관되게 법선벡터가 +++ ---가 될테니까 (그래야 안인지
	// 밖에 있는지 확실하게 알 수 있다!
	minX = v0.x;
	if (v1.x < minX) minX = v1.x;
	if (v2.x < minX) minX = v2.x;
	minY = v0.y;
	if (v1.y < minY) minY = v1.y;
	if (v2.y < minY) minY = v2.y;
	//최소 x,y를 구하기! (제일 왼쪽x와 제일 위y)

	maxX = v0.x;
	if (v1.x > maxX) maxX = v1.x;
	if (v2.x > maxX) maxX = v2.x;
	maxY = v0.y;
	if (v1.y > maxY) maxY = v1.y;
	if (v2.y > maxY) maxY = v2.y;
	//최대 x,y를 구하기! (제일 오른쪽x와 제일 아래y)

	
	// 사각형부분을 확인하기 위해서 minY ~ maxY돌고!
	// minX ~ maxX부분을 돌자! 
	for (int firstY=minY; firstY <= maxY;firstY++) {
		for (int firstX = minX; firstX <= maxX;firstX++) {
			POINT p1 = { firstX, firstY }; // 외적을 사용하기 위해 사용하자!
			// {firstX,firstY,0} (수학)처럼 생각하자 (다만 POINT는 무조건x,y만 있다!)
			// 근데 외적은 3차원 벡터 연산이잖아 -> 확장해서 생각해!!
			// why? 어차피 2D 평면이라 z 0이잖아 ㅎㅎ~!
			// p1은 그리고 검사할 픽셀 위치이다!
			
			// 법선벡터를 구해서 다 같은 방향(+++ --- 인지 확인하자!)
			int check1 = CrossProductCheck(v0, v1, p1); 
			// v0,v1 벡터 AB 만들고! v0,p1 벡터 AC 만들고!!
			// 그 다음 AB AC 를 외적하는 느낌으로 가면된다!
			int check2 = CrossProductCheck(v1, v2, p1); 
			// 똑같이 v1,v2 벡터 AB / v1,p1 AC 벡터 만들고
			// AB AC 외적한다
			int check3 = CrossProductCheck(v2, v0, p1); 
			// 마찬가지로 v2,v0 벡터 v2,p1 벡터를 만들고 외적한다!
			
			// 삼각형 내부 판정은 방향이 일관되게 해야한다!
			// 외적의 부호는 선의 방향으로 결정되므로 통일되게 가야한다!
			// 방향이 바뀌면 양수->음수 음수->양수로 반대로 뒤집히므로 안된다!

			// 다 양수거나 다 음수냐!! (0을 포함한)
			if(check1 >= 0 && check2 >=0 && check3 >=0
				|| check1 <= 0 && check2 <= 0 && check3 <=0){
				SetPixel(g_hRT, firstX, firstY, RGB(255, 255, 255));
			}  // 0-- 면 선위에 있는거고 00-면 꼭짓점에 있는거고 
			// 000이면 평행한 3개의 직선(삼각형 X 일직선O)일때 그 위에 있다!
		}
	}

}
 
// 3개의 정점이 다 정말루 안에 있는지 확인하기 위해서 만든 함수!
// 외적을 사용하여 접근해서 다 ---가 되거나 +++ 이되야한다!(네모안에서 
// 달라질 수 없음 -+- 나 +-+ 이나 --+이나 되면 삼각형 안이아님!!
INT CrossProductCheck(POINT a, POINT b, POINT c)
{ // (ax,ay,0) (bx,by,0) (firstX,firstY,0)이라고 생각하면된다(3차원으로만 되니까!)
	/* 
	* 우선 벡터를 만든다 AB AC라고 생각하고!
	AB = b.x - a.x (x) b.y - a.y (y) 0 (z) < POINT a,b 두개의 벡터
	AC = c.x - a.x (x) c.y - a.y (y) 0 (z) < POINT b,c 두개의 벡터

	AB AC 외적
	x 성분 (b.y-a.y)*0 - 0(c.y-a.y) = 0
	y 성분 (b.x-a.x)*0 - 0*(c.x - a.x) = 0
	z 성분 (b.x-a.x) * (c.y-a.y) - (b.y-a.y) * (c.x-a.x)
	(0,0,z) // x,y (2차원좌표로만 했으니 외적시 무조건 z값만 나온다 법선벡터!)
	*/
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	// ABAC외적한것을 (0,0,z)법선벡터니까 2D에선 z만 나옴! 리턴한다!
	// INT로 리턴하는 이유는 양수거나 음수거나 0이 나오기 때문에!
}

//펜을 이동시키는 함수를 직접 제작해보자 (그리기 없음) 그냥 펜만이동이야!!
BOOL MyMoveToEx(int x, int y)
{ // HDC(h_gRT)를 안쓰는이유 -> 원래 winapi에서는 HDC를 모르기 때문에 매개변수로 보내줘야했었음
	// 하지만 여기서는 따로 함수를 만들어서 사용하기에 전역변수인것들을 자연스럽게 사용가능
	// 그렇기에 넣어주지 않아도 된다에요

	//LPPOINT도 현재는 이전 펜 위치를 알려주는 WinAPI함수인데 현재 위치 저장만 필요하기에
	// 굳이 지금은 사용하지 않는다. 원래쓰던 MoveToEx에서도 NULL을 입력했었음(필요없다)
	g_MovePenLoc.x = x;
	g_MovePenLoc.y = y; // 위치를 넣어준다 (구조체)

	return TRUE;
	//MoveToEx(hdc, 1, 1, NULL);//원래 사용되는 함수
	/* 매개 변수
		[in] hdc 디바이스 컨텍스트에 대한 핸들입니다.
		[in] x 새 위치의 x 좌표(논리 단위)를 논리 단위로 지정합니다.
		[in] y 새 위치의 y 좌표(논리 단위)를 논리 단위로 지정합니다.

		[out] lppt 이전 현재 위치를 수신하는 POINT 구조체에 대한 포인터입니다.
		이 매개 변수가 NULL 포인터이면 이전 위치가 반환되지 않습니다.
		반환 값 함수가 성공하면 반환 값이 0이 아닙니다.

		함수가 실패하면 반환 값은 0입니다.
		*/


}


BOOL MyLineTo(HDC hdc, int x, int y) // SetPixel사용을 위해 핸들을 받고 펜 위치에서
// 어디까지 그릴지를 생각해서 x,y를 받는다(last로)
{	// 펜 위치 g_MovePenLoc에서 목표위치(x,y)로 받은 곳까지 선을 그린다에요
	// WinApi LineTo를 대신해보자!
	float startX = g_MovePenLoc.x; // (이동한 펜 위치(MyMoveTo에서))
	float startY = g_MovePenLoc.y; 
	// 시작점

	float lastX = x; //  (펜이 그리면서 멈출 위치)
	float lastY = y;
	// 끝점

	// 수가 더 많은 경우를 계산해서 찍어준다에요
	float pixelX = startX;
	float pixelY = startY;
	// 찍을 픽셀을 미리미리 픽업해두기
	// pixelX,Y에서 점을 찍을거에요


	// 가로선 그리기!
	if (startY == lastY) // 가로선일때 (100,0) (400,0)이럴때 ( 세로가 움직이지 않음)
	{
		if (startX < lastX) { // 만약 startX가 lastX보다 작으면
			for (pixelX; pixelX <= lastX; pixelX++) { // pixelX가 커질때만 그려진다
				SetPixel(g_hRT, pixelX, (int)pixelY, RGB(255, 255, 255));
			} // 
		}
		else { // 또는 startX 가 같거나 lastX보다 크면
			for (pixelX; pixelX > lastX; pixelX--) { // pixelX가 작아질때만 그려진다
				SetPixel(g_hRT, pixelX, (int)pixelY, RGB(255, 255, 255));
			}
		}
	}  // 여기는 이제 y위치가 같을때에만 그려진다

	// 세로선 그리기! (x의 위치가 그대로 일때!)
	else if (startX == lastX) // 세로선일때 (0,100), (0,300) 이런식
	{
		if (startY < lastY) { // startY가 작을때(더 위일때)
			for (pixelY; pixelY <= lastY; pixelY++) { // pixelY가 커지며(내려갈때) 그려진다
				SetPixel(g_hRT, (int)pixelX, pixelY, RGB(255, 255, 255));
			}
		}
		else { // startY가 더커용(더 아래일때)
			for (pixelY; pixelY > lastY; pixelY--) { // pixelY가 작아지며(올라갈때) 그려진다
				SetPixel(g_hRT, (int)pixelX, pixelY, RGB(255, 255, 255));
			}
		}
	}
	// 대각선으로 그릴때 (x,y 고정이 아닐때!)
	else { // y의 수가 더 많을 경우
		float manyX = abs(lastX - startX); // 그려지는 총 숫자를 비교해보자!
		float manyY = abs(lastY - startY);
		// X가 많거나 Y가 많거나 이런걸 계산하기 위함! (그래야 기울기가 0에 가까워질때
		// 적게 그려지는걸 방지할 수 있음!)


		float inclination = (float)(lastY - startY) / (float)(lastX - startX);
		// 기울기 구하기

		if (manyX > manyY) { // 만약 X가 Y보다 많을 경우! (X를 기준으로 그려보자!)
			if (startX <= lastX) // 시작점이 끝점보다 작을때!(더 왼쪽일때)
			{
				for (pixelX; pixelX <= lastX; pixelX++) { // 끝점까지 커지면서(오른쪽으로) 
					pixelY = inclination * (pixelX - startX) + startY;
					//pixelX가 커지면서 pixelY를 바꾼다 -> x위치에 대응하는 y 구한다!
					SetPixel(g_hRT, pixelX, (int)pixelY, RGB(255, 255, 255));
				}
			} // 만약 pixelX나 Y를 잘못더하거나 빼면 무한 루프가 돌 수 있음
			// if(pixelY > lastY인데 pixelY가 ++가 되거나 하면 무한루프)
			else {// 시작점이 끝점보다 클때!(더 오른쪽이다에요)
				for (pixelX; pixelX > lastX; pixelX--) { // 끝점까지 작아지면서(왼쪽으로)
					pixelY = inclination * (pixelX - startX) + startY; 
					//PixelX에 위치하는 Y를 구한다에요!(x가 -- 되니까!)
					SetPixel(g_hRT, pixelX, (int)pixelY, RGB(255, 255, 255));
				} // 그리기!

			}
		}
		else if (manyY > manyX) { // 만약 X가 Y보다 적은경우!(Y를 기준으로 그리자!)
			if (startY <= lastY) // 시작Y가 더 작아요! (위에요!)
			{
				for (pixelY; pixelY <= lastY; pixelY++) { // Y는 커지면서 찍을거에요(아래로!)
					pixelX = (pixelY - startY) / inclination + startX; 
					//PixelY에 위치하는 X를 구한다 -> Y가바뀌면 X도 바뀌니까!
					SetPixel(g_hRT, (int)pixelX, pixelY, RGB(255, 255, 255));
				}
			} // 만약 pixelX나 Y를 잘못더하거나 빼면 무한 루프가 돌 수 있음
			// if(pixelY > lastY인데 pixelY가 ++가 되거나 하면 무한루프) 조심!
			else { // 시작 Y가 더 커요!(더 아래에요!)
				for (pixelY; pixelY > lastY; pixelY--) { // pixelY가 작아지며(위로가며) 그리기
					pixelX = (pixelY - startY) / inclination + startX;
					SetPixel(g_hRT, (int)pixelX, pixelY, RGB(255, 255, 255));
				}

			}
		}
	}
	/*  -> 실패 (기울기가 0이되는 경우(세로선,가로선) 무제한에 가까워지는 숫자가나와
	* 그림이 이상하게 그려지거나 안그려지게 된다 그러므로 다시 !
	if (abs(startX) < abs(lastX)) 
	{
		for (pixelX; pixelX <= lastX; pixelX++) { // pixelX가 커질때만 그려진다
			pixelY = inclination * (pixelX - startX) + startY; // 직선의 방정식(두 점을 지나는)
			SetPixel(g_hRT, pixelX, (int)pixelY, RGB(255, 255, 255));
		}
	}
	else if (abs(startX) > abs(lastX))
	{
		for (pixelX; pixelX > lastX; pixelX--) { // pixelX가 커질때만 그려진다
			pixelY = inclination * (pixelX - startX) + startY; // 직선의 방정식(두 점을 지나는)
			SetPixel(g_hRT, pixelX, (int)pixelY, RGB(255, 255, 255));
		}
	}
	else { 
		if (abs(startY) <= abs(lastY))
		{
			for(pixelY; pixelY <= lastY; pixelY++) {
				pixelX = (pixelY - startY) / inclination + startX; // 직선의 방정식(두 점을 지나는)
				SetPixel(g_hRT, (int)pixelX, pixelY, RGB(255, 255, 255));
			}
		}
		else {
			for (pixelY; pixelY > lastY; pixelY--) {
				pixelX = (pixelY - startY) / inclination + startX; // 직선의 방정식(두 점을 지나는)
				SetPixel(g_hRT, (int)pixelX, pixelY, RGB(255, 255, 255));
			}

		}
	} * /


	/* => 실패 선이 이상하게 그려진다 (+일때만을 가정해버려서)
	if (abs(lastX - startX) > abs(lastY - startY)) { // x의 수가 더 많을 경우
		for (pixelX; pixelX <= lastX; pixelX++) { // pixelX가 커질때만 그려진다
			pixelY = inclination * (pixelX - startX) + startY; // 직선의 방정식(두 점을 지나는)
			SetPixel(g_hRT, pixelX, (int)pixelY, RGB(255, 255, 255));
		}
	}

	else { // y의 수가 더 많을 경우
		for (pixelY; pixelY <= lastY; pixelY++) {
			pixelX = (pixelY - startY) / inclination + startX; // 직선의 방정식(두 점을 지나는)
			SetPixel(g_hRT, (int)pixelX, pixelY, RGB(255, 255, 255));
		}
	}
	*/

	//LineTo(hdc, x, y);
	/*매개 변수
		[in] hdc 디바이스 컨텍스트에 대한 핸들입니다.

		[in] x 줄 끝점의 x 좌표를 논리 단위로 지정합니다.

		[in] y 줄 끝점의 y 좌표를 논리 단위로 지정합니다.

		반환 값 함수가 성공하면 반환 값이 0이 아닙니다.
		함수가 실패하면 반환 값은 0입니다.
		현재 펜을 사용하여 선이 그려지고 펜이 기하학적 펜인 경우 현재 브러시가 그려집니다.

		LineTo가 성공하면 현재 위치가 지정된 끝점으로 설정됩니다.
		*/

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// Engine / System Functions
//
////////////////////////////////////////////////////////////////////////////////


/*
////////////////////////////////////////////////////////////////////////////// 
//
//! 십자선 그리기 : 마우스 클릭 위치
//
void CrossDraw(POINT pt) // 십자선을 그리는 함수(끝점,시작점)
{
	MoveToEx(g_hRT, pt.x - 5, pt.y, NULL);
	//이동하기 점 왼쪽에서 -5해서 y고정!
	LineTo(g_hRT, pt.x + 5, pt.y);
	//그린다 +5까지 (-5~5 y고정)

	MoveToEx(g_hRT, pt.x, pt.y - 5, NULL);
	// 이동하기 점 아래로 -5해서 x고정!
	LineTo(g_hRT, pt.x, pt.y + 5);
	// 그린다 +5 (-5~5 x고정)

	// -> ---------- 그리고 
	// |
	// |
	// | 그리는거 그러면 +로 보일테니까
} // CrossDraw를 따로 만든 이유 ? -> 재사용을 위해서
// g_hDC에서 g_hRT로 다보낸다 -> 백버퍼로 ! 
*/


////////////////////////////////////////////////////////////////////////////////
//
//! 십자선 그리기 : 마우스 클릭 위치
//! 
//! \param	pt		출력 위치
//! \param	name	출력 문자열
//
void CrossDraw(POINT pt, TCHAR* name, ...) // 특정 좌표에 십자 + 텍스트 출력
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenGreen); //현재 DC에 greenPen장착!
	// 즉 이후 그려지는 초록을 g_hRT에 흰색을 hOldPen에 저장한다
	MyMoveToEx(pt.x - 5, pt.y); // 가로선 그리기 -5에서
	MyLineTo(g_hRT, pt.x + 5, pt.y); // 5까지 쭉 (y고정)

	MyMoveToEx(pt.x, pt.y - 5);
	MyLineTo(g_hRT, pt.x, pt.y + 5); //세로선 -5에서 5까지 쭉 (x고정) 
	// 십자선그려짐

	va_list vl; // 가변 인자 처리용 변수
	TCHAR buff[80] = _T(""); // buff의 저장
	va_start(vl, name); //  name 뒤부터 가변 인자 시작
	_vstprintf(buff, name, vl); // 출력!!!! TCHAR버전 sprintf()입니다
	// va_end(vl); // 원래는 반드시 호출하는게 정석입니다 플랫폼 따라 문제 생길수잇음
	TextOut(g_hRT, pt.x, pt.y, buff, _tcslen(buff)); // 문자열 출력 함수(v0,v1,v2)

	SelectObject(g_hRT, hOldPen);// 원래 펜 복구, 안하면 이후 모든 선이 초록색이됨
	// 흰색으로 복귀
}

////////////////////////////////////////////////////////////////////////////// 
//
//! 장면 그리기 시작. : 렌더링에 필요한 (디바이스의) 선위 작업을 수행.
//

void BeginScene() // 장면을 그리기 전에 준비한다에요
{
	//g_hDC = GetDC(g_hWnd); // 현재 윈도우 g_hWnd에 그릴 수 있는
	// GDI도구 HDC를 가져와서 전역 g_hDC에 저장한다
	// 이제 다른 함수에서 g_hDC를 사용해서 그리기 가능하다
	
	// 화면을 모니터에 바로 출력X 백버퍼로 갈거니까

	SetBkMode(g_hRT, TRANSPARENT); // 백버퍼 DC에 설정한다!
	// 텍스트를 출력할 때 글자 배경을 투명하게 한다
	// 이전에는 SetBkColor로 글자 뒤 배경색을 칠했는데 
	// Transparent로 하면 글자 뒤 배경 사각형을 칠하지 않는다
}


////////////////////////////////////////////////////////////////////////////// 
//
//! 장면 그리기 종료. : 렌더링에 필요한 (디바이스의) 후위 작업을 수행.
//
void EndScene() // 장면 그리기가 정리된 이 후
{
	//ReleaseDC(g_hWnd, g_hDC); // GetDC로 가져온 그리기 도구를
	// Windows에 반납만해준다! 

	//화면 DC 사용안할거다 -> 렌더타겟을 계속 사용한다
	// RenderTargetCreate에서 한번 만들어서 계속 쓰는 백버퍼DC!!
	// RenderTargetRelease에서 해제하겠죠? 
}


////////////////////////////////////////////////////////////////////////////// 
//
//! 작업영역 "렌더타겟" 클리어.
//
void Clear(COLORREF col) { 
	// 매개변수 col -> 어떤 색으로 화면을 지울지
	HBRUSH hBrush = CreateSolidBrush(col);
	// 브러시 생성 [중요] 붓으로 칠하기 도구 HBRUSH
	// CreateSolidBrush - 단색 브러시 생성
	// FillRect가 어떤 색으로 채울지 알아야하니까!
	RECT rc; // Rect구조체 생성
	GetClientRect(g_hWnd, &rc);
	// [중요] 창의 클라이언트 영역 크기 얻기
	// &rc(주소전달)의 크기가 들어가겠죠 (client area)
	FillRect(g_hRT, &rc, hBrush);
	// 지정 영역을 브러시 색으로 가득채우기 (col색으로)
	// 화면을 지웁니다 (이전 프레임 흔적 제거)
	// 안하면 이전 프레임 그림이 남는다

	// g_hDC에서 RT로 변경 -> 메모리 상의 BackBuffer(RenderTarget)를 초기화한다

	DeleteObject(hBrush);
	// [매우중요] CreateSolidBrush로 생성한 GDI 객체 해제
	// 안하면 리소스 누수 푸슈슉
}


////////////////////////////////////////////////////////////////////////////// 
//
//! 장면 출력
//
void Present() // 렌더링 이후 잠깐 쉬는 함수
{ // 원래 그래픽스 API에서 Present는 완성된 화면 보여주는 작업
	//Swap Chain 을 Flip 하는 함수의 '이름'을 가지고 있지만...
	//여기서는 단순 지연 처리를 수행합니다.
	//너무 빠른 그리기는 깜박거림 Flickering 이 발생됩니다.
	//....

	//Sleep(5);		//--> 적당히 느리게..연출.. 5ms 쉬어라!!


	//Back->Front 장면복사 : "Flip", "Swap"!! ★
	// : 메모리-> 메모리 복사를 통한 깜박거림Flickering 의 최소화. 

	RECT rc; // RECT 구조체 생성 
	GetClientRect(g_hWnd, &rc); // ( 현재 창의 클라이언트 영역 크기 가져오기)
	HDC hdc = GetDC(g_hWnd); //  실제 화면에 그릴 DC를 잠깐 빌립니다
	// hdc는 프론트 버퍼쪽!! 사용자가 실제보는 창화면 DC로 보면 된다
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_hRT, 0, 0, SRCCOPY);
	//BitBlt는 비트맵 복사 함수 -> hdc복사, 위치0,0 크기right x bottom
	// 복사 원본은? g_hRT! 원본 위치도 0,0 / 복사방식은 SRCCOPY
	// 백버퍼 전체를 실제 화면에 그대로 복사!
	// 백버퍼에 그려둔 완성된 화면을 실제 화면 hdc(DC)로 복사한다
	ReleaseDC(g_hWnd, hdc);
	// hdc를 다시 반납합니다(빌린거니까)

	/* 
		g_hRT(백버퍼)의 (0,0)부터
		rc.right x rc.bottom 크기만큼을

		hdc(실제 화면)의 (0,0)에
		SRCCOPY 방식으로 복사해라 -> 원본(Source)을 그대로 복사해라!
	*/
}

///////////////////////////////////////////////////////////////////////////////
//
//! 백버퍼용 렌더타겟을 생성.★
//
int RenderTargetCreate(HWND hwnd)
{
	RECT rc; GetClientRect(hwnd, &rc); // 현재 창의 내부 크기 얻기
	// 백버퍼 bitmap 크기를 창 크기와 동일하게 만들려고!
	HDC hdc = GetDC(hwnd); // 현재 창의 실제 화면 DC 가져오기
	g_hRT = CreateCompatibleDC(hdc); //렌더타겟,DC 핸들 생성(Surface)
	// 실제 화면과 호환되는 메모리용 DC 생성 (아직 빈메모리)

	g_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	// 백버퍼bitmap 생성 (창크기만큼!!)
	SelectObject(g_hRT, g_hBmpRT); //(메모리 DC, Bitmap) 연결[중요]
	// g_hRT가 g_hBmpRT에 그림 그리도록 연결한다
	ReleaseDC(hwnd, hdc); // GetDC로 잠깐 빌린 화면 DC 반납
	// 실제 화면 DC 필요없어서 ( 백버퍼 준비완료 )
	
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// 
//! 렌더타겟 제거.★
//
void RenderTargetRelease() // RenderTaretCreate 에서 만든것을 제거하자!!
{ // bitmap 제거 -> DC 제거 하는거다!
	DeleteObject(g_hBmpRT); // 백버퍼 bitmap 제거!
	// GDI 객체므로 제거해줘야한다! 안 그러면 메모리 누수다!
	DeleteDC(g_hRT); //  메모리 DC 제거!!
	//ReleaseDC() 가 아님 CreateCompatibleDC() < 직접 생성한 메모리 DC

}


/////////////////////////////////////////////////////////////////////////////
//
//! 타이머의 초당 프레임률(fps)를 출력합니다.
//! 
//! \param	x, y  출력할 화면좌표(2D)
//! \return	없음.
//

void PutFPS(int x, int y) // FPS 출력하는 함수 // 어디에 출력 (x,y)
{
	static UINT frm = 0;
	// (중요) 현재까지 그린 프레임 수 
	// static으로 함수가 끝나도 값 유지

	static float fps = 0.0f;
	// 현재 계산된 FPS 저장용 (ex 60, 144, 165등)

	++frm; // (프레임 1증가) SceneRender한번 실행될때 마다
	static ULONGLONG oldtime = GetTickCount64();
	// GetTickCount64 - windows 시작 후 지난 시간을 ms로 변환
	// 1000ms -> 1s (oldtime = fps 계산 시작 시점)
	// 한번만 초기화된다에요

	ULONGLONG nowtime = GetTickCount64();
	// 현재 시간 가져오기 ex oldtime 1000, nowtime 2016


	UINT time = (UINT)(nowtime - oldtime); // 현재 시간 - 첫시간
	// 1016ms : 약 1초지남 이런식으로 시간 계산
	if (time >= 1000) { // 시간 1000넘어가면ms
		fps = (float)(frm * 1000) / (float)time;
		// float 변환 이유 : 안하면 정수 나눗셈 될 수 있오
		frm = 0; // 새 fps 계산 시작 위해 프레임 카운트 초기화
		oldtime = nowtime; // 새 기준 시간 저장
		// 1000, 2016 -> 2016 2016 -> 2016 nowtime 이런식으로 
	}

	DrawText(x, y, RGB(255,255,255), _T("FPS=%.1f/%d     "), fps, time);
	// 화면에 문자열 출력, xy(출력 위치), FPS문자열(~.1)까지 소수점
	// 시간 출력까지 함께 할 시(ex FPS =60.1/1002) 이런식으로
	// fps 프레임률, time -> 측정시간(프레임)
} // PutFPS 호출 - 프레임 수 증가 , 현재 시간 확인 / 1초 지날 경우
// if NO -> 그냥 출력 or Yes -> FPS계산, 프레임, 시간 초기화
// -> No || Yes -> 화면에 FPS/Time 출력




/////////////////////////////////////////////////////////////////////////////
//
//! 문자열을 출력합니다. (GDI)
//!
//! \param	x, y	출력 화면 좌표.
//! \param	msg		출력 문자열 (형식화 문자열 지원)
//! \return	없음.
//


/* 전에 사용하던 함수는 잠시 치워둔다

// 윈도우 화면에 문자열을 출력하는 함수
// ... -> 여러 타입 받을 수 있지만 형식 문자열이 맞아야함
// 포인터나 기본타입OK / Cpp 객체는 조심해야한다
void DrawText(int x, int y, TCHAR* msg, ...)
{ // WINAPI의 GDI를 사용해 TEXT를 그리기
	// x,y > 출력 위치, msg< 출력할 문자열
	// (...)(중요) 가변인자, 매개변수를 추가로 더 받을 수 있다
	//if(drawtext 10 10 _t(~~msg) , fps 하면 < 매개변수를 더 보내면
	// 추가 인자 fps < 를 더 받는거다
	TCHAR buff[128] = _T(""); //_T : 문자열 타입 바꾸는 매크로
	// 유니코드 설정이면 L"Hello"로 
	// 최종 출력 문자열 저장용
	// TCHAR -> 유니코드/멀티바이트 대응 문자열 타입

	// 가변 인자 처리용 변수 
	va_list vl;
	// (...)으로 들어온 추가 인자들을 읽기 위해
	// typedef char* va_list; 
	va_start(vl, msg);
	// 가변 인자 읽기 -> ... 으로 오는 인자들 읽기
	_vstprintf(buff, msg, vl);
	// if(msg(FPS=%d"), vl=60일경우 -> buff = "FPS=60"
	// [핵심] printf 스타일 문자열 생성
	// 형식 문자열 + 값들 -> 최종 문자열로 생성한다

	va_end(vl);
	//가변 인자 처리 종료 
	// ... 로 가져온 가변 인자는 그냥 자연스럽게 사라진다
	// 언제 ? 함수 호출이 끝날 때 va_end할때가 아님
	
	//COLORREF col = RGB(128, 128, 128);

	//RGB => 색상 생성 매크로
	COLORREF col = RGB(64, 64, 64); // 배경색(어두운 회색)
	COLORREF col2 = RGB(255, 255, 255);  // 글자색(흰색)

	HDC hdc = GetDC(g_hWnd);
	//[매우중요] HDC DeviceContext -> 화면에 그리기 위한도구
	// 이 창에 그림을 그릴 준비를 하겠습니다
	// g_hWnd 전역 윈도우 핸들을 넣어서 그릴 준비(현재 게임창)
	// hwnd - 어떤창인지, HDC - 그 창에 그리기 위한 도구

	//col = GetSysColor(COLOR_BACKGROUND+1);
	//SetBkColor(hdc, RGB(128, 128, 128));
	//SetBkMode(hdc, TRANSPARENT);

	SetBkColor(hdc, col);
	// SetBkColor  => 문자 배경색 설정(글자 뒤 배경)
	// handle + 색상1

	SetTextColor(hdc, col2);
	// 글자색을 이걸로 하겠습니다. handle + 색상2

	TextOut(hdc, x, y, buff, (int)_tcslen(buff));
	// 실제 출력 (hdc어디그릴지, 위치, 문자열, 문자열길이)
	// _tcslen -> Tchar용 strlen

	// [중요] GetDc로 가져온 HDC는 사용 후 반환해야함
	ReleaseDC(g_hWnd, hdc);
	// 안하면 리소스 누수 가 날 수 있음
} // DrawText 호출 - 가변인자 받고 최종 문자열 생성
// 창의 HDC얻기 - 배경색, 글자색 설정 - TextOut으로 출력
// HDC 반환 (Windows에게 그리기 자원(HDC를 빌림))
// Text Out으로 그리고 RelaseDC로 반납
// 반환 안할시 Windows내부 GDI 자원 고갈 !! (new delete느낌)

*/


/////////////////////////////////////////////////////////////////////////////
//
//! 문자열을 출력합니다. (GDI)
//!
//! \param	x, y	출력 화면 좌표.
//! \param	msg		출력 문자열 (형식화 문자열 지원)
//! \return	없음.


// 위에 버전DrawText보다 렌더링 루프 구조에 맞게 바뀐 것
void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...)
{ // COLORREF col이 추가 됐다 -> 호출 시 글자색을 직접 수정 가능
	// GetDC, ReleaseDC를 더이상 하지 않는다 << 

	//BeginScene()에서 한번 g_hDC = GetDC(g_hWnd)를 하니까
	// 그걸 공유해서 사용할 수 있음! -> 이 후 EndScene에서 해제

	// 이제는  g_hRT를 사용한다.
	// g_hRT는 백버퍼 bitmap(g_hBmpRT)에 연결된 메모리 DC이다.
	// 이제 TextOut은 실제 화면이 아닌 백 버퍼에서 출력된다
	
	TCHAR buff[128] = _T(""); // 버퍼 생성 (문자열 저장 공간)
	va_list vl; // (가변 인자 관리용 )
	va_start(vl, msg); // 가변 인자 읽기 시작 
	_vstprintf(buff, msg, vl); // [핵심] 형식 문자열 + 값
	// -> 최종 문자열 생성

	va_end(vl); // 가변 인자 종료

	//HDC hdc = GetDC(g_hWnd); -> BeginScene()에서 완료GetDC
	//col = GetSysColor(COLOR_BACKGROUND+1);
	//SetBkColor(hdc, RGB(128, 128, 128));
	//SetBkMode(hdc, TRANSPARENT);
	//SetBkColor(g_hDC, col);

	RECT rc = { x,y,x + 800,y + 600 };
	// 텍스트를 출력할영역!!

	SetTextColor(g_hRT, col); // 글자색 설정 (백버퍼dc의 현재 글자색을 col로)
	//TextOut(g_hRT, x, y, buff, (int)_tcslen(buff));
	// 실제 문자열을 출력한다!!~~
	DrawText(g_hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	//실제로 텍스트를 백버퍼에 출력 -> DTWORDBREAK때문에 글자가 영역 끝에
	// 닿으면 줄바꿈이된다

	SetTextColor(g_hRT, RGB(255, 255, 255));
	// 다음 출력에 색상이 남아 영향을 주지 않게 하는 기본색 (흰색) 복구
	//Release(g_hWnd, hdc);

}


/////////////////////////////////////////////////////////////////////////////// 
//
//! 도움말을 출력합니다.
//!
//! \param	없음.
//! \return 없음.

void ShowInfo() {

	static bool bShow = true; // bShow는 도움말을 보여줄지 말지 저장하는 값
	// static이라 함수가 다시 호출돼도 값 유지
	if (IsKeyUp(VK_F1)) bShow ^= true; // F1키 입력 감지 시 bShow값을 반대로 바꾼다
	// ^= Xor 이므로 true xor -> false , false xor -> true;
	

	PutFPS(1, 1); // FPS 출력 위치(1,1) FPS = 60.0/1000이런식으로
	
	if (!bShow) return;

	/* -> 00 단계
	// 여기서 DrawText는 내가 만든 함수 (WinAPI 원래함수X)
	// 충돌 위험 있긴해서 실무에선 MyDrawText이런식으로 이름짓기도 한다

	int x = 300, y = 50; // 문자 출력 시작 위치 300,50
	DrawText(x, y += 20, _T("■ %s"), g_WindowName);
	// y+=20하는 이유 ? 50+20 = 70 -> 300 70 문자들을 계속해서 내려가며 쓸거니까
	// g_WindowName -> 작성했던거 YENA:SW RENDER~~ BasicFrameWorks 출력
	y += 20; // 한줄 띄우기
	DrawText(x, y += 20, _T("예제 실습용 프레임워크를 제작합니다."));
	//DrawText(x, y += 20, _T("이곳에 3D 렌더링용 프레임워크를 추가합니다."));
	//DrawText(x, y += 20, _T("여기단계별 튜토리얼을 진행할 것입니다."));
	//DrawText(x, y += 20, _T("Idle Time 에 출력중으로 메세지가 깜빡거립니다."));
	DrawText(x, y += 20, _T("1. 윈도우 프레임워크 구축"));
	DrawText(x, y += 20, _T("2. Device 준비용"));
	y += 20;
	DrawText(x, y += 20, _T("[연구] 출력 문자열이 깜빡거리는 이유는?"));
	// 단일 버퍼로 그림을 그리다보니 지우고 그리는 과정에서 깜빡거리는 플리커링 발생
	// 더블 버퍼링을 사용해서 이런 과정을 없애야한다
	
	//DrawText(x, y += 20, _T("더블 버퍼링(Double Buffering)으로 깜빡거림(Flickering)을 해결 할 수 있습니다."));
	//DrawText(x, y += 20, _T("렌더링 프레임수(FPS) 출력 중. (좌측상단)"));
	*/

	int x = 300, y = 1;
	//위치 < 텍스트 출력 시작 위치
	COLORREF col = RGB(255, 255, 255); // 색상 흰색! 
	COLORREF col2 = RGB(255, 255, 0); // 색상 노란색!

	DrawText(x, y, col, _T("■ %s"), g_WindowName); 
	DrawText(x, y += 16, col, _T("1. 점 2개를 이용한 직선 그리기"));
	DrawText(x, y += 16, col2, _T("2. 더블버퍼링 : \"BackBuffer\" 추가."));
	DrawText(x, y += 16, col2, _T("   + 속도/깜박거림 비교."));
	DrawText(x, y += 16, col2, _T("3. 점 3개를 이용한 삼각형 그리기"));

	// x,y위치를 조정하고 col색상을 입힌다 _T로 내용을 작성!
	x = 1, y = 1;
	DrawText(x, y += 16, col, _T("도움말: F1"));
	DrawText(x, y += 16, col, _T("정점정보: F2 (%s)"), g_bShowVtxInfo ? _T("ON") : _T("OFF"));

	y += 16;

	DrawText(x, y += 16, col, _T("[Line]"));
	DrawText(x, y += 16, col, _T("정점 입력 : L-Button"));
	DrawText(x, y += 16, col, _T("정점 리셋 : R-Button"));
	/*
	DrawText(x, y += 16, col, _T("[Line]"));
	DrawText(x, y += 16, col, _T("좌표입력 : L-Button"));
	DrawText(x, y += 16, col, _T("Sp = {%d, %d}"), g_Sp.x, g_Sp.y);
	DrawText(x, y += 16, col, _T("Ep = {%d, %d}"), g_Ep.x, g_Ep.y);*/
}

////////////////////////////////////////////////////////////////////////////////
//
//! \brief	게임 장면을 렌더링합니다.
//! \details 매 게임루프마다 게임(예제)의 장면정보를 갱신하고 출력합니다. 
//!			 게임(렌더링) 객체들은 렌더링 전에 정보를 갱신해야 합니다.
//! \note	 예제를 진행하면서 다양한 장면 객체들을 이곳에 추가할 것입니다.
//

void SceneRender()
{
	//-------------------------------
	// 엔진/시스템 갱신
	//------------------------------- 

	//렌더링 상태, 조명, 카메라 등 조절..
	//...

	//장면 갱신 : 주인공, 몬스터, 오브제 이동, 충돌검사 등등..
	//...

	//-------------------------------
	// 장면 그리기 시작
	//-------------------------------

	//장면 렌더링 : 주인공, 몬스터, 지형.. 
	//... 
	
	//정점 정보 보기!
	if (IsKeyUp(VK_F2)) g_bShowVtxInfo ^= TRUE;

	BeginScene(); // g_hDC = GetDC(g_hWnd); 해서 이번 프레임에 그릴 준비
		Clear(RGB(128, 128, 128)); // 화면 전체를 회색으로 지운다.(이전프레임 제거)
		LineDraw(); // 현재 g_PtCheck 상태에 따라 시작점 십자,선,끝점 십자를 그린다

		ShowInfo();
		//FPS, 창 이름, 현재 Sp,Ep 좌표 같은 디버그 텍스트 출력

	EndScene(); //Release(g_hWnd, g_hDC)해서 빌린 HDC 반납
	
	//도움말 출력.
	//ShowInfo(); 

	//-------------------------------
	// 장면 출력
	//------------------------------- 
	Present(); // 지금 단계에서 실제 화면교체가 아니라 Sleep(5)로 잠깐 지연.




	//-------------------------------
	// 장면 그리기 종료.
	//------------------------------- 
	//... 


}//end of void SceneRender()






/****************** end of file "Render.cpp" *********************************/