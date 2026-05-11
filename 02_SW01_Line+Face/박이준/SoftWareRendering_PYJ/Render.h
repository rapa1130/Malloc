//! 
//! \file	Render.h
//! \brief	기본 렌더링/프레임웍 헤더 
//!			DirectX 기준 버전 : DX12
//! 
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com 
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//!
//
#pragma once
// Render.cpp에서 만든 함수들을 다른 cpp에서도 쓸 수 있게 알려주는 헤더파일


extern HWND g_hWnd;
// extern -> 어딘가에서 만들어진 것을 여기서 선언하겠습니다
// 현재 창 핸들
extern POINT g_Sp, g_Ep;
// extern -> 선의 시작점, 끝점 좌표 (다른 파일에서도 이 좌표를 읽거나 쓸 수 있다)
extern POINT g_Vtx[3]; // 어딘가에 생성된 POINT 3개짜리 배열(삼각형,다각형등 렌더링용
// Vertex(정점) 저장하는 것
extern POINT g_VtxTemp; // Temp = Temporary (임시 좌표 저장용)
// 현재 마우스 위치를 저장하는 임시 버퍼 역할

#define IsKeyDown(k)	((GetAsyncKeyState(k) & 0x8000) == 0x8000)
// 특정키(k)가 눌렸는지 확인하는 WinApi함수 -> GetAsyncKeyState
// 0x8000 현재 키가 눌려있는가 ? 
// 0x0001 지난 호출 이후 눌린 적이 있는가 ?
// 안눌리면 0000 0000 0000 0000 눌리면 1000 0000 0000 0000
// Mask(맨 왼쪽 비트만 검사) &로 눌리면 0x8000 이 되도록 하는것임!
#define IsKeyUp(k)		((GetAsyncKeyState(k) & 0x8001) == 0x8001)
// 키 입력 상태 확인용 매크로 -> IsKeyUp은 키가 올라감! 
// 1000 0000 0000 0001 최상위비트와 최하위 비트를 둘다 검사한다

// 현재 눌려있는 상태인가? + 이전 호출 이후 눌린적이 있는가?를 &로 하는것
// 현재 눌려있고 최근 눌린적 있다 -> 0x8001이 되는것! IsKeyUp과는 조금 안맞는 이름

#ifndef SafeRelease // 자원 해제용 안전 매크로
#define SafeRelease(p)	if((p)){(p) -> Release(); (p) = NULL;}
// DX 같은 COM 객체용 
#define SafeDelete(p)	if((p)){ delete (p); (p) = NULL;}
// SafeDelete(p) -> new로 만든 단일 객체 삭제
#define SafeDelArry(p)	if((p)) { delete[](p); (p) = NULL;}
// SafeDelArry(p) -> new[]로 만든 배열 삭제. 
#endif

int DataLoading();
//데이터 , 렌더링 자원 로딩 함수 (실제 구현은 Render.cpp)

void DataRelease();
// 자원 해제 함수, 프로그램 종료 시 호출 메모리+GDI 자원 정리 용도

void SceneRender();
// 매 프레임 화면을 그리는 함수 선언
// while(g_bLoop)

void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...);
// 내가 만든 DrawText 함수 선언 ( 다른 cpp에서 DrawText(10,10, _T, fps)처럼 사용가능
// 심볼 연결이므로 Render.h를 선언한 다른곳에서도 사용 가능 
// obj에서 Linker가 등장해서 발견하고 호출 -> 구현 가능 


void LineUpdate(POINT pt);
// 클릭한 좌표를 받아 시작점/ 끝점 상태를 갱신합니다
void LineDraw();
// 현재 상태에 따라 십자선과 선을 그립니다

// 위와 아래 차이 -> 위 : 내부 전역 변수 사용
// 아래 : 즉시 특정 선을 그린다

void LineDraw(POINT sp, POINT ep);
// 이건 시작점 / 끝점을 받아서 그린다
void LineDashDraw(POINT sp, POINT ep);
// 내부 전역 변수 사용 (점선 그리기)
void LineClear();
// 선 데이터 초기화 (WM_RBUTTONDOWN)에서 호출한것
void FaceDraw();
// 다각형 면 그리기 

void FaceFilling();
// 다각형 면을 채우기!

void CrossDraw(POINT pt, TCHAR * name, ...);
// 특정 위치에 십자선 출력, 

//void CrossDraw(POINT pt);
// 특정 위치에 십자 표시를 그립니다

int RenderTargetCreate(HWND hwnd);
// Render Target(백버퍼) 생성 함수 선언 -> 메모리 버퍼에 먼저 그린다
// HWND hwnd -> 현재 창 정보가 필요하기 때문에!

void RenderTargetRelease();
//백버퍼 관련 자원 해제 함수 (Memory DC 제거, Bitmap 제거, GDI 객체 정리!

//과제 작성
BOOL MyMoveToEx(int x, int y); // HDC,LPPOINT는 WinApi에서 필요한거지
// 현재 작성한 함수에서는 그다지 필요하지 않음(전역변수로 되어있기 때문에)
BOOL MyLineTo(HDC hdc, int x, int y); // SetPixel을 사용해서 HDC를 사용할 수 밖에없다에요
INT CrossProductCheck(POINT a, POINT b, POINT c); // 3개의 정점이 안쪽을 가르키는지
// 확인하기 위해 제작한 함수이다요!


/****************** end of file "Render.h" ***********************************/