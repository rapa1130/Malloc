#pragma once

#ifdef BH_DLL
#define BH_API extern "C" __declspec(dllexport)
#define BH_APIX __declspec(dllexport)
#else
#define BH_API 
#define BH_APIX 
#endif 


#include<Windows.h>
#include"tchar.h"
#include<string>

namespace BH
{
	BH_APIX int bhErrorW(TCHAR* file, LONG line, TCHAR* func, BOOL bBox, HRESULT hr, TCHAR* msg, ...);

#define bhLog(msg,...) \
	bhErrorW(__FILEW__, __LINE__, __FUNCTIONW__, FALSE, S_OK, msg, __VA_ARGS__);
#define bhError(hr, msg,...) \
	bhErrorW(__FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__);


#define bhToString(str) _T("")
#define bhToStringW(str)
#define ToString(str)

	BH_APIX std::string bhStrFmtVA(const CHAR* fmt, ...);
	BH_APIX std::wstring bhStrFmtVW(const WCHAR* fmt, ...);
	BH_APIX std::basic_string<TCHAR> bhStrFmtVT(const TCHAR* fmt, ...);

#ifdef _UNICODE
#define bhStrFMTV(fmt, ...) bhStrFmtVW(fmt,__VA_ARGS__)
#else
#define bhStrFMTV(fmt, ...) bhStrFmtVA(fmt,__VA_ARGS__)
#endif

	#include <exception>

	class bhException : public std::exception
	{
	private:
		HRESULT m_hr;
		TCHAR m_strHr[1024] = _T("");
		TCHAR m_hrMsg[2048] = _T("");

	public:
		bhException() = delete;
		bhException(HRESULT hr) : m_hr(hr) { getHRMessage(hr); }
		virtual ~bhException() = default;


		_NODISCARD HRESULT hr() noexcept { return m_hr; }
		_NODISCARD virtual const TCHAR* what() noexcept { return m_hrMsg; }


		const TCHAR* getHRMessage(HRESULT hr)
		{
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
				0, m_hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				m_strHr, 1024, NULL);
			_stprintf_s(m_hrMsg, _T("[bh][bhException] Failure with HRESULT of 0x%08x : %s"),
				static_cast<unsigned int>(m_hr), m_strHr);
			return m_hrMsg;
		}
	};

	inline void bhThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw bhException(hr);
		}
	}

#define bhCheck bhThrowIfFailed
#define bhCheckFail bhThrowIfFailed
#define bhCheckError bhThrowIfFailed


	BH_APIX int bhErrorW(TCHAR* file, LONG line, TCHAR* func, BOOL bBox, bhException& e, TCHAR* msg, ...);
}






