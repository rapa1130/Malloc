#pragma warning(disable:4267 4996)
#include "Windows.h"
#include "tchar.h"
#include "stdio.h"
#include "Device.h"		
#include "Render.h"

TCHAR* g_WindowName = _T("SW-Renderer : T05 Rasterization 01 Diffuse Color(GDI)");

LPYENAVERTEXBUFFER9 g_pVB = NULL;

struct COLVTX
{
	float x, y;			//2D 화면좌표.
	DWORD diff;			//정점색.(DWORD type)
};

#define FVF_COLVTX (B3YFVF_XY | B3YFVF_DIFFUSE)		//좌표 +_정점색.


int  ObjLoad	();
void ObjRelease	();
void ObjUpdate	(float dTime);
void ObjDraw	(float dTime);


BOOL g_bWireFrame = FALSE;		//와이어 프레임 출력 플래그.
BOOL g_bCulling   = FALSE;		//뒷면제거 플래그.


 


// 게임 데이터 및 렌더링 자원 로딩.
int DataLoading()
{   

	//추가 코드는 여기에...
	//...

	//오브젝트 로딩.
	ObjLoad(); 


	return TRUE;
}




//게임 데이터 및 렌더링 자원 해제.
void DataRelease()
{
	// 데이터 해제 코드는 여기에..
	// ...  

	//오브젝트 삭제 
	ObjRelease();
}

//  오브젝트 : 생성 및 필요 리소스 로딩.
int ObjLoad()
{
	//삼각형을 위한 3개의 정점 선언. : 정점의 좌표값은 화면(Screen)좌표.
	COLVTX	Vertices[] = {

		//Face 0 : 정삼각형.(CW) 
		// 좌표 (x, y)    색상( a, r, g, b)   a, 곧 Alpha 는 기본값 255 (1.0f)
		{  50.0f, 250.0f, 0xffff0000 },		//v0, Red.★  <-- 지정된 색상이 동일위치/색상으로 표현되어야 합니다.
		{ 150.0f,  50.0f, 0xff00ff00 },		//v1, Green.★ 
		{ 250.0f, 250.0f, 0xff00ffff },		//v2, Light-Blue.★ 

		//Face 1 : 역삼각형.(CCW) 
		{  50.0f, 250.0f, 0xffff0000 },
		{ 150.0f, 450.0f, 0xff00ff00 },
		{ 250.0f, 250.0f, 0xff00ffff },

		//Face 2: 빗각 삼각형 (CW) 테스트 삼각형 
		{  300.0f, 500.0f, 0xffff0000 },
		{  400.0f, 300.0f, 0xff00ff00 },
		{  480.0f, 430.0f, 0xff00ffff },

		//Face 3: 빗각 삼각형 (CCW) 테스트 삼각형 
		{  500.0f, 430.0f, 0xffff0000 },
		{  680.0f, 500.0f, 0xff00ff00 },
		{  600.0f, 300.0f, 0xff00ffff },

		//Face 4 : 직각 삼각2 (CW)
		{  40.0f,  30.0f, 0xffff0000 },
		{  90.0f,  30.0f, 0xff00ff00 },
		{  90.0f, 100.0f, 0xff00ffff },

		//Face 5 : 직각 삼각1 (CCW)
		{  10.0f,  30.0f, 0xffff0000 },
		{  10.0f, 100.0f, 0xff00ff00 },
		{  60.0f, 100.0f, 0xff00ffff },
		// */

	};


	// 정점 버퍼 생성.
	if (YN_FAILED(g_pDevice->CreateVertexBuffer(
							sizeof(Vertices),			// 정점 버퍼 크기 (바이트)
							0,							// 버퍼 사용방식 (기본 0)
							FVF_COLVTX,					// 정점 규격. 
							B3YPOOL_SYSTEMMEM,			// 시스템 메모리 사용. (현재는 이 옵션만 유효) 
							&g_pVB,						// 성공시 리턴되는 버퍼 운용 객체 인터페이스
							NULL						// 예약됨. NULL.
	)))
	{
		return YN_FALSE;
	}

	// 버퍼 주소 얻기
	// 실제 DX 의 그것과는 역활이 다르지만, 동작의 결과는 동일합니다.
	VOID* pBuff;
	if (YN_FAILED(g_pVB->Lock(0, sizeof(Vertices), (void**)&pBuff, 0)))
	{
		return YN_FALSE;
	}

	//버퍼에 정점 데이터 채우기.
	//<DX> CPU -> VB(GPU/VRAM) 정점 데이터 복사
	//<Yena> CPU -> VB(SYSMEM) 정점 데이터 복사.
	memcpy(pBuff, Vertices, sizeof(Vertices));

	//<DX> 작업 완료, 버퍼 사용이 끝나면 버퍼 잠금을 해제(Unlock) 해야합니다.
	//<Yena> 우리는 이 동작을 간략하게 처리하겠습니다.
	g_pVB->Unlock();


	//... 버퍼 생성 완료...
	//... 이제 적절한 때에 VB 를 장치에 등록(Binding) 하고 렌더링을 수행하면 되겠습니다.
	//... 생성된 VB 는 사용자가 제거해야 합니다. (Release 호출) 


	return YN_OK;

}




//오브젝트 제거 
void ObjRelease()
{
	//정점 버퍼 제거..
	SafeRelease(g_pVB);
}


//오브젝트 그리기
void ObjDraw()
{

	//정점 버퍼 등록(Binding) 
	g_pDevice->SetStreamSource(0, g_pVB, 0, sizeof(COLVTX));

	//정점 형식 설정.
	g_pDevice->SetFVF(FVF_COLVTX);

	//'기하도형' 렌더링 : 지금부터 모든 그리기가 시작됩니다...
	// DX 메소드와 (가능한)동일한 파이프라인을 구현하는 것이 목표입니다.
	g_pDevice->DrawPrimitive(B3YPT_TRIANGLELIST, 0, 6);		//Face 6개 그리기...

}



//엔진 및 시스템 상태 갱신.
void SystemUpdate()
{
	if (IsKeyUp(VK_SPACE))	g_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F5))		g_bCulling ^= TRUE;			

	g_pDevice->SetRenderState(B3YRS_FILLMODE, g_bWireFrame ? B3YFILL_WIREFRAME : B3YFILL_SOLID);		
	g_pDevice->SetRenderState(B3YRS_CULLMODE, g_bCulling ? B3YCULL_CCW : B3YCULL_NONE);	  //반시계방향 컬링 
	//g_pDevice->SetRenderState(B3YRS_CULLMODE,  g_bCulling? B3YCULL_CW : B3YCULL_NONE);  //시계방향 컬링 

}

void ShowInfo() 
{ 
	static bool bShow = true;
 	if(IsKeyUp(VK_F1)) bShow ^= true;
  
	if(!bShow) 
	{
		//DrawText(1,20, COLOR(1, 1, 0, 1), "[Help] F1"); 
		return;
	}


	// Today's Topic.
	{
		////int x = 350, y = 1;			
		//int x = g_Mode.Width / 2 - 100;
		//int y = 50;
		//COLORREF col  = RGB(255, 255, 255);
		//COLORREF col2 = RGB(255, 255, 0);
		//COLORREF col3 = RGB(150, 150, 0);
		//COLORREF col4 = RGB(150, 150, 150);
		//COLORREF col5 = RGB(200, 200, 200);
		//DrawText(x, y,  col, _T("■ %s"), g_WindowName);
		//DrawText(x, y += 14, col3, _T("1. 정점(Vertex) 구성"));
		//DrawText(x, y += 14, col3, _T("4. 삼각형(Face) 출력"));
		////DrawText(x, y += 14, col3, _T("2. 정점 규격(Vertex Format) 의 이해"));
		////DrawText(x, y += 14, col3, _T("3. 정점 버퍼(Vertex Buffer) 구축"));
		////DrawText(x, y += 14, col3, _T("4. 삼각형(Face) 출력 : Wireframe"));
		////DrawText(x, y += 14, col3, _T("5. 삼각형(Face) 출력 : Fill Mode"));
		//DrawText(x, y += 14, col3, _T("6. 렌더링 상태 구현 : SetRenderState"));
		////DrawText(x, y += 14, col3, _T("7. 채우기 모드(Fill Mode) 전환 : SPACE BAR"));
		////DrawText(x, y += 14, col2, _T("8. 컬링 모드(Culling Mode) 전환 : F5"));
		//DrawText(x, y += 14, col2, _T("9. 정점 색상(Vertex Color) 처리")); 

		//y+=14*2;
		//DrawText(x, y += 14, col2, _T("<과제 조건 #3>"));
		////DrawText(x, y += 14, col2, _T("* 정점 색상 형식 : DWORD"));
		//DrawText(x, y += 14, col2, _T("* 정점 색상 형식 : B3YXCOLOR + DWORD"));
		////DrawText(x, y += 14, col2, _T("* B3YXCOLOR 와 DWORD 타입의 호환성 향상 : 연산자를 재정의"));
		//DrawText(x, y += 14, col3, _T("* 정점 색상 출력"));
		//DrawText(x, y += 14, col3, _T("* 픽셀 색상 계산 : 선형보간(Linear Interpolation)"));
		//DrawText(x, y += 14, col3, _T("* 픽셀 파이프라인 개정 : 레스터(Rasterization) 개정"));

		//y += 14;
		//DrawText(x, y+=14, RGB(255, 150, 255), _T("<과제 제작용>"));
		//DrawText(x, y+=14, RGB(255, 150, 255), _T("■ _DrawLine (GDI)"));
		//DrawText(x, y+=14, RGB(255, 150, 255), _T("■ _DrawFace (GDI)"));
	}

	{
		int x = 1;
		int y = 100;
		COLORREF col = RGB(0, 255, 0);
		DWORD v;

		TCHAR* wmsg[] = { _T("N/A"), _T("POINT"), _T("WIRE"), _T("SOILD") };
		g_pDevice->GetRenderState(B3YRS_FILLMODE, &v);		
		DrawText(x, y += 14, col, _T("Fill: Space (%s)"), wmsg[v]);

		TCHAR* cmsg[] = { _T("N/A"), _T("NONE"), _T("CW"), _T("CCW") };
		g_pDevice->GetRenderState(B3YRS_CULLMODE, &v);						
		DrawText(x, y += 14, col, _T("Cull: F5 (%s)"), cmsg[v]);
	} 

}


void SceneRender()
{
	
	//-------------------------------
	// 엔진/시스템 갱신.
	//------------------------------- 
	//엔진 경과시간 얻기.
	//float dTime = GetEngineTime();

	//엔진 상태 갱신
	SystemUpdate();
	

	// 장면 갱신 
	// 기하 데이터: 주인공, 몬스터, 오브제 이동, 충돌검사 등등
	// 게임 로직 이벤트,층돌,점수계산..



	//-------------------------------
	// 장면 그리기 시작
	//-------------------------------
 	 
	//장면 그리기 준비
	g_pDevice->BeginScene();

		//렌더타겟(배경) 지우기..	
		g_pDevice->Clear(RGB(0, 30, 89));
		//g_pDevice->Clear(RGB(0, 0, 255));
	 
		//지형 오브제 그리기

		//오브제 그리기.
		ObjDraw();

		//도움말 및 기타 렌더링 정보 출력.
		PutFPS(1, 1);
		ShowInfo();

	//장면 그리기 종료
	g_pDevice->EndScene();

 
	//-------------------------------
	// 장면 그리기 종료
	// 1회의 게임 루프 종료
	//------------------------------- 
	g_pDevice->Present();
}