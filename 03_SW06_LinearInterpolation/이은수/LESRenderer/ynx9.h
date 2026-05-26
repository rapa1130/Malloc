//! 
//! \file	ynx9.h
//! \brief	Yena SWR 인터페이스 헤더 (d3d9.h 대응)
//!			Yens SW Renderer v1.x
//! 
//! \author	김기홍 / Kihong Kim / onlysonim@gmail.com
//! \date	2003.11.4. Kihong Kim / mad_dog@hanmail.net
//! \date	2004.05.07. Updated.
//! \date	2005.09.23. Update (.Net) 
//! \date	2010.07.20. Update
//! \date	2015.11.20. Update
//! \date	2020.08.22. Updated. (DX12.x Windows 10 SDK 10.0.x)
//! \date	2024.04.28. Updated. (VS22)
//
#pragma once


////////////////////////////////////////////////////////////////////////////////
//  
//! Yena 인터페이스/클래스 전방 선언 (Yena 지원, 불요 기능 생략)
//
yninterface IYena;				//IDirect3D9 대응
yninterface IYenaDevice9;		//IDirect3DDevice9 대응

yninterface IYenaDevice9;
typedef IYenaDevice9  YENADEVICE9;			//DX9 대응.
typedef IYenaDevice9* LPYENADEVICE9;		//DX9 대응.

#define YENA_VERSION  9		//DX9.0




////////////////////////////////////////////////////////////////////////////////
//  
// Yena 자료형 선언 
// 
// <주> (이전 예제에서 정의된 자료형 이동...)
//
#include "ynx9types.h"		//!< Yena SWR 자료형 정의 : D3D9 대응 (d3d9types.h) 
//#include "ynx9cpas.h"		//!< Yena SWR 자료형 정의 : D3D9 대응 (d3d9caps.h)





/////////////////////////////////////////////////////////////////////////////// 
//
//! interface IYenaResource9 : 렌더링 자원 운용 인터페이스
//! 			 			 : IDirect3DResource9 대응  
//! 
//! DX IDirect3DResource9 인터페이스 선언, Yena 에서는 구현을 생략합니다. 
//! 전체 인터페이스 상속 구성을 위해서만 이 인터페이스를 운용합니다.
//!
//! \remarks DX 메소드와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다.
// 
////////////////////////////////////////////////////////////////////////////////
//
yninterface IYenaResource9 : public ynIUnknown
{
	/* DX 메소드 구현 생략...
	* 
	// IUnknown methods 
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	// IDirect3DResource9 methods 
	STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9 * *ppDevice) PURE;
	STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) PURE;
	STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD * pSizeOfData) PURE;
	STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
	STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew) PURE;
	STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
	STDMETHOD_(void, PreLoad)(THIS) PURE;
	STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
	*/

	//자원 버퍼 획득 : Yena 전용
	virtual void* GetBuffer		(void) pure;
	virtual int	  GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;
};




/////////////////////////////////////////////////////////////////////////////// 
//
//! interface IYenaVertexBuffer9 : 정점 버퍼 운용 인터페이스
//!								: IDirect3DVertexBuffer9 대응  
//! 
//! \remarks DX 메소드와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다.
// 
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYenaVertexBuffer9 : public IYenaResource9		
{
   
	virtual int Lock	(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) pure;
	virtual int Unlock	(void) pure;
	virtual int GetDesc (_out_ B3YVERTEXBUFFER_DESC* pDesc) pure;
 
	//자원 버퍼 획득 : Yena 전용 
	virtual void* GetBuffer		(void) pure;
	virtual UINT  GetVertexCount(void) pure;
	virtual int	  GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;

};

typedef IYenaVertexBuffer9* LPYENAVERTEXBUFFER9;
//typedef IYenaVertexBuffer9* LPB3YENAVERTEXBUFFER9;		//(구형 호환성 유지용)






/////////////////////////////////////////////////////////////////////////////// 
//
//! interface IYena : 각종 렌더링 '디바이스 Device' 들을 관리할 최상위 인터페이스
//!					   : Direct3D 대응 
//! 
//! \remarks DX 메소드와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다.
//
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYena : public ynIUnknown			
{

	//-----------------------------------------------------------------
	// 인터페이스 선언
	// DX 와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//----------------------------------------------------------------

	//! Yena SWR 렌더링 디바이스 개체 생성 : D3D9 대응
	virtual int CreateDevice(HWND hwnd,						//[in]디바이스의 렌더링 목표 윈도우 핸들.
							 YENAPRESENT_PARAMETERS* pp,	//[in]디바이스 화면 구성 정보.	
							 DWORD vp,						//[in]정점연산 방법 결정 (현재는 SW 만 가능)
							 LPYENADEVICE9* pDev			//[out]성공시 리턴받을 디바이스 개체 포인터.
							 ) pure;
	
};

typedef IYena* LPYENA;


//! Yena 개체 생성 함수 (D3D9 대응)
//! 
IYena* YenaCreate9(DWORD ver);







/////////////////////////////////////////////////////////////////////////////// 
//
//! interface IYenaDevice9 : DX9 전용 렌더링 "디바이스" 인터페이스
//!						   :  IDirect3DDevice9 대응 
//!                 
//! 렌더링 장치(GPU) 접근, 그래픽스 파이프라인의 운용, 자원 생성 및 관리 등을 담당하며
//! 스왑체인 및 렌더타겟의 해상도, 렌더타겟 개수, 색상 포멧, 또는 HWVP 또는 SWVP 등 
//! 장치 관련 정보를 처리하는 렌더링 주 작업 클래스
//!
//! \remarks DX 메소드와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다.
//
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYenaDevice9 : public ynIUnknown		
{
public:
		
	//-----------------------------------------------------------------
	// 인터페이스 선언
	// DX 와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//-----------------------------------------------------------------
	//--------------------------------
	// 스왑체인 및 렌더타겟 운용 메소드들
	//--------------------------------
	virtual int BeginScene	() pure;
	virtual int EndScene	() pure;
	virtual int Clear		(COLORREF col) pure;
	virtual int Present		() pure;

	//--------------------------------
	// 기하 버퍼 및 렌더링 메소드들 
	//--------------------------------
	virtual int CreateVertexBuffer	(UINT Length, DWORD Usage, DWORD FVF, B3YENAPOOL Pool, _out_ IYenaVertexBuffer9** ppVertexBuffer, _in_opt_ HANDLE* pSharedHandle) pure;
	virtual int SetStreamSource		(UINT StreamNumber, IYenaVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) pure;
	virtual int SetFVF				(DWORD FVF) pure;
	virtual int DrawPrimitive		(B3YPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) pure;
	
	//--------------------------------
	// 렌더링 상태 조절 메소드들.
	//--------------------------------
	virtual int SetRenderState	(B3YRENDERSTATETYPE	State, DWORD Value)   pure;
	virtual int GetRenderState	(B3YRENDERSTATETYPE State, DWORD* pValue) pure;


	//--------------------------------
	// 멤버데이터 접근자 Accessors : Yena 전용
	//--------------------------------
	virtual HDC		 GetRT		() pure;
	virtual COLORREF GetBkColor	() pure; 

};

typedef IYenaDevice9* LPYENADEVICE9;
//typedef B3YenaDevice9* LPB3YENADEVICE9;	//DX9 대응.






/////////////////////////////////////////////////////////////////////////////// 
//
// 기타 디바이스 대응 클래스의 확장 선언 예시는 아래와 같습니다.
// 확장성을 고려한 클래스 설계 베이스 입니다만, 이번 프로젝트에서는 
// 제작/사용하지 않습니다.  "이런식으로 쓸 수 있겠군.." 정도로 참고해 주십시오.
//
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
//! B3YenaDevice10 : DX10 대응 렌더링 "디바이스" 인터페이스 
//!                : ID3D10Device 대응 
//
/////////////////////////////////////////////////////////////////////////////// 

yninterface IYenaDevice10 : public ynIUnknown
{
public:

	//...DX10 대응 Device ...
};



/////////////////////////////////////////////////////////////////////////////// 
//
//! B3YenaDevice11 : DX11 대응 렌더링 "디바이스" 인터페이스 
//!                : ID3D11Device 대응 
//
/////////////////////////////////////////////////////////////////////////////// 

yninterface IYenaDevice11 : public ynIUnknown
{
public:

	//...DX11 대응 Device ...
};




/////////////////////////////////////////////////////////////////////////////// 
//
//! B3YenaDevice11 : DX12 대응 렌더링 "디바이스" 인터페이스 
//!                : ID3D12Device 대응 
//
/////////////////////////////////////////////////////////////////////////////// 

yninterface IYenaDevice12 : public ynIUnknown
{
public:

	//...DX12 대응 Device ...
};





/////////////////////////////////////////////////////////////////////////////// 
//
//! B3YenaDeviceGL : OpenGL 대응 렌더링 "디바이스" 인터페이스 
//!
//
/////////////////////////////////////////////////////////////////////////////// 

yninterface IYenaDeviceGL : public ynIUnknown
{
public:

	//...OpenGL 대응 Device ...
};



/////////////////////////////////////////////////////////////////////////////// 
//
//! B3YenaDeviceVK : Vulkan 대응 렌더링 "디바이스" 인터페이스 
//!
//
/////////////////////////////////////////////////////////////////////////////// 

yninterface IYenaDeviceVK : public ynIUnknown
{
public:

	//...Vulkan 대응 Device ...
};






/**************** end of "ynx9.h" ***********************************/
