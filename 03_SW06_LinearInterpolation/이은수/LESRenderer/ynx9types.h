//! 
//! \file	ynx9Types.h
//! \brief	Yena SW 엔진 : DX9 대응 자료형 정의, "d3d9types.h" 대응
//!			Yens SW Renderer v1.x
//! 
//! \author	김기홍 / Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date	2003.11.04. Updated.
//! \date	2004.05.07. Updated
//! \date	2005.09.23. Updated (.Net)
//! \date	2008.09.23. Updated.
//! \date	2010.07.20. Updated
//! \date	2015.11.20. Updated
//! \date	2020.08.22. Updated. (DX12.x Windows 10 SDK 10.0.x)
//! \date	2025.04.28. Updated. (1.x)(VS22)
//
#pragma once

//#include "B3API.h"		//2005.04.07. DLL Export.
#include "windows.h" 

//#pragma warning(disable : 4305)
//#pragma warning(disable : 5906)



/////////////////////////////////////////////////////////////////////////////
//
//! B3Yena 버퍼 규격 : D3DFORMAT 대응, <Yena> 미지원 기능 생략.
//! 
//! [참고] D3DFORMAT (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dformat)
//
enum B3YFORMAT
{
	B3YFMT_UNKNOWN = 0,

	// 렌더타겟용 포멧들.
	//
	//B3YFMT_R8G8B8               = 20,
	B3YFMT_A8R8G8B8 = 21,
	//B3YFMT_X8R8G8B8             = 22,
	//B3YFMT_R5G6B5               = 23,
	//B3YFMT_X1R5G5B5             = 24,
	//B3YFMT_A1R5G5B5             = 25,
	//B3YFMT_A4R4G4B4             = 26,
	//B3YFMT_R3G3B2               = 27,
	//B3YFMT_A8                   = 28,
	//B3YFMT_A8R3G3B2             = 29,
	//B3YFMT_X4R4G4B4             = 30,
	//B3YFMT_A2B10G10R10          = 31,
	//B3YFMT_A8B8G8R8             = 32,

	// Depth/Stencil Buffer 용 포멧들 : <DX> 실수표현의 정밀도 별 포멧 결정.(성능 및 호환성 고려)
	//								 : <Yena> 32bit 만 지원.
	//B3YFMT_D16_LOCKABLE         = 70,
	B3YFMT_D32 = 71,		//32bIT Depth Buffer. : DX9 미지원 하지만 Yena 에서는 이것만 사용. 
	//B3YFMT_D24S8                = 75,		//24bit Dpeth + 8bit Stencil :  (DX9)
	//B3YFMT_D24X8                = 77,		//24bit Depth Only. (DX9) 
	//B3YFMT_D16                  = 80,		//16bit Depth Only. (DX9)

	// Vertex Buffer, Index Buffer 용 포멧
	// <Yena> 시스템 메모리를 사용하므로 이 포멧은 유의미한 동작은 없으나, 구조의 일관성을 위해 사용.
	B3YFMT_VERTEXDATA = 100,
	//B3YFMT_INDEX16  = 101,
	B3YFMT_INDEX32	  = 102,

	//B3YFMT_FORCE_DWORD = 0x7fffffff
	//B3YFMT_MAX
};




/////////////////////////////////////////////////////////////////////////////
//
//! B3Yena 자원 규격 : D3DRESOURCETYPE 대응, <Yena> 미지원 기능 생략.
//
enum B3YRESOURCETYPE {
	B3YRTYPE_SURFACE = 1,				//일반 "서피스" 버퍼
	//B3YRTYPE_VOLUME = 2,
	B3YRTYPE_TEXTURE = 3,				//텍스처용
	//B3YRTYPE_VOLUMETEXTURE = 4,
	//B3YRTYPE_CUBETEXTURE = 5,
	B3YRTYPE_VERTEXBUFFER = 6,			//정점 버퍼용
	B3YRTYPE_INDEXBUFFER = 7,			//색인 버퍼용

	//B3YRTYPE_FORCE_DWORD = 0x7fffffff
};




/////////////////////////////////////////////////////////////////////////////
//
//! B3Yena 메모리 풀(Memory Pool) 종류 : D3DPOOL 대응, <Yena> 미지원 기능 생략.
//! 
//! <DX> 일반적인 자원은 GPU 의 VRAM 및 AGP 메모리를 사용하며 장치 드라이버에 의해 관리됩니다.
//! 지정된 메모리 풀에 따라 자원 별 용법과 API 가 달라집니다. 
//! 만일 런타임시 장치가 소실(Lost) 되는 경우, D3DPOOL_DEFAULT 버퍼 자원(RT, VB, IB 등) 은 
//! Release 후 수동 복구(Restore) 해야 합니다. 보다 상세한 내용은 아래 문서를 참고하십시오.
//! 
//! <Yena> 시스템 메모리만을 사용합니다 
//! <주> 메모리 관리는 중요한 요소이지만, Yena SWR 를 만드는 동안은 "그려려니" 합시다.
//! 
//! [참고] D3DPOOL (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dpool)
//! 
enum B3YPOOL {
	//B3YPOOL_DEFAULT = 0,			//기본 GPU 메모리(힙) 사용
	//B3YPOOL_MANAGED = 1,			//(자동)관리되는 GPU 메모리 사용 <DX 권장 옵션>
	B3YPOOL_SYSTEMMEM = 2,			//시스템 메모리 사용 <Yena 옵션>
	//B3YPOOL_SCRATCH = 3,

	//B3YPOOL_FORCE_DWORD = 0x7fffffff
};

typedef B3YPOOL	B3YENAPOOL;			//구형 호환성 유지.


//! 메모리 관리 영역 옵션.(구형)
//
/*enum B3YENAPOOL {
	B3YPOOL_DEFAULT = 1,	//기본값 : 비디오 메모리 사용.
	B3YPOOL_MANAGED,		//자동관리 : 비디오 - 시스템 혼용.
	B3YPOOL_SYSTEM,			//시스템메모리 사용. (현재는 이 옵션만 유효)
};
*/




/////////////////////////////////////////////////////////////////////////////
//
//! B3Yena 버퍼 용도 정의 : D3DUSAGE_XXX 대응
//!
//! <DX> 아래의 상수들은 enum 처리 없이 개별 정의 되어 있음.
//! <Yena> 동일하게 개별 상수 처리, 부분 차용, 미지원 기능 생략.
//!
//! [참고] D3DUSAGE (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dusage)
//
/////////////////////////////////////////////////////////////////////////////
// Usages for RT, DS.
#define B3YUSAGE_RENDERTARGET       (0x00000001L)
#define B3YUSAGE_DEPTHSTENCIL       (0x00000002L)
#define B3YUSAGE_DYNAMIC            (0x00000200L)

// Usages for Texture
//#define B3YUSAGE_AUTOGENMIPMAP    (0x00000400L)
//#define B3YUSAGE_DMAP             (0x00004000L)

// Usages for Vertex/Index buffers 
#define B3YUSAGE_WRITEONLY          (0x00000008L)
#define B3YUSAGE_SOFTWAREPROCESSING (0x00000010L)

//나미저 Usages 옵션 생략
//....




/////////////////////////////////////////////////////////////////////////////// 
//
//! 기하도형 타입 : D3DPRIMITIVETYPE 대응, <Yena> 미지원 기능 생략 
//!
//! [참고] D3DPRIMITIVETYPE (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dprimitivetype)
//
enum B3YPRIMITIVETYPE
{
	//B3YPT_POINTLIST           = 1,
    B3YPT_LINELIST              = 2,
    //B3YPT_LINESTRIP           = 3,
    B3YPT_TRIANGLELIST          = 4,
    //B3YPT_TRIANGLESTRIP       = 5,
    //B3YPT_TRIANGLEFAN         = 6,
	//B3YPT_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */

};




/////////////////////////////////////////////////////////////////////////////// 
//
//! 렌더링 상태 옵션. : D3DRENDERSTATETYPE 대응. 
//!
//! [참고] D3DBLENDSTATETYPE (https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3d9types/ne-d3d9types-_d3drenderstatetype)
// 
enum B3YRENDERSTATETYPE {
	//B3YRS_ZENABLE                   = 7,    
	B3YRS_FILLMODE					  = 8,    // Fill Mode State.
	//B3YRS_ZWRITEENABLE              = 14,   // TRUE to enable z writes 
	//B3YRS_ALPHATESTENABLE           = 15,   // TRUE to enable alpha tests 
	B3YRS_CULLMODE                    = 22,	  // Culling State  
	//B3YRS_ALPHABLENDENABLE          = 27,   // TRUE to enable alpha blending 
	//B3YRS_FOGENABLE                 = 28,   // TRUE to enable fog blending  
	//B3YRS_SPECULARENABLE            = 29,   // TRUE to enable specular  
	//B3YRS_LIGHTING                  = 137,
	//B3YRS_AMBIENT                   = 139, 

	//렌더링 상태값이 계속 추가될 예정.
	//..

	B3YRS_MAX_
};



/////////////////////////////////////////////////////////////////////////////// 
//
//! Fill Mode 별 옵션. : D3DFILLMODE  대응. 
//!
//! [참고] D3DFILLMODE (https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/d3d9types/ne-d3d9types-_d3dfillmode)
//
enum B3YFILLMODE {
	B3YFILL_POINT		= 1,		//점으로 그리기.
	B3YFILL_WIREFRAME	= 2,		//선으로 그리기.
	B3YFILL_SOLID		= 3,		//(지정)색으로 채우기.

	//B3YCULL_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
};



//
// Culling Mode 별 옵션.: D3DCULL 대응  
//
enum B3YCULL {
	B3YCULL_NONE	= 1,	//컬링 없음. 
	B3YCULL_CW		= 2,	//시계방향 컬링.
	B3YCULL_CCW		= 3,	//반시계방향 컬링.(기본값) 
	
	//B3YCULL_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
};




/////////////////////////////////////////////////////////////////////////////// 
//
//! 정점 규격 정의 (하나 이상의 규격 조합 가능) : D3DFVF 대응 
// 
#define  B3YFVF_XY			 0x0001		//2D 좌표.(DX 미지원, Yena 전용)
//#define  B3YFVF_XYZ		 0x0002		//3D 좌표.(미 변환)(Local)
//#define  B3YFVF_XYZRHW	 0x0004		//3D 좌표.(변환 완료) XY + Depth
//#define  B3YFVF_NORMAL	 0x0010		//노멀. 
#define  B3YFVF_DIFFUSE		 0x0040		//확산색(Diffuse) 색상
//#define  B3YFVF_SPECULAR	 0x0080		//정반사(Specular) 색상. 
//#define  B3YFVF_TEX1		 0x0100		//텍스처 좌표.1 
//#define  B3YFVF_TEX2		 0x0200		//텍스처 좌표.2 

#define CHECK( value, bit) (((value) & (bit)) == bit)




/////////////////////////////////////////////////////////////////////////////// 
//
//! 렌더타겟 (백버퍼) - 정보 설정 구조체 : D3DPRESENT_PARAMETERS 대응 
//!
//! <Yena> 미지원 기능 생략
//! [참고] D3DPRESENT_PARMETERS (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dpresent-parameters)
//
struct YENAPRESENT_PARAMETERS {

	DWORD		Width;				//해상도 : 클라이언트 영역기준.
	DWORD		Height;
	DWORD		BackBuffercnt;		//백버퍼 개수
	BOOL		Windowed;			//창모드 실행?

	//이하 생략...
	//B3YFORMAT		BackBufferFormat;
	//B3YSWAPEFFECT SwapEffect;
	//...

};



/////////////////////////////////////////////////////////////////////////////// 
//
//! 해상도 정보 구조체. : D3DDISPLAYMODE 대응 
//!
//! <Yena> 미지원 기능 생략
//! [참고] D3DDISPLAYMODE (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3ddisplaymode)
//! 
struct YENADISPLAYMODE {
	DWORD Width, Height;

	//이하 생략...
	//UINT            RefreshRate;
	//B3YFORMAT       Format;
};



/////////////////////////////////////////////////////////////////////////////// 
//
//! 장치 생성 플래그 : 연산 가속화 정의, D3DCREATE 대응 
//!
//! [참고] D3DCREATE (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dcreate)
//! 
enum YENACREATE
{
	YENACREATE_SOFTWARE_VERTEXPROCESSING,	//!< CPU 가 정점 연산을 처리.<Yena 기본>
	YENACREATE_HARDWARE_VERTEXPROCESSING,	//!< GPU 가 정점 연산을 처리.<Yena 불가>
	//YENACREATE_MIXED_VERTEXPROCESSING		//!< CPU / GPU 혼성 연산 가능.
	//YENACREATE_MULTITHREADED				//!< 다중쓰레드 환경 호환성 추가.
	//...
};

typedef YENACREATE	BEHEVIOR_FLAG;			//구형 호환성 유지.




/////////////////////////////////////////////////////////////////////////////
//
//! Vertex Buffer Description : 정점 버퍼 정보 기술, D3DVERTEXBUFFER_DESC 대응 
//!
//! [참고] D3DVERTEXBUFFER_DESC (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dvertexbuffer-desc)
//!
struct B3YVERTEXBUFFER_DESC
{
	B3YFORMAT           Format;		//!< 버퍼 포멧
	B3YRESOURCETYPE     Type;		//!< 자원 형식
	DWORD               Usage;		//!< 자원 용도
	B3YPOOL             Pool;		//!< 메모리 형식
	UINT                Size;		//!< 크기 (Bytes)
	
	DWORD               FVF;		//!< 정점 규격
};








/*********** end of file "ynx9Types.h" *******************************/