//! 
//! \file	ynDefine.h
//! \brief	Yena SW 엔진 기본 정의 문서. 
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
//! \date	2025.04.28. Updated. (v2)(VS22)
//
#pragma once

//#include "B3API.h"		//2005.04.07. DLL Export.
#include "windows.h" 

#pragma warning(disable : 4305)
//#pragma warning(disable : 5906)

 
/////////////////////////////////////////////////////////////////////////////
//
#define YES_ 1
#define NO_	 0


/////////////////////////////////////////////////////////////////////////////
//
#include "assert.h"
#define ASSERT(Val) assert((Val))

#define YN_INVALIED(res) ((res) == NULL)
#define YN_VALIED(res)	 ((res) != NULL)
#define YN_FAILED(res)	 ((res) < 0 )

#define YN_ENABLED(res)	 ((res) == TRUE )
#define YN_DISABLED(res) ((res) == FALSE )

#define YN_CHECK	YN_ENABLED

#define YN_OK       0 
#define YN_FALSE   -1 
#define YN_FAIL    YN_FALSE  



/////////////////////////////////////////////////////////////////////////////
//
#define _LOGFILE_ON_			//로그 파일 생성하기..



/////////////////////////////////////////////////////////////////////////////
//
// DXUtil.h 의 것을 사용함. 2004.06.07. 
//
#ifndef SafeDelete
#define SafeDelete(pBuff)	if((pBuff)){ delete (pBuff);	 (pBuff) = NULL; }
#define SafeDelArry(pBuff)	if((pBuff)){ delete [] (pBuff);  (pBuff) = NULL; }
#define SafeRelease(pBuff)	if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#endif


/////////////////////////////////////////////////////////////////////////////
//
//'도'->'라디안' 으로 변경 메크로.
// pi : 180 = r : 1 
// r = pi / 180      즉  1 라디안 : 0.017444444444444444
// 
#define YN_PI	3.141592f 
#define ynConvertToRadian(deg)	((float)(YN_PI/180.0f)*((float)deg))

//구형 호환성 유지.
#define B3Y_PI	YN_PI				
#define MAKERADIAN	ynConvertToRadian




/*********** end of file "ynDefine.h" *******************************/