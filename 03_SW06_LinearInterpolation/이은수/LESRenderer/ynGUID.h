//! 
//! \file	ynGUID.h
//! \brief	Yena SWR COM / GUID 헤더
//!			unknownbase.h 대응 (Windows/COM)
//!			Yens SW Renderer v2.x
//! 
//! \author	김기홍 / Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date	2003.11.04. Updated.
//! \date	2004.05.07. Updated.
//! \date	2005.09.23. Updated (.Net)
//! \date	2008.09.23. Updated.
//! \date	2010.07.20. Updated
//! \date	2015.11.20. Updated
//! \date	2020.08.22. Updated. (DX12.x Windows 10 SDK 10.0.x)
//! \date	2025.04.28. Updated. (v2)(VS22)
// 
#pragma once
//#pragma warning(disable:4996)

//시스템 / 플랫폼 헤더
#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "stdio.h"

#include "vector"
#include "map"
#include "algorithm"
using namespace std;


/////////////////////////////////////////////////////////////////////////////// 
//
//! GUID/UUID 정의 
//! Windows COM 은 인터페이스 및 클래스 객체 식별용 ID 로 GUID(UUID)를 사용합니다.
//! GUID 는 128bit 데이터 구조체이며 보통 문자열 형태로 정의됩니다.
//! Windows COM 의 GUID 들은 별도 정의되어 라이브러리(UUID.lib) 형태로 배포됩니다. 
//! 
//! 표준 GUID : IUnknown = {{16진수 8개}{16진수 4개}{16진수4개}{16진수4개}{16진수12개}}
//!                     = "00000000-0000-0000-C000-000000000046"
//!
//! 사용자 정의 GUID 생성은 guidgen.exe 또는 cuidgen.exe 같은 단독 툴이나 또는    
//! CoCreateGUID, UUidCreate 함수를 사용합니다.
//! 
//! <Yena> SWR 는 정규 GUID 형식에서 살짝 벗어나 적당한 IID (임의의 상수)를 정의하겠습니다.
//!
//! [참고] GUID (https://learn.microsoft.com/en-us/windows/win32/api/guiddef/ns-guiddef-guid)
//! [참고] CoCreateGUID (https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-cocreateguid)
//! [참고] UuidCreate (https://learn.microsoft.com/en-us/windows/win32/api/rpcdce/nf-rpcdce-uuidcreate)
//! [참고] Defining GUID (https://learn.microsoft.com/en-us/windows-hardware/drivers/kernel/defining-and-exporting-new-guids)
//! 
/////////////////////////////////////////////////////////////////////////////// 
//
//! Yena GUID 구조체 : GUID 대응 
//! <Yena> SWR 는 정규 GUID 형식에서 살짝 벗어나 적당한 IID 를 정의하겠습니다.(임의 상수)
//! 
typedef struct _YNID {
    unsigned long  Data1;       //16진수 8개
    unsigned short Data2;       //16진수 4개
    unsigned short Data3;       //16진수 4개
    unsigned char  Data4[8];    //16진수 4개(2byte) + 16진수 6개(4byte)
} YNID;


//! Yena IID 정의 : 인터페이스 별 고유 ID 지정.
EXTERN_C const YNID IID_ynIUnknown;
EXTERN_C const YNID IID_IYena;
EXTERN_C const YNID IID_IYenaDevice9;
EXTERN_C const YNID IID_IYenaVertexBuffer9;     


//! Yena IID 동일 여부 검사 함수
//! [참고] IsEqualGUID, IsEqualIID (https://learn.microsoft.com/en-us/windows/win32/api/guiddef/nf-guiddef-isequaliid)
//! 
BOOL ynIsEqualIID(const YNID& id1, const YNID& id2);



/*
//(DX11 대응) Yena SWR Interface/GUID 관련 재정의 <주> VC/COM 은 "rpcndr.h" 에 정의됨
#define YN_IID    const YNID &                              //인터페이스 ID, GUID/REFFID 대응 
//#define YN_REFIID const IID &                             //인터페이스 ID, GUID/REFFID 대응

#define YN_DECLSPEC_UUID(x)     __declspec(uuid(x))         //인터페이스 ID 컴파일러 지정,  COM/DECLSPEC_UUID 대응
#define YN_DECLSPEC_NOVTABLE    __declspec(novtable)        //인터페이스 접근(생성)제한 컴파일러 옵션 설정, COM/DECLSPEC_NOVTABLE 대응
#define YN_MIDL_INTERFACE(x)    struct YN_DECLSPEC_UUID(x) YN_DECLSPEC_NOVTABLE
//#define YN_INTERFACE            struct YN_DECLSPEC_NOVTABLE
#define YN_INTERFACE(x)         struct YN_DECLSPEC_UUID(x) YN_DECLSPEC_NOVTABLE
*/


 



/**************** end of "ynGUID.h" ***********************************/
