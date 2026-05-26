//! 
//! \file	ynUnknownbase.h
//! \brief	Yena SWR COM 기반 헤더 (v2.x)
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
//#include "tchar.h"
//#include "math.h"
//#include "stdio.h"

//#include "vector"
//#include "map"
//#include "algorithm"
//using namespace std;



/////////////////////////////////////////////////////////////////////////////// 
//
// Yena 소프트웨어 COM 상수 정의
// 
/////////////////////////////////////////////////////////////////////////////// 

//Yena SWR Interface 정의 <주> VC/COM 은 "combaseapi.h" 에 정의됨
#ifndef interface
#define __STRUCT__ struct
#define interface __STRUCT__                              //직접 선언. 
#else
//#define ynInterface struct                              //일반 interface 선언
//#define yninterface interface                           //VC COM interface 선언 사용, "combaseapi.h" 에 정의됨
#define yninterface __interface                           //VC interface 선언 사용 (Micro Specific), 아래의 YN_INTERFACE 과 동일하나 Interface rule 이 강제됨. [참고] __interface (https://learn.microsoft.com/en-us/cpp/cpp/interface?view=msvc-170)
//#define yninterface __interface  __declspec(novtable)   //상동. __interface 는 novtable 포함됨.
#endif


//(DX11 대응) Yena SWR Interface/GUID 관련 재정의 <주> VC/COM 은 "rpcndr.h" 에 정의됨
#define YN_IID    const YNID &                              //인터페이스 ID, GUID/REFFID 대응 
//#define YN_REFIID const IID &                             //인터페이스 ID, GUID/REFFID 대응
#define YN_DECLSPEC_UUID(x)     __declspec(uuid(x))         //인터페이스 ID 컴파일러 지정,  COM/DECLSPEC_UUID 대응
#define YN_DECLSPEC_NOVTABLE    __declspec(novtable)        //인터페이스 접근(생성)제한 컴파일러 옵션 설정, COM/DECLSPEC_NOVTABLE 대응
#define YN_MIDL_INTERFACE(x)    struct YN_DECLSPEC_UUID(x) YN_DECLSPEC_NOVTABLE
//#define YN_INTERFACE            struct YN_DECLSPEC_NOVTABLE
#define YN_INTERFACE(x)         struct YN_DECLSPEC_UUID(x) YN_DECLSPEC_NOVTABLE

//(DX9 대응) Yena SWR Interface/GUID 관련 재정의 <주> VC/COM 은 "combaseapi.h" 에 정의됨 
#define YN_DECLARE_INTERFACE(iface)                interface YN_DECLSPEC_NOVTABLE iface
#define YN_DECLARE_INTERFACE_(iface, baseiface)    interface YN_DECLSPEC_NOVTABLE iface : public baseiface

//(DX9/11) 인터페이스 정의 매크로
#define BEGIN_INTERFACE
#define END_INTERFACE

//Yena SW-COM 호출 규약, COM/STDMETHODCALLTYPE 대응 (__stcall : Win32 API 기본 호출규약)
#define YN_STDMETHODCALLTYPE    __stdcall                   


//메소드/함수 팔라미터 속성 표시용 (가독성 향상)
#ifndef _in_
#define _in_            //input
#define _in_out_        //input & output
#define _out_           //output
#define _out_opt_       //output, option
#define _opt_           //option
#define _in_opt_        //input, option
#endif

//메소드 순수 가상함수 표시용 (가독성 향상)
#ifndef pure
#define pure =0
#endif
 




/////////////////////////////////////////////////////////////////////////////// 
//
//! ynUnKnown : Yena SWR 최상위 인터페이스 <간략>
//!    
//! \remarks Yena SWR 은 DX (COM) 인터페이스를 필요한 만큼(가능한) 준수하고 있으나
//!          실제 COM 의 개발 모델을 완전히 적용 또는 구현하지 않습니다. (그럴 이유도 없기에...)
//!          우리의 목표는 SW Rendering 이므로, 전반적인 인터페이스 구조와 운용 흐름에 집중하여 주십시오.
// 
/////////////////////////////////////////////////////////////////////////////// 
//
//struct __declspec(novtable) ynIUnknown
yninterface ynIUnknown
{
public:
    //! 지정 ID 의 인터페이스 획득.
    virtual int QueryInterface(YN_IID riid, _out_ void** ppvObject) = 0;

    virtual ULONG AddRef (void) pure;       //!< 참조 카운트 증가+1
    virtual ULONG Release(void) pure;       //!< 참조 카운트 감소-1, 0 이되면 객체 제거

    /*template<class Q>
    int QueryInterface(_out_ Q** pp)
    {
        return QueryInterface(__uuidof(Q), (void**)pp);
    }*/
    
};




/*
/////////////////////////////////////////////////////////////////////////////// 
//
// ynUnKnown : Yena SWR 최상위 인터페이스
// 
/////////////////////////////////////////////////////////////////////////////// 
// The code examples in this topic are meant to convey general concepts, not real - world practice.
// Defining new COM interfaces is beyond the scope of this series, but you would not define an interface directly in a header file.
// Instead, a COM interface is defined using a language called Interface Definition Language(IDL).
// The IDL file is processed by an IDL compiler, which generates a C++ header file.

EXTERN_C const IID IID_ynUnknown;

extern "C++"
{
    //MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")        //<원본> COM/IUnknown GUID
    //struct __declspec(novtable) ynUnknown
    YN_MIDL_INTERFACE("00000000-0000-YENA-SWR2-000000000000")       //<예나> Yena SWR GUID (v2)
    ynUnknown
    {
    public:
        //BEGIN_INTERFACE
        virtual HRESULT YN_STDMETHODCALLTYPE QueryInterface(YN_REFIID riid, _out_ void** ppvObject) = 0;

        virtual ULONG YN_STDMETHODCALLTYPE AddRef(void) = 0;

        virtual ULONG YN_STDMETHODCALLTYPE Release(void) = 0;

        template<class Q>
        HRESULT YN_STDMETHODCALLTYPE QueryInterface(_out_ Q** pp)
        {
            return QueryInterface(__uuidof(Q), (void**)pp);
        }

        //END_INTERFACE
    };
} // extern C++
*/




/**************** end of "ynUnknownbase.h" ***********************************/
