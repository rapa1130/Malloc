//! 
//! \file	Yena.h
//! \brief	Yena SWR 기본 헤더 파일 (v1.x)
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


/////////////////////////////////////////////////////////////////////////////// 
//
// 시스템 / 플랫폼 헤더
//
#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "stdio.h"

#include "string"
#include "vector"
#include "algorithm"
using namespace std;


/////////////////////////////////////////////////////////////////////////////// 
//
// Yena SW Renderer 헤더
//
// Yena SWR 장치/공통 인터페이스
#include "ynDefine.h"		//!< Yena SWR 기본/공통 상수 정의
#include "ynError.h"		//!< Yena 에러처리 헤더.
#include "ynGUID.h"			//!< Yena SWR COM / GUID 헤더.
#include "ynUnknownbase.h"	//!< Yena SWR COM 헤더, 기반 인터페이스 헤더
#include "ynx9types.h"		//!< Yena SWR 자료형 정의 : D3D9 대응
#include "ynx9.h"			//!< Yena SWR 장치 헤더 : D3D9 대응
//#include "ynGI.h"			//!< Yena GI : DXGI 대응 운용
//#include "ynCommon.h"		//!< Yena SWR 공통 헤더 : DX11 대응
//#include "ynx11.h"		//!< Yena SWR 장치 헤더 : D3D11 대응

// Yena 확장 기능 클래스들
#include "ynMath.h"			// 수학 라이브러리.
//#include "ynXSurface.h"	// 서피스 관리.
//#include "ynTexture.h"	// 텍스쳐 관리.
//#include "ynDebug.h"		// 디버깅 관련.
//#include "ynLight.h"		// 조명 관리 
//#include "ynShader.h"		// 쉐이더 관련.

// Yena 서비스 클래스들
//#include "ynGrid.h"		//그리드.
//#include "ynAxis.h"		//방향축.

using namespace YENA;



/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/* --> 이하 클래스, 인터페이스로 전환, ynx9.h 로 이동 
/////////////////////////////////////////////////////////////////////////////// 
//
// B3Yena SW Renderer : 각종 렌더링 '디바이스 Device' 들을 관리할 최상위 클래스
//					  : Direct3D 대응 
//                    : DX 메소드와 (거의)동일한 시그니쳐(Signature) 를 
//                      구현하는 것이 목표입니다.
//
/////////////////////////////////////////////////////////////////////////////// 

class B3Yena							
{
	friend B3Yena* B3YenaCreate9(DWORD ver);
	//friend class B3YenaDevice9;			 


protected:
	B3Yena(void);			//객체생성 방지용, 생성자 숨김. 
public:
	virtual ~B3Yena(void);


	//-----------------------------------------------------------------
	// 외부 노출 메소드들 
	// DX 메소드와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//----------------------------------------------------------------

	//! Yena SWR 렌더링 디바이스 개체 생성 : D3D9 대응
	int CreateDevice(HWND hwnd,						//[in]디바이스의 렌더링 목표 윈도우 핸들.
					 YENAPRESENT_PARAMETERS* pp,	//[in]디바이스 화면 구성 정보.	
					 DWORD vp,						//[in]정점연산 방법 결정 (현재는 SW 만 가능)
					 LPB3YENADEVICE9* pDev			//[out]성공시 리턴받을 디바이스 개체 포인터.
					);

};





/////////////////////////////////////////////////////////////////////////////// 
//
// B3YenaDevice9 : DX9 전용 렌더링 "디바이스" 클래스, 
//                 IDirect3DDevice9 대응 
//                 
//                DX 또는 GL 의 지원여부 및 세부버전을 선택(DX8, DX9, DX10, DX11 등..) 하거나
//                해상도, 렌더타겟 개수 또는 HWVP 또는 SWVP 을 결정하는 등의 렌더링 관련 정보를 
//                처리하는 실제 작업용 클래스
//
//               : DX 메소드와 (거의)동일한 시그니쳐(Signature) 를 
//                구현하는 것이 목표입니다.
//
/////////////////////////////////////////////////////////////////////////////// 

class B3YenaDevice9				
{
	friend class B3Yena;
 
};

//typedef B3YenaDevice9* LPB3YENADEVICE9;		//DX9 대응.


//... 이하 클래스, 인터페이스로 전환, ynx9.h 로 이동 

*/



 



/**************** end of "B3Yena.h" ***********************************/
