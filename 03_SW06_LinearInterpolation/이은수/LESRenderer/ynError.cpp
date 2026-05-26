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
//! ynError.cpp : 에러, 예외 및 로그 처리
//!				: Yena SWR 용으로 발췌 및 개정 (25.0420)
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

//구형 C/C++ Runtime 호환성 경고 해제용 정의. (25.0118)
//#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "Yena.h"						//Yena 엔진 필수 헤더. 
#include "ynError.h"					//에러처리 헤더.


/////////////////////////////////////////////////////////////////////////////
//
//  엔진 전역 데이터들.
//  
namespace YENA
{
	//....
}

 




//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// Yena 에러처리 함수들
//
//////////////////////////////////////////////////////////////////////////////// 

namespace YENA { 
//namespace YENA::YnException { 


/* Yena SWR 에서는 미사용

////////////////////////////////////////////////////////////////////////////// 
//
// 에러 메세지 처리 : DXErr 라이브러리 사용.
//
// DXGetErrorString 를 통해 HRESULT 값을 문자열로 전환, 출력합니다.
// DXGetErrorString 는 DirectX 의 에러메세지를 처리하는 전통적인 함수였으나  
// DX 가 Windows SDK 와 통합되면서 사라졌습니다. 
// 사용자는 FormattedMessage 를 이용하여 다양한 에러 메세지를 처리 할 수 있겠으나
// 우리는 전통적인 간단한 사용법 및 Windows SDK 와의 독립성을 유지하기 위해 
// DXGetErrorString 를 구현하여 프로젝트에 추가하겠습니다. 관련사항은 다음을 참고 하십시오. 
// 첨부 : DXErr.h / cpp (Nov.8, 2015)
// 참조 : "Where is DXErr.lib?" https://blogs.msdn.microsoft.com/chuckw/2012/04/24/wheres-dxerr-lib/
//
////////////////////////////////////////////////////////////////////////////// 

////////////////////////////////////////////////////////////////////////////////
//
//! 에러 메세지 처리 : DXErr 라이브러리 사용.
//!
//! \param	bMBox	메세지박스 출력 여부.
//! \param	msg		출력할 사용자 메세지
//! \param	hr		HRESULT 에러
//! \param	pBlob	에러 메세지 데이터(Blob)
//! \return	성공하면 YN_OK, 실패하면 에러코드 (YN_xxx) 리턴.
//
YENA_APIX int ynErrorW(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob)
{
	//if(ynDbgLv <= YNDBG_LV0) return YN_OK;

	TCHAR errw[4096] = _T("");
#ifdef UNICODE            
	::mbstowcs(errw, (char*)pBlob->GetBufferPointer(), pBlob->GetBufferSize());
#else
	strcpy(errw, (char*)pBlob->GetBufferPointer(), pBlob->GetBufferSize());
#endif

	TCHAR errmsg[1024];
	_stprintf(errmsg, _T("> %s : hr=%s (0x%08X) \n\n> %s \n\n"),
		msg, DXGetErrorString(hr), hr, errw);

	//(디버깅 중) VS 출력창으로 출력..
	//OutputDebugString(_T("\n"));
	OutputDebugString(errmsg);
	//OutputDebugString(errw);

	//메세지 창 출력..
	if (bMBox)
	{
		MessageBox(NULL, errmsg, _T("Yena::Error"), MB_OK | MB_ICONERROR);
		//MessageBox(NULL, errw, _T("Yena::Error"), MB_OK | MB_ICONERROR);
	}

	//로그파일로 출력.
	//...

	return YN_OK;
}
*/



////////////////////////////////////////////////////////////////////////////// 
//
//! 에러 메세지 처리 : 예외(e), 일반, 로그, Output 출력, 상세, MB(선택)
//!
//! (25.0122) 출력 메세지 교정
//
YENA_APIX int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, ynException& e, TCHAR* msg, ...)
{
	TCHAR msgva[1024] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);

	return ynErrorW(file, line, func, bMBox, e.hr(), msgva);
}




////////////////////////////////////////////////////////////////////////////// 
//
//! 에러 메세지 처리 : 일반, 로그, Output 출력, 상세, MB(선택)
//!
//! (25.0122) 출력 메세지 교정
//! (25.0420) Yena SWR 용으로 개정
//
YENA_APIX int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...)
{
	TCHAR msgva[1024] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);
	
	//사용자 메세지 출력.
	TCHAR usermsg[1024] = _T("");
	{
		//_stprintf(usermsg, _T("\n> %s \n"), msgva);
		_stprintf(usermsg, _T("\n> [Yena] %s \t Func = %s : File = %s (%d) \n"), msgva, func, file, line);
		OutputDebugString(usermsg);
	}

	//Yena COM/DX 에러 메세지 출력.
	TCHAR outmsg[1024] = _T("");
	TCHAR hrmsg[1024] = _T("");
	if (SUCCEEDED(hr))
	{
		//성공 상태, 메세지 간단 출력...
		//_stprintf(outmsg, _T("> [Yena] %s \t Func = %s : File = %s (%d) "), msgva, func, file, line);
		//OutputDebugString(outmsg);
	}
	else
	{
		//오류 있음, 에러 메세지 (상세) 출력
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,		//No-line breaker.
					  0, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), hrmsg, 1024, NULL);
		_stprintf(outmsg, _T("> [Yena] 에러코드(0x%X) : hr=%s \n"), hr, hrmsg);
		//_stprintf(msg, _T("> [Yena] 에러코드(0x%X) : hr=%s : %s \n"), hr, DXGetErrorString(hr), hrmsg);
		OutputDebugString(outmsg);
		_stprintf(outmsg, _T(">\t\t Func = %s \n>\t\t File = %s (%d) \n\n"), func, file, line);
		OutputDebugString(outmsg);
	}

	//로그파일로 출력.
	//...

	//메세지 창 출력 : 모든 메세지 일괄 출력. 
	//MessageBox 는 Output 창과 출력 형태가 다소 상이하므로 별도로 재구성 후 출력한다.
	if (bMBox)
	{
		TCHAR msg[2048] = _T("");
		_stprintf(msg, _T("%s \n[Yena] 에러코드(0x%X) : hr=%s \nFunc = %s \nFile = %s (%d)"),
				  msgva, hr, hrmsg, func, file, line);
		MessageBox(NULL, msg, _T("Yena::Error"), MB_OK | MB_ICONERROR);
	}

	return YN_OK;
}





/* (25.0122) 이전 구형 함수. 
////////////////////////////////////////////////////////////////////////////// 
//
//! 에러 메세지 처리 : 일반, 로그, Output 출력, 상세, MB(선택)
//
YENA_APIX int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...)
{
	TCHAR msgva[1024] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);

	
	TCHAR hrmsg[1024] = _T("");
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), hrmsg, 1024, NULL);
	TCHAR errmsg[2048] = _T("");
	_stprintf(errmsg, _T("> %s \n> 에러코드(0x%X) : hr=%s : %s \n> File = %s (%d)\n> Func = %s \n\n"),
			  msgva, hr, DXGetErrorString(hr), hrmsg, file, line, func);
	//OutputDebugString(_T("\n"));
	OutputDebugString(errmsg);
 
	//로그파일로 출력.
	//...

	//메세지 창 출력 : 모든 메세지 일괄 출력. 
	if (bMBox)
	{
		MessageBox(NULL, errmsg, _T("Yena : Error"), MB_OK | MB_ICONERROR);
	}

	return YN_OK;
}
*/



/////////////////////////////////////////////////////////////////////////////
//
//! 가변인자 문자열 구성 함수. <MultiBytes>
//! 사용예:  string dst = ynStrFmtVA(msg, ...);
//! [참고] Src_ex\expAfx.h
//! 
YENA_APIX std::string  ynStrFmtVA(const CHAR* fmt, ...)
{
	char buff[1024] = "";
	va_list vl;
	va_start(vl, fmt);
	vsprintf(buff, fmt, vl);
	va_end(vl);
	return string(buff);
}


/////////////////////////////////////////////////////////////////////////////
//
//! 가변인자 문자열 구성 함수. <Unicode>
//! 사용예: wstring dst = ynStrFmtVW(msg, ...);
//! [참고] Src_ex\expAfx.h
//! 
YENA_APIX std::wstring ynStrFmtVW(const WCHAR* fmt, ...)
{
	WCHAR buff[1024] = L"";
	va_list vl;
	va_start(vl, fmt);
	vswprintf(buff, fmt, vl);
	va_end(vl);
	return wstring(buff);
}



/////////////////////////////////////////////////////////////////////////////
//
//! 가변인자 문자열 구성 함수. <TCHAR>
//! 사용예:  basic_string<TCHAR> dst = ynStrFmtVT(msg, ...);
//! [참고] Src_ex\expAfx.h
//! 
YENA_APIX std::basic_string<TCHAR>  ynStrFmtVT(const TCHAR* fmt, ...)
{
	TCHAR buff[1024] = _T("");
	va_list vl;
	va_start(vl, fmt);
	_vstprintf(buff, fmt, vl);
	va_end(vl);
	return std::basic_string<TCHAR>(buff);
}




//} // end of namespace YENA::Exception
} // end of namespace YENA
 


/****************** end of file "ynError.cpp" *********************************/



