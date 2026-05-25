#include"Render.h"
#include<tchar.h>
#include<Windows.h>
#include"Device.h"
#include"bhError.h"

TCHAR* g_windowName = _T("BH");
IBHVertexBuffer9* g_vb = NULL;


struct COLVTX
{
	float x, y;
	DWORD diff;
};


#define FVF_COLVTX B3BFVF_XY | B3BFVF_DIFFUSE


int ObjLoad();
void ObjRelease();
void ObjUpdate(float dt);
void ObjDraw(float dt);

void SystemUpdate();



BOOL g_bWireFrame = false;
BOOL g_bCull = false;


int DataLoading()
{
	ObjLoad();
	return TRUE;
}

void DataRelease()
{
	ObjRelease();
}

void ShowInfo()
{
	static bool bShow = true;
	if (IsKeyUp(VK_F1)) {
		bShow ^= true;
	}
	if (!bShow) return;


	{
		int x = 300;
		int y = 20;
		
		COLORREF col = RGB(255, 255, 255);
		DrawText(x, y, col, _T("■ BH SW-Renderer 06 Rasterization"));

		col = RGB(150, 150, 150);
		DrawText(x, y+=25, col, _T("1.정점색을 추가합니다.(+FVF확장)."));
		DrawText(x, y+=15, col, _T("2.정점색을 출력합니다. : 선형보간(Linear Interpolation)"));
		DrawText(x, y+= 15, col, _T("3.지정된 정점 및 삼각형 6개를 HW 렌더링 결과와 동일하게 렌더링합니다."));
		DrawText(x, y+= 15, col, _T("4.픽셀 파이프라인(Pixel Pipleline) 을 확장합니다."));
		DrawText(x, y+= 15, col, _T("5.삼각형 내부를 지정 색상으로 채웁니다. (Filling)"));
		DrawText(x, y+= 15, col, _T("6.렌더링 상태를 구현합니다 : SetRenderState"));
		DrawText(x, y+= 15, col, _T("7.채우기 상태 (Fill Mode)를 구현합니다. : SPACE BAR"));
		DrawText(x, y+= 15, col, _T("8.뒷면 컬링 상태 (Back-Face Culling Mode) 를 구현합니다: F5"));

		col = RGB(255, 255, 255);
		DrawText(x, y+= 25, col, _T("9.픽셀 색상 출력 : 레스터 (Rasterization) 을 구현합니다."));
		DrawText(x, y+= 15, col, _T("10.선형보간(Linear Interpolation) 을 통해 픽셀값을 계산합니다."));
		DrawText(x, y+= 15, col, _T("11.B3YXCOLOR 확장 : DWORD 타입 호환성 향상, 연산자 재정의."));
	}

	{
		int x = 1;
		int y = 110;

		TCHAR* fillWords[] =
		{
			{_T("N/A")},
			{_T("POINT")},
			{_T("WIRE")},
			{_T("SOLID")}
		};

		TCHAR* cullWords[] =
		{
			{_T("N/A")},
			{_T("None")},
			{_T("CW")},
			{_T("CCW")}
		};

		DWORD state;
		g_pBHDevice->GetRenderState(B3BRS_FILLMODE, &state);
		DrawText(0, y, RGB(0, 255, 0), _T("Fill: Space (%s)"),fillWords[state]);
		g_pBHDevice->GetRenderState(B3BRS_CULLMODE, &state);
		DrawText(0, y+=17, RGB(0, 255, 0), _T("Cull: F5 (%s)"), cullWords[state]);
	}
}

void SceneRender()
{
	SystemUpdate();

	g_pBHDevice->BeginScene();
		g_pBHDevice->Clear(g_bWireFrame? RGB(50, 50, 50) : RGB(0, 0, 100));
		ObjDraw(0);
		PutFPS(1, 1);
		ShowInfo();
	g_pBHDevice->EndScene();
	g_pBHDevice->Present();
}

int ObjLoad()
{
	COLVTX Vertices[] =
	{

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

	if (BH_FAILED(g_pBHDevice->CreateVertexBuffer(
		sizeof(Vertices),
		B3BUSAGE_WRITEONLY,
		FVF_COLVTX,
		B3BPOOL_SYSTEMMEM,
		&g_vb,
		NULL
	)))
	{
		return BH_FAIL;
	}



	VOID* pBuff = NULL;
	if (BH_FAILED(g_vb->Lock(0, sizeof(Vertices), (void**)&pBuff, 0)))
	{
		return BH_FAIL;
	}

	memcpy(pBuff, Vertices, sizeof(Vertices));

	g_vb->Unlock();


	return BH_OK;
}

void ObjRelease()
{
	SafeRelease(g_vb);
}

void ObjUpdate(float dt)
{
}

void ObjDraw(float dt)
{
	g_pBHDevice->SetFVF(FVF_COLVTX);
	g_pBHDevice->SetStreamSource(0, g_vb, 0, sizeof(COLVTX));
	g_pBHDevice->DrawPrimitive(B3BPT_TRAINALGELIST, 0, 6);
}


void SystemUpdate()
{

	if (IsKeyUp(VK_SPACE)) g_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F5)) g_bCull ^= TRUE;

	g_pBHDevice->SetRenderState(
		B3BRS_FILLMODE,
		g_bWireFrame ? B3BFILL_WIREFRAME : B3BFILL_SOLID
	);
	g_pBHDevice->SetRenderState(
		B3BRS_CULLMODE,
		g_bCull ? B3BCULL_CCW : B3BCULL_NONE
	);

}