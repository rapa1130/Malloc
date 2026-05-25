#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include"bhError.h"
#include"bhDefine.h"


namespace BH
{
	BH_APIX int bhErrorW(TCHAR* file, LONG line, TCHAR* func, BOOL bBox, bhException& e, TCHAR* msg, ...)
	{
		TCHAR buff[1024] = _T("");
		va_list vl;
		va_start(vl, msg);
		_vstprintf(buff, msg, vl);
		va_end(vl);

		return bhErrorW(file, line, func, bBox, e.hr(), buff);
	}

	BH_APIX int bhErrorW(TCHAR* file, LONG line, TCHAR* func, BOOL bBox, HRESULT hr, TCHAR* msg, ...)
	{
		TCHAR buff[1024] = _T("");
		va_list vl;
		va_start(vl, msg);
		_vstprintf(buff, msg, vl);
		va_end(vl);

		TCHAR userMsg[1024] = _T("");
		{
			_stprintf(userMsg, _T("\n> [BH] %s \t Func = %s : File = %s (%d) \n"), buff, func, file, line);
			OutputDebugString(userMsg);

		}



		TCHAR hrMsg[1024] = _T("");
		TCHAR outMsg[1024] = _T("");


		if (SUCCEEDED(hr))
		{

		}
		else
		{
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
				0, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), hrMsg, 1024, NULL);
			OutputDebugString(hrMsg);
			_stprintf(outMsg, _T(">\t\t Func = %s \n>\t\t File = %s (%d) \n\n"), func, file, line);
			OutputDebugString(outMsg);
		}

		if (bBox)
		{
			TCHAR msg[2048] = _T("");
			_stprintf(msg, _T("%s \n[BH] 에러코드(0x%X) : hr=%s \nFunc = %s \nFile = %s (%d)"),
				userMsg, hr, hrMsg, func, file, line);
			MessageBox(NULL, msg, _T("BH::Error"), MB_OK | MB_ICONERROR);
		}
		return BH_OK;
	}
	BH_APIX std::string bhStrFmtVA(const CHAR* fmt, ...)
	{
		char buff[1024] = "";
		va_list vl;
		va_start(vl, fmt);
		vsprintf(buff, fmt, vl);
		va_end(vl);

		return std::string(buff);
	}

	BH_APIX std::wstring bhStrFmtVW(const WCHAR* fmt, ...)
	{
		WCHAR buff[1024] = L"";
		va_list vl;
		va_start(vl, fmt);
		vswprintf(buff, fmt, vl);
		va_end(vl);

		return std::wstring(buff);
	}

	BH_APIX std::basic_string<TCHAR> bhStrFmtVT(const TCHAR* fmt, ...)
	{
		TCHAR buff[1024] = _T("");
		va_list vl;
		va_start(vl, fmt);
		_vstprintf(buff, fmt, vl);
		va_end(vl);

		return std::basic_string<TCHAR>(buff);
	}

}

