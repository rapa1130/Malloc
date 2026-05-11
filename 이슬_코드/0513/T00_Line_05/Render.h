#pragma once

////////////////////////////////////////////////////////////
//
// 함수 선언 영역
//


// 렌더링 데이터 불러오기 및 해제
int	DataLoading();
void DataRelease();

// 오브젝트 렌더링
void ClearLine();
void UpdateLine(POINT pt);  // 점 업데이트
// Bresenham을 진행하기 전 그리기 시작할 위치, 종료할 위치, 방향 정의
void DrawLinePixel(POINT sp, POINT ep);
// Bresenham알고리즘 사용 + SetPixel 호출
void Bresenham(LONG h, LONG w, LONG pW, LONG pH, LONG endW, int AddW, BOOL isWX);
void DrawLines();  // 라인 그리기
void DrawCrossLine(POINT pt, TCHAR* name, ...);
void DrawDashLine(POINT sp, POINT ep);
void DrawSolidLine(POINT sp, POINT ep);
/////////////////////////
// 과제 영역
// 삼각형 라인 그릴 부분 지정해서 DrawHorizonLine 호출
void DrawFace();
// X축 방향으로 SetPixel 호출
void DrawHorizonLine(LONG sP, LONG eP, LONG y);
/////////////////////////



// 게임 장면 렌더링
int CreateRenderTarget(HWND hwnd); // 백버퍼용 렌더타겟을 생성 ★
void SceneRender();
void BeginScene();  // 장면 그리기 시작
void Clear(COLORREF color);  // 작업영역 "렌더타겟" 클리어
void ShowInfo();  // 도움말 출력
void PutFPS(int x, int y);  // 타이머의 초당 프레임률(fps)을 출력
// 텍스트 출력
void DrawText(int x, int y, COLORREF color, TCHAR* msg, ...);
void EndScene();  // 장면 그리기 종료
void Present();  // 장면 출력
void ReleaseRenderTarget();  // 렌더타겟 제거

// T00_Line에서 추가
extern HWND g_hWnd;  // 윈도우 핸들

	// T00_Line_03에서 추가
extern POINT g_Vtx[3];
extern POINT g_VtxTemp;

	//비동기 키보드 입력 처리 매크로
#define IsKeyDown(k) ((GetAsyncKeyState(k) & 0x8000) == 0x8000)
#define IsKeyUp(k) ((GetAsyncKeyState(k) & 0x8001) == 0x8001)

#ifndef SafeRelease
#define SafeRelease(p) if((p)){ (p)->Release(); (p) = NULL; }
#define SafeDelete(p) if((p)){ delete (p); (p) = NULL; }
#define SafeDelArray(p) if((p)){ delete[] (p); (p) = NULL; }
#endif


