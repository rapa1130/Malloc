#include "Yena.h"
#include <cmath>

//각종 렌더링 '디바이스 Device' 들을 관리할 최상위 클래스
class B3Yena : public IYena					
{
protected:
	//레퍼런스 참조 카운트 
	ULONG m_ref = 0;

//protected:
public:
	B3Yena(void);
	virtual ~B3Yena(void);



	// 인터페이스 재정의
	// Yena SWR 렌더링 디바이스 개체 생성 : D3D9 대응
	virtual int CreateDevice(HWND hwnd,						
							YENAPRESENT_PARAMETERS* pp,		
							DWORD vp,						
							LPYENADEVICE9* pDev				
	);


	// 참조 카운트 메소드 재정의 
	virtual ULONG AddRef		(void);
	virtual ULONG Release		(void);
	virtual int   QueryInterface(YN_IID riid, _out_ void** ppvObject);
};


class B3YenaDevice9 : public IYenaDevice9			
{
	friend class B3Yena;

protected:
	//레퍼런스 참조 카운트 
	ULONG m_ref = 0;


protected:

	// 장치 운용 기본 정보
	HWND		 m_hWnd;
	YENAPRESENT_PARAMETERS m_PresentParam;
	DWORD		 m_VertexProcessing;
	//YENADISPLAYMODE  m_Display; 


	// 렌더타겟(Back-Buffer) 구성용 핸들
	HBITMAP  m_hBmpRT;
	HDC		 m_hSurfaceRT;
	COLORREF m_BkColor;

		
	// 정점 버퍼 관련 데이터 
	// 외부, 사용자 지정(장치에 등록), 렌더링시 변경 가능
	//IYenaVertexBuffer9*	m_pVB[16];	
	IYenaVertexBuffer9* 	m_pVB[1];	
	DWORD m_FVF;						
	UINT  m_Stride;						
	

	// 렌더링 옵션 
	UINT  m_PrimCnt;					//이번에 그려질 Face 개수.
	UINT  m_StartVtx;					//이번에 그려질 (시작) 정점 번호.
	UINT  m_VtxNum;						//현재 작업중인 정점 번호.(디버깅용)
	UINT  m_FaceNum;					//현재 작업중인 페이스 번호.(디버깅용)


	// 렌더링 상태값 
	DWORD  m_RState[B3YRS_MAX_];


protected:
	// 내부 메소드
	int  _RenderTargetCreate	();
	void _RenderTargetRelease	();

	// 렌더링 파이프 라인 별 연산 메소드.
	int	_VertexPipeLine			();		//정점 파이프 라인.
	int _GeometryPipeLine		();		//기하 파이프 라인.	
	int _PixelPipeLine			();		//픽셀 파이프 라인.	

	// 그리기 함수.
	//int _DrawLine	(B3YVECTOR2& start, B3YVECTOR2& end);
	int _DrawLine	(B3YXVECTOR2 v0, B3YXVECTOR2 v1);
	//int _DrawLine	(B3YXVECTOR2 v0, B3YXVECTOR2 v1, DWORD c0, DWORD c1);			//+정점색(DWORD 타입) -> 제거. B3YXCOLOR 로 통합.★
	int _DrawLine	(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXCOLOR c0, B3YXCOLOR c1);	//정점색 버전(B3YXCOLOR 타입)★

	//int _DrawLine(B3YVECTOR2& v0, B3YVECTOR2& v1);	
	int _DrawFace	(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2);
	//int _DrawFace	(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2, DWORD c0, DWORD c1, DWORD c2);				//+정점색(DWORD 타입) -> 제거. B3YXCOLOR 로 통합.★
	int _DrawFace	(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2, B3YXCOLOR c0, B3YXCOLOR c1, B3YXCOLOR c2);	//정점색 (B3YXCOLOR 타입)★


	// 렌더링 상태 함수.
	void _InitRenderState	();
	bool _FaceCulling		(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2);		//페이스 컬링. false 리턴시, 그리기 동작 없음.



public:
	B3YenaDevice9(void);
	virtual ~B3YenaDevice9(void);
	
	
	//-----------------------------------------------------------------
	// 인터페이스 재정의
	//-----------------------------------------------------------------
	// 
	// 
	// 
	// 
	//--------------------------------
	// 스왑체인 및 렌더타겟 운용 메소드들
	//--------------------------------
	virtual int BeginScene	();
	virtual int EndScene	();
	virtual int Clear		(COLORREF col);
	virtual int Present		();

	//--------------------------------
	// 기하 버퍼 및 렌더링 메소드들 
	//--------------------------------
	virtual int CreateVertexBuffer	(UINT Length, DWORD Usage, DWORD FVF, B3YENAPOOL Pool, _out_ IYenaVertexBuffer9** ppVB, _in_opt_ HANDLE* pSharedHandle);
	virtual int SetStreamSource		(UINT SteamNumber, IYenaVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride);
	virtual int SetFVF				(DWORD FVF);
	virtual int DrawPrimitive		(B3YPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	
	//--------------------------------
	// 렌더링 상태 조절 메소드들
	//--------------------------------
	virtual int SetRenderState	(B3YRENDERSTATETYPE	State, DWORD Value);
	virtual int GetRenderState	(B3YRENDERSTATETYPE State, DWORD* pValue);

		
	//--------------------------------
	// 멤버데이터 접근자 Accessors : Yena 전용
	//--------------------------------
	virtual HDC		 GetRT		();
	virtual COLORREF GetBkColor	();


	//--------------------------------
	// 참조 카운트 메소드 재정의 
	//--------------------------------
	virtual ULONG AddRef		(void);
	virtual ULONG Release		(void);
	virtual int   QueryInterface(YN_IID riid, _out_ void** ppvObject);

};


#define CHECKSTATE( state, val ) (m_RState[(state)] == (val))	




//B3YenaVertexBuffer 정점 버퍼.관리 클래스
class B3YenaVertexBuffer9 : public IYenaVertexBuffer9		
{
	friend class B3YenaDevice9;

protected:
	//레퍼런스 참조 카운트 
	ULONG m_ref = 0;

protected:
	void* m_pVBuffer;				//정점 버퍼 : 실제 데이터가 저장됨.
	DWORD m_SizeInByte;				//정점 버퍼 크기 (바이트)
	DWORD m_FVF;					//정점 버퍼 규격 조합 플래그.
	UINT  m_Stride;					//정점 버퍼 안의 1마디(정점 구조 하나) 의 크기.
	BOOL  m_bLocked;				//버퍼 잠금 상태 (On/Off)

	B3YVERTEXBUFFER_DESC m_Desc;	//정점 버퍼 정보 

protected:
	//(내부) 버퍼 운용 함수들 
	int		_Create			(B3YVERTEXBUFFER_DESC desc);
	int		_Create			(UINT Length, DWORD FVF, B3YPOOL Pool);		//구형 호환.
	
	void*	_GetVBuffer		(){ return m_pVBuffer;	}
	DWORD	_GetSizeInByte	(){ return m_SizeInByte;}
	DWORD	_GetFVF			(){ return m_FVF;		}	
	DWORD	_GetStride		(){ return m_Stride;	}	

public:
	B3YenaVertexBuffer9(void);
	virtual ~B3YenaVertexBuffer9(void);


	//-----------------------------------------------------------------
	// 인터페이스 재정의
	//-----------------------------------------------------------------
	virtual int Lock	(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
	virtual int Unlock	(void);
	virtual int GetDesc	( _out_ B3YVERTEXBUFFER_DESC* pDesc);

	//자원 버퍼 획득 : Yena 전용
	virtual void* GetBuffer		(void);
	virtual UINT  GetVertexCount(void);
	virtual int	  GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData);

	//--------------------------------
	// 참조 카운트 메소드 재정의 
	//--------------------------------
	virtual ULONG AddRef		(void);
	virtual ULONG Release		(void);
	virtual int   QueryInterface(YN_IID riid, _out_ void** ppvObject);


};

typedef B3YenaVertexBuffer9* LPB3YENAVERTEXBUFFER9;

IYena* YenaCreate9(DWORD ver)
{
	//COM 객체 생성.
	B3Yena* pB3Yena = new B3Yena;
	assert(pB3Yena != NULL);

	/*//인터페이스 생성 "RTTI" 
	//QueryInterface 등의 메소드 사용이 기본
	IYena* pYena = dynamic_cast<IYena*>(pB3Yena);
	assert(pYena != NULL);
	//객체 참조카운트 증가.
	pB3Yena->AddRef();					
	*/

	//인터페이스 질의 및 획득.(GUID 및 Query) 
	//GUID 와 Query 를 사용하는 것이 보다 DX/COM 표준 운용에 가깝습니다.
	//획득한 인터페이스는 사용후 Release 를 호출해야 합니다. (안전한 메모리 해제)
	IYena* pYena = nullptr;
	if (YN_FAILED(pB3Yena->QueryInterface(IID_IYena, (void**)&pYena)))
	{

	}

	return pYena;
}




B3Yena::B3Yena(void)
{
#ifdef _DEBUG
	ynLog(_T("B3Yena 생성됨..."));
#endif
}


B3Yena::~B3Yena(void)
{
#ifdef _DEBUG
	ynLog(_T("B3Yena 제거됨..."));
#endif
}


int B3Yena::CreateDevice(HWND hwnd, 
						 YENAPRESENT_PARAMETERS* pp, 
						 DWORD vp, 
						 LPYENADEVICE9* ppDevice
						 )
{
	//입력 정보 확인 : 오류별로 리턴값을 다르게 하는 것을 추천
	if(hwnd == NULL) return YN_FAIL;
	if(pp == NULL) return YN_FAIL;
	if(ppDevice == NULL) return YN_FAIL;


	//디바이스 COM 객체 생성
	B3YenaDevice9* pB3Dev = new B3YenaDevice9;
	assert(pB3Dev != NULL);

	//디바이스 초기화
	pB3Dev->m_hWnd = hwnd;
	pB3Dev->m_PresentParam = *pp;
	pB3Dev->m_VertexProcessing = vp;
	pB3Dev->_RenderTargetCreate();			//렌더타겟(백버퍼) 생성


	/*//인터페이스 생성 "RTTI" 
	//QueryInterface 등의 메소드 사용이 기본
	IYenaDevice9* pDev = dynamic_cast<IYenaDevice9*>(pB3Dev);
	assert(pDev != NULL);
	//객체 참조카운트 증가.
	pB3Dev->AddRef();
	*/

	//인터페이스 질의 및 획득.(GUID 및 Query) 
	//GUID 와 Query 를 사용하는 것이 보다 DX/COM 표준 운용에 가까움
	//획득한 인터페이스는 사용후 Release 를 호출해야 한다. 안전한 메모리 해제
	IYenaDevice9* pDev = nullptr;
	if (YN_FAILED(pB3Dev->QueryInterface(IID_IYenaDevice9, (void**)&pDev)))
	{

	}


	*ppDevice = pDev;

	return YN_OK;  
}




ULONG B3Yena::AddRef(void)
{ 
	 return ++m_ref;
};



ULONG B3Yena::Release(void)
{
	if (--m_ref <= 0) delete this; 

	return m_ref;
};



int B3Yena::QueryInterface(YN_IID riid, _out_ void** ppvObject)
{
	//void* pInterface = nullptr;

	//지정 ID 의 유효성 검사
	//if(riid == IID_IYena)
	//if (IsEqualGUID((GUID&)riid, (GUID&)IID_IYena))	//GUID 동일 검사
	//if (IsEqualIID((IID&)riid, (IID&)IID_IYena))		//IID 동일 검사 <상동>
	if (ynIsEqualIID(riid, IID_IYena))					//IID 동일 검사 <상동>
	{
		AddRef();										//참조 카운트 증가
		*ppvObject = dynamic_cast<IYena*>(this);		//지정 인터페이스 리턴
	}

	return YN_OK;
}


B3YenaDevice9::B3YenaDevice9(void)
{
	m_hWnd = NULL;
	ZeroMemory(&m_PresentParam, sizeof(m_PresentParam));

	m_hBmpRT = NULL;				
	m_hSurfaceRT = NULL;		 
	m_BkColor = RGB(0, 0, 255);	

	m_pVB[0] = NULL;	 
	m_FVF    = 0;			 
	m_Stride = 0;		 

	m_PrimCnt  = 0;		
	m_StartVtx = 0;		
	m_VtxNum   = 0;
	m_FaceNum  = 0;

	//렌더상태 초기화
	_InitRenderState();

#ifdef _DEBUG
	ynLog(_T("B3YenaDevice9 생성됨..."));
#endif

}


B3YenaDevice9::~B3YenaDevice9(void)
{ 
	_RenderTargetRelease();	

#ifdef _DEBUG
	ynLog(_T("B3YenaDevice9 제거됨..."));
#endif
}



HDC B3YenaDevice9::GetRT()
{ 
	return m_hSurfaceRT; 
}

COLORREF B3YenaDevice9::GetBkColor()
{ 
	return m_BkColor; 
}




///////////////////////////////////////////////////////////////////////////////
//
//! 백버퍼용 렌더타겟을 생성.
//!
//! <Yena> DX IDirect3DSwapChain9 인터페이스를 통해 스왑체인을 구축하는 등의 
//! 저수준 작업이 필요하지만 가능한 간략히 표현합니다. 
//! 더블 버퍼링을 통한 깜박임을 없애는 목적과 결과는 동일합니다.
//! [참고] IDirect3DSwapChain9 (https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nn-d3d9-idirect3dswapchain9)
//! 
int B3YenaDevice9::_RenderTargetCreate()
{
	HDC hdc = GetDC(m_hWnd);

	m_hSurfaceRT = CreateCompatibleDC(hdc);
	m_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, m_PresentParam.Width, m_PresentParam.Height);
	SelectObject(m_hSurfaceRT, m_hBmpRT);

	ReleaseDC(m_hWnd, hdc);

	return YN_OK;  
}



///////////////////////////////////////////////////////////////////////////////
// 
//! 렌더타겟 제거.
//
void B3YenaDevice9::_RenderTargetRelease()
{  	
	DeleteObject(m_hBmpRT);
	DeleteDC(m_hSurfaceRT); 
}




///////////////////////////////////////////////////////////////////////////////
//
//! 장면 그리기 시작 : 렌더링에 필요한 (디바이스의) 선위 작업을 수행.
//
int B3YenaDevice9::BeginScene()
{ 
	//폰트 배경색 기본값 
	//SetBkColor(m_hSurfaceRT, m_BkColor);
	SetBkMode(m_hSurfaceRT, TRANSPARENT);	

	//기본 펜 색상 .
	HPEN  hPen = (HPEN)GetStockObject(WHITE_PEN);
	SelectObject(m_hSurfaceRT, hPen);

	return YN_OK;
}



///////////////////////////////////////////////////////////////////////////////
//
//! 장면 그리기 종료 : 렌더링 종료에 필요한 (디바이스의) 후위 작업을 수행.
//
int B3YenaDevice9::EndScene()
{
	//...

	return YN_OK;  
}




///////////////////////////////////////////////////////////////////////////////
//
//! 렌더타겟 클리어.
//!
//! \param  col	RT 을 지울 지정색.
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::Clear(COLORREF col)
{ 	
	HBRUSH hBrush = CreateSolidBrush(col); 
	RECT rc = { 0, 0, (LONG)m_PresentParam.Width,  (LONG)m_PresentParam.Height};
	FillRect(m_hSurfaceRT, &rc, hBrush); 
	DeleteObject(hBrush);

	m_BkColor = col;

	return YN_OK;  
}




///////////////////////////////////////////////////////////////////////////////
//
//! RT 의 내용(렌더링된 장면)을 Front Buffer 에 출력합니다. "Flipping", "Swapping"
//!
//! \param   없음
//! \return  오류시 YN_OK 이외의 값.
//
int B3YenaDevice9::Present() 
{
	HDC hdc = GetDC(m_hWnd);
	BitBlt(hdc, 0, 0, m_PresentParam.Width, m_PresentParam.Height, m_hSurfaceRT, 0, 0, SRCCOPY); 
	ReleaseDC(m_hWnd, hdc); 

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! CreateVertexBuffer : 정점 버퍼 생성.
//! 
//! \param	Length			정점 버퍼의 전체 크기 (바이트)	
//! \param	Usage			버퍼 용도 (미지정은 0)
//! \param	FVF				정점 규격
//! \param	Pool			버퍼-메모리 형식 : 시스템메모리 사용. (Yena 유일 옵션) 
//! \param	ppVertexBuffer	성공시 리턴되는 정점 버퍼 인터페이스 포인터
//! \param	pSharedHandle	공유 헨들 (기본 NULL)
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::CreateVertexBuffer( UINT Length, 
									   DWORD Usage,
									   DWORD FVF, 
									   B3YENAPOOL Pool, 
									   _out_ IYenaVertexBuffer9** ppVertexBuffer,
									   _in_opt_ HANDLE* pSharedHandle
									  )
{

	//정점 버퍼 운용 개체 생성.
	LPB3YENAVERTEXBUFFER9 pB3VB = new B3YenaVertexBuffer9;		
	if(YN_INVALIED(pB3VB)) return YN_FAIL;
	 
	//생성할 정점 버퍼 정보 기술
	//<Yena> 시스템 메모리를 사용하므로 필수 동작은 아니지만, 구조의 일관성을 위해 사용.
	//GPU 메모리(VRAM) 확보를 위해서 이 정도의 수고는 필요하다...정도로 이해해 봅시다.
	B3YVERTEXBUFFER_DESC desc = {};
	desc.Format = B3YFMT_VERTEXDATA;		//버퍼 포멧 : 정점 데이터용
	desc.Type	= B3YRTYPE_VERTEXBUFFER;	//버펴 형식 : 정점 버퍼
	desc.Usage	= B3YUSAGE_WRITEONLY;		//버퍼 사용방식 : 읽기 전용, <주> 정점버퍼는 CPU "쓰기전용", GPU "읽기 전용" 임을 상기합시다.
	desc.Pool	= Pool;						//버퍼 메모리 풀 형식
	desc.Size	= Length;					//버퍼 크기
	desc.FVF	= FVF;						//버퍼에 저장될 정점 규격
	
	//정점 버퍼 생성.
	if (YN_FAILED(pB3VB->_Create(desc)))	
	{
		//Error...
		return YN_FAIL;
	}

	/* //정점 버퍼 생성 (구형)
	if (YN_FAILED(pB3VB->_Create(Length, FVF, Pool)))	 
	{
		//Error...
		return YN_FAIL;
	}*/


	/*//인터페이스 생성 "RTTI" 
	//QueryInterface 등의 메소드 사용이 기본이지만, Yena 에서는 직접 형변환 하겠습니다.
	//이쪽이 COM Interface 의 구조와 동작의 이해에 도움이 될 것입니다.
	IYenaVertexBuffer9* pVB = dynamic_cast<IYenaVertexBuffer9*>(pB3VB);
	assert(pVB != NULL);
	//객체 참조카운트 증가.
	pVB->AddRef();
	*/

	//인터페이스 질의 및 획득.(GUID 및 Query) 
	//GUID 와 Query 를 사용하는 것이 보다 DX/COM 표준 운용에 가깝습니다.
	//획득한 인터페이스는 사용후 Release 를 호출해야 합니다. (안전한 메모리 해제)
	IYenaVertexBuffer9* pVB = nullptr;
	if (YN_FAILED(pB3VB->QueryInterface(IID_IYenaVertexBuffer9, (void**)&pVB)))
	{
		//...ERROR...
	}

	//외부로 리턴..
	*ppVertexBuffer = pVB;

	return YN_OK;  
}



/*
////////////////////////////////////////////////////////////////////////////////
//
//! SetVertexBuffer : 정점 버퍼를 디바이스에 등록.
//! 
//! \param	pVB		렌더링 할 정점 버퍼 포인터.
//! \param	Stride	렌더링 할 버퍼의 1마디(정점 데이터) 크기
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::SetVertexBuffer( IYenaVertexBuffer9* pVB, UINT Stride )
{
	if(YN_INVALIED(pVB)) return YN_FAIL;

	m_pVB = pVB;
	m_Stride = Stride;			

	return YN_OK;  
}
*/


////////////////////////////////////////////////////////////////////////////////
//
//! SetStreamSource : 정점 버퍼를 디바이스에 등록.
//! 
//! \param	StreamNumber	(기하, 정점)스트리밍 색인 (Yena 기본값은 0)
//! \param	pVB				렌더링 할 정점 버퍼 포인터.
//! \param	OffsetBytes		(기하,정점) 스트리밍 시작 색인 (Yena 기본값은 0)
//! \param	Stride			렌더링 할 버퍼의 1마디(정점 데이터) 크기
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::SetStreamSource(UINT StreamNumber, IYenaVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride)
{
	if (YN_INVALIED(pVB)) return YN_FAIL;

	// <DX> 복수의 정점 버퍼로 동시 '기하 스트리밍'을 지원합니다. (DX9, 최대 16)
	// 또는 지정 VB 내의 데이터중 부분 렌더링(OffsetBytes)도 가능합니다.
	// m_pVB[StreamNumber] = pVB;
	// 
	// <Yena> 1개의 정점 버퍼만 사용합니다.
	m_pVB[0] = pVB;
	m_Stride = Stride;

	// 이하 생략..
	// ... = StreamNumber
	// ... = OffsetBytes

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 정점 규격을 설정합니다. 
//! 렌더링시 SetStreamSource 로 등록된 또는 등록할 버퍼 내 정점과 동일 규격을 명시해야 합니다.
//
int B3YenaDevice9::SetFVF(DWORD fvf)
{	
	m_FVF = fvf;

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! DrawPrimitive : 기하 데이터 렌더링 
//! 
//! 이 메소드의 호출 이전에 정점버퍼, FVF, 마디 크기(Stride) 등 렌더링에 필요한
//! 모든 정보가 디바이스에 등록되어 있어야 합니다.
//! 
//! \param	PrimitiveType	렌더링 기하 타입.
//! \param	StartVertex		렌더링 정점 시작번호 (정점 버퍼 안)
//! \param	PrimitiveCount	렌더링 기하 개수.
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::DrawPrimitive( B3YPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount ) 
{
	if(YN_INVALIED(m_pVB)) return YN_FAIL;
	//if(YN_INVALIED(m_Stride)) return YN_FAIL;

	//렌더링 기초 옵션 저장.
	m_PrimCnt = PrimitiveCount;	 
	m_StartVtx = StartVertex;	 


	//-------------------------------------------------
	// 각 렌더링 파이프라인 단계별 적절한 연산이 수행되며
	// Yena 는 각 기능을 가능한 상세하게 구현하겠습니다.
	//-------------------------------------------------
	//1.정점 파이프 라인...
	_VertexPipeLine();

	//2.기하 파이프 라인...
	_GeometryPipeLine();
	
	//3.픽셀 파이프 라인...
	_PixelPipeLine();
	
	//4.최종 출력...
	//...

	return YN_OK; 
}




///////////////////////////////////////////////////////////////////////////////
//
//! 정점 파이프 라인 연산을 수행 : 출력결과는 GP 으로 넘어감니다.
//
int B3YenaDevice9::_VertexPipeLine()		
{
	
	//이번 데모는 이미 2D 좌표를 가지고 있으므로
	//추가 변환이 필요없고, 바로 화면에 출력합니다.
	//...

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 기하 파이프 라인 연산을 수행 : 출력결과는 PP 으로 넘어감니다.
//
int B3YenaDevice9::_GeometryPipeLine()		
{

	//이번 데모는 이미 2D 좌표를 가지고 있으므로
	//추가 변환이 필요없고, 바로 화면에 출력합니다.
	//...
 
	return YN_OK;
}





///////////////////////////////////////////////////////////////////////////////
//
//! 픽셀 파이프 라인 연산을 수행 : 출력결과는 RT 에 픽셀로 기록됩니다.
//
int B3YenaDevice9::_PixelPipeLine()		
{

	//이번 데모는 이미 2D 좌표를 가지고 있으므로
	//추가 변환이 필요없고, 바로 화면에 출력합니다.
	//...

	//---------------------------------------------
	// 기하(정점) 버퍼 접근, 정점 처리 "스트리밍"
	//---------------------------------------------
	// <DX9> 다중 버퍼 동시 '스트리밍' 지원 (max.16)
	// <Yena> 1개의 정점 버퍼만 사용합니다.
	B3YVERTEXBUFFER_DESC desc;							
	m_pVB[0]->GetDesc(&desc);							//(장치에 등록된) 정점 버퍼 정보 획득..
	UINT  vtxCnt = desc.Size / m_Stride;				//(장치에 등록된) 정점 개수 확인.
	void* pOrgVB = m_pVB[0]->GetBuffer();				//(장치에 등록된) 원본 버퍼 접근.

 
	//---------------------------------------------
	// 현제 데모는 Triangle-List 만 처리합니다.
	// Triangle-List 은 Face 마다 3개의 정점을 사용합니다.  
	//---------------------------------------------
	BYTE* pCurrVB  = (BYTE*)pOrgVB;						//현재 처리 중인 정점버퍼 주소. 
	BYTE* pCurrVtx = NULL;								//현재 처리 중인 정점의 주소.
	UINT  faceCnt  = 0;
	m_FaceNum = -1;

	//for(int i=0; i<vtxCnt; i+=3)						//3개씩 처리..(전부 다 그리기..)
	for(UINT i=m_StartVtx; i<vtxCnt; i+=3)				//지정 정점부터, 그리기.
	{
		//m_VtxNum = i;
		m_FaceNum++;


		//--------------------------------------------------------------- 
		// 외부에서 설정된 정점버퍼에서 정점 데이터를 구합니다. 
		// 단 정점 버퍼의 FVF 규격을 '믿고' 정점별 정보를 구합니다.  
		// GPU (개발자) 입장에서는 FVF 대로 POS, COLOR 가 들어온다..믿을 수 밖에 없겠지요★
		// 순서가 틀리면, 오류가 한가득...
		//
		// 정확한 데이터 획득을 위해서는  FVF 검사가 필요하지만 
		// 이번 데모는 간결한 진행을 위해 이를 생략합니다. 
		//---------------------------------------------------------------- 
		//----------------- 
		// 기하 정보 획득 : 정점 버퍼 읽기.
		//----------------- 
		//현재 기하데이터(삼각형) 포인터 얻기...
		pCurrVB = (BYTE*)pOrgVB + (i)*m_Stride;

		//첫번째 정점 
 		pCurrVtx = pCurrVB;			
  		B3YVECTOR2  v0 = *(B3YVECTOR2*)pCurrVtx;		//좌표 얻기.
		DWORD c0 = *(DWORD*)(pCurrVtx + sizeof(B3YVECTOR2));		//<--- 색상 얻기. 버퍼에서 실제 색상 읽기★


		//2번째 정점..
		pCurrVtx = pCurrVB + m_Stride;					//1개 크기만큼 이동. 
 		B3YVECTOR2  v1 = *(B3YVECTOR2*)pCurrVtx;		//좌표 얻기.
		DWORD c1 = *(DWORD*)(pCurrVtx + sizeof(B3YVECTOR2));		//<--- 색상 얻기. 버퍼에서 실제 색상 읽기★


		//3번째 정점..
		pCurrVtx = pCurrVB + (2)*m_Stride;				//1개 크기만큼 이동. 
 		B3YVECTOR2  v2 = *(B3YVECTOR2*)pCurrVtx;		//좌표 얻기.
		DWORD c2 = *(DWORD*)(pCurrVtx + sizeof(B3YVECTOR2));		//<--- 색상 얻기. 버퍼에서 실제 색상 읽기★
		//--------------------------------------------------------------- 
		// n번째 삼각형의 정보를 모두 구했습니다.
		//
		// 삼각형의 가시성(Visualization) 을 판정을 통해 보이지 않는
		// 삼각형들을 렌더링 파이프라인에서 제외시키면 전반적인 
		// 렌더링 속도 향상을 이끌어 낼 수 있습니다.
		// 
		// 이러한 동작은 대부분 Geometry Pipeline 에서 처리되지만 
		// 일단 이 곳에서 처리해 봅시다.
		//---------------------------------------------------------------  

		// 컬링 체크 : 지오메트리 파이프라인에서 해야 하지만, 
		//             일단 여기서...추후 이동..
		if (_FaceCulling(v0, v1, v2))
		{
			//true 가 리턴되면, 컬링해야 하는 삼각형으로 판정, 다음 삼각형으로 넘어간다.
			//....

			//지정 개수 이상의 삼각형이 그려지면, 작업 종료... 			
			if (++faceCnt >= m_PrimCnt)
				break;
			else
				continue;
		}


		//--------------------------------------------------------------- 
		// n번째 삼각형의 정보를 모두 구했으니, 삼각형을 그려봅시다. 
		//
		// 정점색을 보간하여, 완전한 레스터(Rasterization) 구현이
		// 목표입니다.
		//--------------------------------------------------------------- 

		//기하 도형 그리기..(렌더링 상태 적용)
		//
		if (m_RState[B3YRS_FILLMODE] == B3YFILL_WIREFRAME)	
		{
			//라인 그리기 : 정점색 추가.(과제) ★★★
			//정점색 추가.(B3YXCOLOR 타입으로 처리해 봅시다)★
			//VB 내부의 색상(DWORD 타입)이, B3YXCOLOR 타입으로 
			//(자동,암묵적) 형변환되도록 연산자를 오버로딩합시다.★
			//코드의 가독성 증가, 생산성 증대, 등등...의 목적입니다..★
			_DrawLine(v0, v1, c0, c1); 		//정점색 추가.★
			_DrawLine(v0, v2, c0, c2);  	//정점색 추가.★	
			_DrawLine(v1, v2, c1, c2);		//정점색 추가.★
		}
		else
		{
			//삼각형 그리기 : 정점색 추가.(과제)★★★
			_DrawFace(v0, v1, v2, c0, c1, c2);
		}


		//지정 개수 이상의 삼각형이 그려지면, 작업 종료... 
		if(++faceCnt >= m_PrimCnt )	
			break; 
	}


	return YN_OK;
}





///////////////////////////////////////////////////////////////////////////////
//
//! 라인 그리기. (ver.GDI) ★
//
int B3YenaDevice9::_DrawLine(B3YXVECTOR2 v0, B3YXVECTOR2 v1)
{  
	//...과제...★
	//GDI 대신 사용자 그리기 함수로 처리합니다.
	//SetPixel 은 사용가능합니다. 색상은 자유.
	//...
	//MoveToEx(m_hSurfaceRT, (int)v0.x, (int)v0.y, NULL);
	//LineTo(m_hSurfaceRT, (int)v1.x, (int)v1.y);





	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 삼각형 그리기 (ver.GDI) ★
//
int B3YenaDevice9::_DrawFace(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2)
{
	//...과제...★
	//GDI 대신 사용자 그리기 함수로 처리합니다.
	//SetPixel 은 사용가능합니다. 색상은 자유.
	//...
	POINT pt[3] = { v0, v1, v2 };	//연산자 오버로딩.
	Polygon(m_hSurfaceRT, pt, 3);

	return YN_OK;
}



/*
///////////////////////////////////////////////////////////////////////////////
//
//! 라인 그리기. (ver.User)  : 직접 그리기.[2] (세로방향기준)+정점색.(과제)★
//!
//! <주> 정점 버퍼 내의 색상은 DWORD 형임에 주의. ★
//
int B3YenaDevice9::_DrawLine(B3YXVECTOR2 v0, B3YXVECTOR2 v1, 
							 DWORD c0, DWORD c1		//정점색 버전(DWORD 타입)★
)
{

	//<과제> ★★★
	// 정점 사이의 간격만큼 각 정점색상을 보간하여 라인색을 출력합니다. 
	// ... 
	//
	// 함수 시그니쳐(Signature : 인자/리턴값/이름) 의 변경없이,
	// 함수의 Body 를 완성 하십시요.★
	// ...

	//임시 그리기..(삭제후 과제 수행)★
	_DrawLine(v0, v1);


	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! \bstar 삼각형 그리기 : 정점색 추가.(DWORD 타입)(과제) ★★★
//! 
//! \warning <주> 정점 버퍼 내의 색상은 DWORD 형임에 주의. ★
//!
//! \todo	GDI 를 사용하지 않고 이 함수를 완성하십시오. 단, Setpixel 은 사용가능.
//! 
int B3YenaDevice9::_DrawFace(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2,
							 DWORD c0, DWORD c1, DWORD c2		//정점색 추가.(DWORD 타입)★
)
{
	//<과제> ★★★
	// 정점 3개와 색상 3개를 이용해 삼각형에 색상을 채웁니다.★
	// ...
	//
	// 함수 시그니쳐(Signature : 인자/리턴값/이름) 의 변경없이,
	// 함수의 Body 를 완성 하십시요.★
	// ...

	// <GDI 버전> 임시 그리기.. (삭제후 과제 수행)★
	// ...
	POINT pt[3] = { v0, v1, v2 };	//연산자 오버로딩.
	Polygon(m_hSurfaceRT, pt, 3);

	return YN_OK;
}
*/





///////////////////////////////////////////////////////////////////////////////
//
//! 라인 그리기. (ver.User)  : 직접 그리기.[2] (세로방향기준)+정점색.(과제)★
//!
//! <주> 정점 버퍼 내의 색상은 여전히 DWORD 형임에 주의, 클래스 연산자 오버로딩으로 호환성 유지.★
//
int B3YenaDevice9::_DrawLine(B3YXVECTOR2 v0, B3YXVECTOR2 v1,
							 B3YXCOLOR c0, B3YXCOLOR c1		//정점색 버전(B3YXCOLOR 타입)★
)
{
	POINT pt[2] = { v0, v1 };	//연산자 오버로딩.

	int x0 = pt[0].x, y0 = pt[0].y;
	int x1 = pt[1].x, y1 = pt[1].y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	// 같은 점이면 리턴
	if (dy == 0 && dx == 0) return -1;

	if (dx >= dy)
	{
		// 수평에 가까운 선은 x를 주축으로 => y가 1증가할때마다 가로로  2픽셀 이상 찍어야 할때

		// 작은 X 좌표를 x0로 오게 정렬
		if (x0 > x1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		// x가 하나 증가할때마다 y는 얼만큼 증가해야 하는지 계산
		float invSlope = (dx == 0) ? 0.0f : (float)(y1 - y0) / (float)(x1 - x0);
		float curY = (float)y0;

		for (int x = x0; x <= x1; x++)
		{
			SetPixel(m_hSurfaceRT, x, (int)std::floor(curY), RGB(0, 255, 0));
			curY += invSlope;
		}
	}
	else
	{
		// 수직에 가까운 선은 y를 주축으로 => x가 1증가할때마다 세로로 2픽셀 이상 찍어야 할때

		// 작은 Y 좌표를 y0로 오게 정렬
		if (y0 > y1)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		// y가 하나 증가할때마다 x는 얼만큼 증가해야 하는지 계산
		float invSlope = (dy == 0) ? 0.0f : (float)(x1 - x0) / (float)(y1 - y0);
		float curX = (float)x0;

		for (int y = y0; y <= y1; y++)
		{
			SetPixel(m_hSurfaceRT, (int)std::floor(curX), y, RGB(0, 255, 0));
			curX += invSlope;
		}
	}

	return YN_OK;
}

void SortPoints(std::vector<POINT>& pts)
{
	int n = (int)pts.size();
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1 - i; j++)
		{
			// y좌표 비교 (Win32 좌표계는 아래로 갈수록 y가 큼)
			if (pts[j].y > pts[j + 1].y)
			{
				// POINT 구조체 값 교환
				POINT temp = pts[j];
				pts[j] = pts[j + 1];
				pts[j + 1] = temp;
			}
		}
	}
}

float CalculateInverseSlope(POINT p1, POINT p2)
{
	float dtx = (float)p2.x - p1.x;
	float dty = (float)p2.y - p1.y;

	if (dty == 0) return 0.0f;
	return dtx / dty;          // y가 1 변할 때 x의 변화량
}

///////////////////////////////////////////////////////////////////////////////
//
//! 삼각형 그리기 : 정점색 추가.(B3YXCOLOR 타입)(과제) ★★★
//! 
//! <주> 정점 버퍼 내의 색상은 여전히 DWORD 형임에 주의, 클래스 연산자 오버로딩으로 호환성 유지.★
//
int B3YenaDevice9::_DrawFace(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2,
							 B3YXCOLOR c0, B3YXCOLOR c1, B3YXCOLOR c2		//정점색 추가(B3YXCOLOR 타입)★
)
{
	struct Vertex
	{
		POINT pt;
		B3YXCOLOR color;
	};

	// 위치와 색상을 매칭하여 리스트 생성
	std::vector<Vertex> vertices = {
		{ {(int)v0.x, (int)v0.y}, c0 },
		{ {(int)v1.x, (int)v1.y}, c1 },
		{ {(int)v2.x, (int)v2.y}, c2 }
	};

	// 2. Y좌표 기준 오름차순 정렬
	std::sort(vertices.begin(), vertices.end(), [](const Vertex& a, const Vertex& b) {
		return a.pt.y < b.pt.y;
		});

	const Vertex& top = vertices[0];
	const Vertex& mid = vertices[1];
	const Vertex& bot = vertices[2];

	// 역기울기 계산
	float invSlope1 = CalculateInverseSlope(top.pt, mid.pt); // Top -> Mid
	float invSlope2 = CalculateInverseSlope(top.pt, bot.pt); // Top -> Bottom
	float invSlope3 = CalculateInverseSlope(mid.pt, bot.pt); // Mid -> Bottom

	float curX1 = (float)top.pt.x;
	float curX2 = (float)top.pt.x;

	// 상단 삼각형 렌더링 (Top -> Mid)
	for (int y = top.pt.y; y <= mid.pt.y; y++)
	{
		int startX = (int)std::floor(min(curX1, curX2));
		int endX = (int)std::floor(max(curX1, curX2));

		// Y축 변화율 (0.0f ~ 1.0f) 계산
		float tTopMid = (top.pt.y == mid.pt.y) ? 1.0f : (float)(y - top.pt.y) / (mid.pt.y - top.pt.y);
		float tTopBot = (top.pt.y == bot.pt.y) ? 1.0f : (float)(y - top.pt.y) / (bot.pt.y - top.pt.y);

		// [보간 1, 2] 각 변의 현재 Y축 위치에 주입될 정점 색상 구하기
		B3YXCOLOR colorEdge1, colorEdge2;
		ynLerp(&colorEdge1, top.color, mid.color, tTopMid);
		ynLerp(&colorEdge2, top.color, bot.color, tTopBot);

		// 왼쪽 경계선과 오른쪽 경계선의 색상 매칭
		B3YXCOLOR colorLeft = (curX1 < curX2) ? colorEdge1 : colorEdge2;
		B3YXCOLOR colorRight = (curX1 < curX2) ? colorEdge2 : colorEdge1;

		for (int x = startX; x <= endX; x++)
		{
			// X축 변화율 계산
			float tX = (startX == endX) ? 0.0f : (float)(x - startX) / (endX - startX);

			// [보간 3] 최종 픽셀 색상 보간
			B3YXCOLOR finalColor;
			ynLerp(&finalColor, colorLeft, colorRight, tX);

			// 0~1.0f의 변환 값을 DWORD로 변환
			SetPixel(m_hSurfaceRT, x, y, DWORD(finalColor));
		}

		curX1 += invSlope1;
		curX2 += invSlope2;
	}

	// 하단 삼각형 렌더링 (Mid -> Bottom)
	curX1 = (float)mid.pt.x;
	curX2 = (float)top.pt.x + invSlope2 * (float)(mid.pt.y - top.pt.y);

	for (int y = mid.pt.y; y <= bot.pt.y; y++)
	{
		int startX = (int)std::floor(min(curX1, curX2));
		int endX = (int)std::floor(max(curX1, curX2));

		float tMidBot = (mid.pt.y == bot.pt.y) ? 1.0f : (float)(y - mid.pt.y) / (bot.pt.y - mid.pt.y);
		float tTopBot = (top.pt.y == bot.pt.y) ? 1.0f : (float)(y - top.pt.y) / (bot.pt.y - top.pt.y);

		B3YXCOLOR colorEdge3, colorEdge2;
		ynLerp(&colorEdge3, mid.color, bot.color, tMidBot);
		ynLerp(&colorEdge2, top.color, bot.color, tTopBot);

		B3YXCOLOR colorLeft = (curX1 < curX2) ? colorEdge3 : colorEdge2;
		B3YXCOLOR colorRight = (curX1 < curX2) ? colorEdge2 : colorEdge3;

		for (int x = startX; x <= endX; x++)
		{
			float tX = (startX == endX) ? 0.0f : (float)(x - startX) / (endX - startX);

			B3YXCOLOR finalColor;
			ynLerp(&finalColor, colorLeft, colorRight, tX);

			SetPixel(m_hSurfaceRT, x, y, DWORD(finalColor));
		}

		curX1 += invSlope3;
		curX2 += invSlope2;
	}

	return YN_OK;
}








///////////////////////////////////////////////////////////////////////////////
//
// 렌더링 상태별 초기화. 
//
void B3YenaDevice9::_InitRenderState()
{
	::ZeroMemory(m_RState, sizeof(DWORD) * B3YRS_MAX_);

	// 렌더링 장치, 상태 기본값 설정
	//
	m_RState[B3YRS_FILLMODE] = B3YFILL_SOLID;	//단일색 채우기 
	m_RState[B3YRS_CULLMODE] = B3YCULL_CCW;		//반시계 방향(CCW) 컬링 
}




///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 조절.
//
int B3YenaDevice9::SetRenderState(B3YRENDERSTATETYPE state, DWORD value)
{
	m_RState[state] = value;

	return YN_OK;
}



///////////////////////////////////////////////////////////////////////////////
//
// 렌더링 상태 얻기   
//
int B3YenaDevice9::GetRenderState(B3YRENDERSTATETYPE state, DWORD* value)
{
	*value = m_RState[state];
	 
	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! _FaceCulling : 삼각형 컬링, 2D(Screen) 좌표 기준. 
//! 
//! 2D Screen 좌표계 정점으로 컬링판정을 수행합니다.
//!	기본 컬링 모드는 반시계방향(CCW)으로 이를 만족하면 true 를 리턴합니다. 
//! 이 결과는 렌더링 파이프라인에서 해당 기하(삼각형)을 제외하는 조건으로 사용됩니다. 
//!				
//! \warning	2D(Screen) 좌표계는 +Y 축이 화면아랫 방향으로 3D 좌표계와 다름에 주의!
//! 
//! \param	v0	컬링할 삼각형의 첫번째 정점
//! \param	v1	컬링할 삼각형의 두번째 정점
//! \param	v2	컬링할 삼각형의 세번째 정점
//! \return	컬링 조건 만족시 TRUE, 아니면 FALSE 리턴.
//
bool B3YenaDevice9::_FaceCulling(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2)
{
	bool bCull = false;

	switch (m_RState[B3YRS_CULLMODE])
	{
		case B3YCULL_NONE:				//컬링 없음. 무조건 그리기.
			bCull = false;		
			break;

		case B3YCULL_CW:				//정점 순서가 시계 방향(CW)이면, 컬링. 그리지 않음.
		{
			B3YXVECTOR2 v01 = v1 - v0;	//방향 계산 (연산자 오버로딩 추가)
			B3YXVECTOR2 v02 = v2 - v0;

			float z = B3YXVec2CCW(&v01, &v02);

			if (z > 0)					// 외적의 결과가 양수면, 'CW' 				
				bCull = true;			//곧, 컬링되어야 함.	  
		}
		break;

		case B3YCULL_CCW:				//정점 순서가 반시계 방향(CCW)이면, 컬링. 그리지 않음.
		{
			B3YXVECTOR2 v01 = v1 - v0;
			B3YXVECTOR2 v02 = v2 - v0;

			float z = B3YXVec2CCW(&v01, &v02);

			if (z < 0)					// 외적의 결과가 음수면, 'CCW' 
				bCull = true;			//곧, 컬링되어야 함.	
		}
		break;
	}


	return bCull;
}




////////////////////////////////////////////////////////////////////////////////
//
//! AddRef : Yena COM 참조 카운트 증가 
//! 
//! \return	현재 참조 카운트
//
ULONG B3YenaDevice9::AddRef(void)
{
	return ++m_ref;
};



////////////////////////////////////////////////////////////////////////////////
//
//! Release : Yena COM 참조 카운트 감소, 0 이하면 객체 제거.
//! 
//! \return	현재 참조 카운트
//
ULONG B3YenaDevice9::Release(void)
{
	if (--m_ref <= 0) delete this;

	return m_ref;
};



////////////////////////////////////////////////////////////////////////////////
//
//! QueryInterface : Yena COM 인터페이스 획득.
//! 
//! <COM> 객체에 인터페이스를 질의하고 유효한 경우 해당 인터페이스가 리턴됩니다. 
//!	지정 인터페이스 ID 로 참조되는 (원본) 객체는 참조 카운트가 증가되므로 
//! 인터페이스 사용 후 안전한 메모리 해제를 위해 Release 를 호출해야 합니다.
//! 
//! <Yena> Yena 전용 GUID 를 통해 해당 ynInterface 를 획득하며 
//! 기본적인 기능은 Windows / COM 과 동일합니다.
//! 
//! \param	riid		Yena 인터페이스 ID
//! \param	ppvObject	(생성된) Yena 인터페이스 포인터.
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::QueryInterface(YN_IID riid, _out_ void** ppvObject)
{
	//void* pInterface = nullptr;

	//지정 ID 의 유효성 검사..(간략)
	//if(riid == IID_IYena)
	//if (IsEqualGUID((GUID&)riid, (GUID&)IID_IYena))	//GUID 동일 검사
	//if (IsEqualIID((IID&)riid, (IID&)IID_IYena))		//IID 동일 검사 <상동>
	if (ynIsEqualIID(riid, IID_IYenaDevice9))			//IID 동일 검사 <상동>
	{
		AddRef();										//참조 카운트 증가
		*ppvObject = dynamic_cast<IYenaDevice9*>(this);	//지정 인터페이스 리턴
	}

	return YN_OK;
}









/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
// class B3YenaVertexBuffer : 정점 버퍼.관리 클래스
//						    : IDirect3DVertexBuffer9 대응
//
/////////////////////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////////////////////
//
B3YenaVertexBuffer9::B3YenaVertexBuffer9(void)
{
	m_pVBuffer = NULL;
	m_SizeInByte = 0;
	m_FVF = 0;
	m_Stride = 0;
	m_bLocked = FALSE;
	
	ZeroMemory(&m_Desc, sizeof(m_Desc));


#ifdef _DEBUG
	ynLog(_T("B3YenaVertexBuffer9 생성됨..."));
#endif
}



///////////////////////////////////////////////////////////////////////////////
//
B3YenaVertexBuffer9::~B3YenaVertexBuffer9(void)
{
	SafeDelArry(m_pVBuffer);

#ifdef _DEBUG
	ynLog(_T("B3YenaVertexBuffer9 제거됨..."));
#endif
}




////////////////////////////////////////////////////////////////////////////////
//
//! 사용자가 지정한 옵션으로 버퍼를 생성합니다.
//! <DX> 주어진 옵션에 맞추어 VRAM 을 확보합니다. 
//! <Yena> 일반 시스템 메모리를 생성합니다.
//! 
//! \param	desc 생성할 정점버퍼 정보기술 구조체 
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaVertexBuffer9::_Create( B3YVERTEXBUFFER_DESC desc )
{
	//이미 만들어져 있다면 오류..사용자에게 해결을 요구. 
	//예외처리 추가를 추천합니다. ex) YN_ERR_NOTEMPTY
	if (YN_VALIED(m_pVBuffer)) return YN_FAIL;

	//정점 버퍼 메모리 확보. 
	//DX 처럼 VRAM 을 확보할 수 없으므로, Pool 옵션은 무시합니다. 
	m_pVBuffer = static_cast<void*>(new BYTE[desc.Size]);	assert(m_pVBuffer);
	m_SizeInByte = desc.Size;
	m_FVF = desc.FVF;
	m_Desc = desc;

	//정점 1개의 크기(바이트) 계산하기.
	//지정된 정점규격(FVF) 기준 계산, 가능한 단순하게 처리합니다.
	m_Stride = 0;
	if (CHECK(m_FVF, B3YFVF_XY))	  m_Stride += sizeof(float) * 2;	//2d 좌표.(필수)
	if (CHECK(m_FVF, B3YFVF_DIFFUSE)) m_Stride += sizeof(DWORD);		//색상이 있다면? 크기 변경.★


	//오류 체크..
	if (m_Stride <= 0) {  /* Error!!...  */  return YN_FAIL; }


	return YN_OK;
}



////////////////////////////////////////////////////////////////////////////////
//
//! 사용자가 지정한 옵션으로 버퍼를 생성합니다.
//! <DX> 주어진 옵션에 맞추어 VRAM 을 확보합니다. 
//! <Yena> 일반 시스템 메모리를 생성합니다.
//! 
//! \param	Length	'정점 버퍼'의 전체 크기 (바이트)	
//! \param	FVF		'정점' 규격. 
//! \param	Pool	메모리 사용 옵션.  	
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaVertexBuffer9::_Create( UINT Length, DWORD FVF, B3YPOOL Pool )
{
	// 생성할 정점 버퍼 정보 기술
	// <Yena> 시스템 메모리를 사용하므로 유의미한 동작은 없으나, 구조의 일관성을 위해 사용.
	// GPU 메모리(VRAM) 확보를 위해서 이 정도의 수고는 필요하다...정도로 이해해 봅시다.
	B3YVERTEXBUFFER_DESC desc = {}; 
	desc.Format = B3YFMT_VERTEXDATA;
	desc.Type   = B3YRTYPE_VERTEXBUFFER;
	desc.Usage  = B3YUSAGE_WRITEONLY;			//정점버퍼는 CPU "쓰기전용", GPU "읽기 전용" 임을 상기합시다.
	desc.Pool	= Pool;
	desc.Size	= Length;
	desc.FVF	= FVF;
	int res = _Create(desc);

	return res;
}


/*int B3YenaVertexBuffer9::_Create(UINT Length, DWORD FVF, B3YENAPOOL Pool)
{
	//이미 만들어져 있다면 오류..사용자에게 해결을 요구. 
	//예외처리 추가를 추천합니다. ex) YN_ERR_NOTEMPTY
	if(YN_VALIED(m_pVBuffer)) return YN_FAIL; 

	//정점 버퍼 메모리 확보. 
	//DX 처럼 VRAM 을 확보할 수 없으므로, Pool 옵션은 무시합니다. 
	m_pVBuffer = static_cast<void*>(new BYTE[ Length ]);	assert(m_pVBuffer);
	m_SizeInByte = Length; 
	m_FVF = FVF;
	
	//정점 1개의 크기(바이트) 계산하기.
	//지정된 정점규격(FVF) 기준 계산, 가능한 단순하게 처리합니다.
	m_Stride = 0; 
	if(CHECK(m_FVF, B3YFVF_XY))	 m_Stride += sizeof(float)*2;		//2d 좌표.
	//if(CHECK(m_FVF, B3YFVF_DIFFUSE))  m_Stride += sizeof(DWORD);	//색상
	 
	//오류 체크..
	if(m_Stride <= 0) {  // Error!!... /  return YN_FAIL;}


	return YN_OK;
}*/



////////////////////////////////////////////////////////////////////////////////
//
//! 정점 버퍼 잠그기.
//! 
//! <DX> 정점 버퍼(VRAM) 에 데이터를 입력하기 위해서는 CPU 가 접근 가능해야 합니다.
//! Lock 함수는 버퍼를 CPU 의 "쓰기 또는 읽기" 가능 상태로 변경합니다.
//! 전체 버퍼 및 부분 잠금이 가능하며 잠금된 영역은 GPU 의 접근이 제한됩니다.("접근 잠금")
//! 사용이 끝난 버퍼는 UnLock 함수로 잠금을 해제해야 합니다.
//! 
//! <Yena> "잠금" 및 "해제" 동작을 SW 로 구현,묘사 가능하겠으나
//! 시스템 메모리를 사용중으로 특별한 기능은 추가하지 않겠습니다.
//! DX 의 실제 역활과는 다르지만, 동작의 흐름은 유지합니다.
//!
//! [참고] DX9::Lock (https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3dvertexbuffer9-lock)
//! 
//! \param	OffsetToLock	'잠금' 할 버퍼 (옵셋) 주소. 기본값 0,
//! \param	SizeToLock		'잠금' 할 버퍼 크기
//! \param	ppbData			리턴받을 버퍼 포인터
//! \param	Flags			'잠금' 옵션. 기본값 0.	
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaVertexBuffer9::Lock( UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags )
{
	if (YN_ENABLED(m_bLocked))							//잠금 상태 확인...
	{
		ynLog(_T("이 버퍼는 이미 잠겨(Locked) 있습니다. %s=%x)"), ynToString(m_pVBuffer), m_pVBuffer);
		//return YN_FAIL;		//지속적인 경고 출력을 위해 리턴하지 않음.
	}

	if (YN_INVALIED(*ppbData)) return YN_FAIL;			//유효성 검사.

	BYTE* pVB = (BYTE*)m_pVBuffer + OffsetToLock;		//지정 주소 계산.
	*ppbData = (void*)pVB;								//주소 넘김. 
	 
	//잠금 상태 설정.
	m_bLocked = TRUE;

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! Unlock 정점 버퍼 잠금 해재 
//! 
//! <DX> 현재 버퍼의 잠금 상태를 해제 합니다.
//! 이후 버퍼는 원 상태 - CPU "쓰기 불가", GPU "읽기/접근 가능" 상태가 되며
//! 파이프라인에서 사용가능해 집니다.
//! Lock 과 UnLock 함수는 쌍(Pair) 로 사용되어야 합니다.
//! 
//! <Yena> "잠금" 및 "해제" 동작을 SW 로 구현,묘사 가능하겠으나
//! 시스템 메모리를 사용중으로 특별한 기능은 추가하지 않겠습니다.
//! 
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaVertexBuffer9::Unlock(void)
{
	if (YN_DISABLED(m_bLocked))							//잠금 상태 확인...
	{
		ynLog(_T("이 버퍼는 이미 잠금 해제(Unlocked) 상태 입니다. %s=%x)"), ynToString(m_pVBuffer), m_pVBuffer);
		//return YN_FAIL;		//지속적인 경고 출력을 위해 리턴하지 않음.
	}

	//<Yena> "잠금" 및 "해제" 동작을 SW 로 구현,묘사 가능하겠으나
	//시스템 메모리를 사용중으로 특별한 기능은 추가하지 않겠습니다.
	//...

	//잠금 상태 설정.
	m_bLocked = FALSE;

	return YN_OK;
}





//GetDesc 정점 버퍼 정보 획득 
int B3YenaVertexBuffer9::GetDesc( _out_ B3YVERTEXBUFFER_DESC* pDesc )
{
	if (YN_INVALIED(pDesc)) return YN_FAIL;			//유효성 검사.
	
	*pDesc = m_Desc;

	return YN_OK;
}

void* B3YenaVertexBuffer9::GetBuffer()
{
	if (YN_INVALIED(m_pVBuffer))
	{
		ynLog(_T(" NULL 객체를 참조하고 있습니다 : %s = NULL, Size=%d, Stride=%d"),
				ynToString(m_pVBuffer), m_Desc.Size, m_Stride);
	}

	return m_pVBuffer;
}



UINT B3YenaVertexBuffer9::GetVertexCount()
{
	if (YN_INVALIED(m_pVBuffer))
	{
		ynLog(_T(" NULL 객체를 참조하고 있습니다 : %s = NULL, Size=%d, Stride=%d"), 
				ynToString(m_pVBuffer), m_Desc.Size, m_Stride);
	}

	if (m_Desc.Size <= 0 || m_Stride == 0)
	{
		ynLog(_T(" 미설정 객체를 참조하고 있습니다 : %s = %x , Size=%d, Stride=%d"), 
				ynToString(m_pVBuffer), m_pVBuffer, m_Desc.Size, m_Stride);
	}

	return m_Desc.Size / m_Stride; 
}

int B3YenaVertexBuffer9::GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData)
{
	void* pVB = GetBuffer();
	UINT  vtxs = GetVertexCount();

	//외부로 리턴.
	*ppBuffer = pVB;
	*pSizeData = vtxs;
	 
	return YN_OK;
}



ULONG B3YenaVertexBuffer9::AddRef(void)
{
	return ++m_ref;
};


ULONG B3YenaVertexBuffer9::Release(void)
{
	if (--m_ref <= 0) delete this; 

	return m_ref;
};


//QueryInterface COM 인터페이스 획득.
int B3YenaVertexBuffer9::QueryInterface(YN_IID riid, _out_ void** ppvObject)
{
	//void* pInterface = nullptr;

	//지정 ID 의 유효성 검사
	//if(riid == IID_IYena)
	//if (IsEqualGUID((GUID&)riid, (GUID&)IID_IYena))	//GUID 동일 검사
	//if (IsEqualIID((IID&)riid, (IID&)IID_IYena))		//IID 동일 검사 <상동>
	if (ynIsEqualIID(riid, IID_IYenaVertexBuffer9))		//IID 동일 검사 <상동>
	{
		AddRef();												//참조 카운트 증가
		*ppvObject = dynamic_cast<IYenaVertexBuffer9*>(this);	//지정 인터페이스 리턴
	}

	return YN_OK;
}