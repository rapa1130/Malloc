/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  B3/YENA : 2D/3D 렌더링 엔진.						                       // 
//                                                                         //
//  제 작 : Kihong Kim / mad_dog@hanmail.net                               // 
//          Zero-G Interactive http://www.zero-g.kr						   //	
//  초기개발 : 2003.12 - 2004.03.                                          //
//  개발환경 : OS - Windows 2k SP4 / Visual Studio 6.0/2003/2005/2008       //
//                  DirectX 9.0b(2003, summer)                             //
//                  DirectX 9.0c(2007, November)                           //
//                  OpenGL 1.x / 2.x                                       //
//             H/W - AMD Athlon XP(Barton) 2500+@2300                      //    
//                   ATI Radeon 9600 Pro                                   //
//                   ABit NF7-II /w SAMSUNG DDR 512MB                      //
//                   SB Live! DE 5.1                                       //
//  테스트 환경 : CPU - AMD Athlon										   //	 
//                      AMD Athlonx64 w/q4                                 // 
//                      Intel Pentium III/IV & Celeron                     //
//                      Core2 w/Quad                                       //
//                VGA - ATI Radeon 9xxx Series                             //
//                          Radeon 1xxx Series                             //
//                          Radeon HD 3/4/5/6 Series                       //
//                      nVidia GeForce 2 MX/Ti Series.                     //  
//                             GeForce 3 Ti Series.                        //
//                             GeForce 4 MX/Ti Series.                     //
//                             GeForce FX Series.                          //
//                             GeForce 5/6/7/8/9xxx Series.                //
//                             GeForce 2/4xx GTS Series.                   //
//                             GeForce 7xx GTX Series.                     //
//                             GeForce 2Way-SLI / 3Way-SLI		   	       //
//                OS - Windows 98 / 2K / XP / VISTA / Win7 x32:x64         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//
//! ynError.h : 에러, 예외 및 로그 처리
//!			  : Yena SWR 용으로 발췌 및 개정 (25.0420)
//!
//! \author Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net 
//! \date	2003.11.04. Updated.
//! \date	2006.08.13. Updated.
//! \date	2010.12.01. Updated. (DX11.Jun.2010)
//! \date	2016.12.27. Updated. ynError / DX11.1 (Windows SDK 8.1)
//! \date	2018.03.05. Updated. C++17  / GCC11 / VS17
//! \date	2020.08.22. Updated. Windows 10 SDK 10.0.x (DX12.x)
//! \date	2024.04.25. Updated. DX12.X / VS22
//!	\date	2025.04.20. Yena SWR 용으로 발췌 및 개정.
//
#pragma once
//#pragma warning(disable:4996)



////////////////////////////////////////////////////////////////////////////////
//  
//! Yena 엔진 / 라이브러리 선행 처리기
//
#ifdef YENA_DLL
#define YENA_API	extern "C" __declspec(dllexport)
#define YENA_APIX	__declspec(dllexport)
#else
#define YENA_API 
#define YENA_APIX 
#endif




/////////////////////////////////////////////////////////////////////////////
//
// Yena 에러처리 전역 데이터들.
//  
namespace YENA {
//namespace YnException {

//! 에러 메세지 처리 : 일반, 로그, Output 출력, 상세, MB(선택)
YENA_APIX int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...);

//! 에러 메세지 출력 : Thread-Safe 버전.
//YENA_APIX int ynErrorWT(TCHAR* file, UINT line, TCHAR* func, HRESULT hr, UINT detail, BOOL bMBox, TCHAR* msg, ...);

//! 에러 메세지 출력 : 기본, 로그. MB(선택)
//YENA_APIX int ynErrorWL(BOOL bMBox, HRESULT hr, TCHAR* msg, ...);

//#define ynMsg(hr, msg, ...)		ynErrorWL(TRUE, hr, msg, __VA_ARGS__)
//#define ynStrFormat(msg, ...)	ynStrFmtV(msg, __VA_ARGS__).c_str()



//! 에러 메세지 출력 : 디버그+간단+MB(OFF), VS Output 간단 로그 출력용, Thread-Safe 버전.
//#define ynLogT(msg, ...) \
//		ynErrorWT( __FILEW__, __LINE__, __FUNCTIONW__, S_OK, YNERR_SIMPLE, YNERR_MB_OFF, msg, __VA_ARGS__ )


//! 에러 메세지 출력 : 일반+상세+MB(ON)
#define ynError(hr, msg, ...) \
		ynErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__ )

//! 에러 메세지 출력 : 디버그+간단+MB(OFF), VS Output 간단 로그 출력용.
#define ynLog(msg, ...) \
		ynErrorW( __FILEW__, __LINE__, __FUNCTIONW__, FALSE, S_OK, msg, __VA_ARGS__ )


// 문자열 전환
#ifndef ynToString
#define ynToString(v)  _T(#v)		//!< 문자열 전환.
#define ToString(v)    _T(#v)		//!< 문자열 전환 : 구형 호환성 유지용. 
#endif

//! DX 시스템 에러 메세지 획득.
//YENA_APIX const WCHAR* ynGetErrorStringW	  (in HRESULT hr);
//! DX 시스템 에러 메세지 획득 (Description)
//YENA_APIX void ynGetErrorDescriptionW (in HRESULT hr, out WCHAR* desc, in UINT size);

//#define ynGetErrorString		ynGetErrorStringW
//#define ynGetErrorDescription	ynGetErrorDescriptionW


//! 가변인자 문자열 구성 함수.
YENA_APIX std::string  ynStrFmtVA (const CHAR*  fmt, ...);
YENA_APIX std::wstring ynStrFmtVW (const WCHAR* fmt, ...);
YENA_APIX std::basic_string<TCHAR>  ynStrFmtVT(const TCHAR* fmt, ...);

#ifdef _UNICODE
#define ynStrFmtV(fmt, ...)	ynStrFmtVW(fmt, __VA_ARGS__)
#else
#define ynStrFmtV(fmt, ...)	ynStrFmtVA(fmt, __VA_ARGS__)
#endif




/////////////////////////////////////////////////////////////////////////////
//
// 디버깅 메세지 출력 매크로
//
/////////////////////////////////////////////////////////////////////////////

/*#define getHRMessage(hr, str) \
{	\
	TCHAR buff[1024] = _T(""); \	
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr,\
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buff, 1024, NULL); \
	str = buff; \
}*/




//////////////////////////////////////////////////////////////////////////////// 
//
// 에러처리 클래스
//
//////////////////////////////////////////////////////////////////////////////// 

//#include "windows.h"
//#include "tchar.h"
#include <exception>

//! 예외처리 헬퍼 클래스 (간단버전)(25.0122)
//! COM / DX 예외처리용
//! [참고] COM Error Code : https://learn.microsoft.com/en-us/windows/win32/com/com-error-codes-10
//! 
class ynException : public std::exception
{
private:
	HRESULT m_hr;
	TCHAR m_strhr[1024] = _T("");
	TCHAR m_hrMsg[2048] = _T("");
public:
	ynException	() = delete;
	ynException	(HRESULT hr) : m_hr(hr) { getHRMessage(); }
	virtual ~ynException () = default;

	_NODISCARD				HRESULT hr	() noexcept { return m_hr;		}
	_NODISCARD virtual const TCHAR* what() noexcept { return m_hrMsg;	}
	//		   virtual const TCHAR* what() const noexcept override
	
	const TCHAR* getHRMessage() noexcept
	{
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,		//No-line breaker.
					  0, m_hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					  m_strhr, 1024, NULL);
		_stprintf_s(m_hrMsg, _T("\n> [Yena][ynException] Failure with HRESULT of 0x%08X : %s"),
					static_cast<unsigned int>(m_hr), m_strhr);
		return m_hrMsg;
	}

	//virtual int print() noexcept{}
};


inline void ynThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw ynException(hr);
    }
}

#define ynCheck			ynThrowIfFailed
#define ynCheckFail		ynThrowIfFailed
#define ynCheckError	ynThrowIfFailed
//#define ynChkErr		ynThrowIfFailed

YENA_APIX int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, ynException& e, TCHAR* msg, ...);

//! 예외처리 메세지 출력 : 일반+상세+MB(ON)+예외(e)
#define ynError(e, msg, ...) \
		ynErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, e, msg, __VA_ARGS__ )


//} // end of namespace YENA::YnException
//using namespace YnException;


} // end of namespace YENA



/****************** end of file "ynError.h" *******************************/