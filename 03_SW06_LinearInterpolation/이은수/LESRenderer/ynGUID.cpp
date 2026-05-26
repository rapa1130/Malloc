//! 
//! \file	ynUnknownbase.cpp
//! \brief	Yena SWR COM 기반 소스 (v2.x)
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

//Yena SWR COM / GUID 기반 헤더
#include "ynGUID.h"



/////////////////////////////////////////////////////////////////////////////// 
//
// Yena SWR COM / GUID 전역 공통 데이터
// 
/////////////////////////////////////////////////////////////////////////////// 

//! Yena IID 정의 : 인터페이스 별 고유 ID 지정.
//! 
//! <예시> 표준 GUID : IUnknown = { {128bit, 16진수, 8개}{16진수 4개}{16진수4개}{16진수4개}{16진수12개}
//!       const GUID IID_IUnknown = { "00000000-0000-0000-C000-0000000000046" };
//! 
//! <예시3> 
//! Yena Interface IID 범례 = { "YNSW"(8), 엔진버전(4), API(4), 버전(2), 인터페이스명(2), 버전+부가정보(4) };
//! IID_ynIUnknown          = { YNSW, B3, DX, 9.0, 00, 0.0 }; 
//! IID_IYena               = { YNSW, B3, DX, 9.0, YN, 1.0 }; 
//! IID_IYenaDevice9        = { YNSW, B3, DX, 9.0, DV, 1.0 }; 
//! IID_IYenaVertexBuffer9  = { YNSW, B3, DX, 9.0, VB, 1.0 };
//
//! Yena Interface IID 범례                 = { "YNSW"(8), 엔진버전(4), API(4), 버전(2), 인터페이스명(4), 버전+부가정보(10) };
const YNID IID_ynIUnknown          = { 0x594E5357, 0xB300, 0x4458, { 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }; 
const YNID IID_IYena               = { 0x594E5357, 0xB300, 0x4458, { 0x90, 0x59, 0x42, 0x00, 0x00, 0x00, 0x00, 0x01 } };   // Yena 버전 : 0x09 + 0x01~FF (8bit)
const YNID IID_IYenaDevice9        = { 0x594E5357, 0xB300, 0x4458, { 0x90, 0x44, 0x56, 0x00, 0x00, 0x00, 0x00, 0x01 } };   // YenaDevice 버전 : 0x09 + 0x01~0xFF (8bit)
const YNID IID_IYenaVertexBuffer9  = { 0x594E5357, 0xB300, 0x4458, { 0x90, 0x56, 0x42, 0x00, 0x00, 0x00, 0x00, 0x01 } };   // YenaVB 버전 :  0x09 + 0x010~0xFF (8bit) 

//! GUID 정의 매크로 (예시)(상동)
DEFINE_GUID(IID_YN_TEST_INTERFRACE, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);




////////////////////////////////////////////////////////////////////////////////
//
//! ynIsEqualIID : Yena IID 동일 여부 검사 함수
//!              : IsEqualGUID, IsEqualIID 와 동일 기능
//! 
//! [참고] IsEqualGUID, IsEqualIID (https://learn.microsoft.com/en-us/windows/win32/api/guiddef/nf-guiddef-isequaliid)
//!
//! \param	id1     검사할 IID
//! \param	id2     감사할 IID
//! \return	    동일하면 TRUE, 그렇지 않으면 FALSE.
//
BOOL ynIsEqualIID(const YNID& id1, const YNID& id2)
{
    return !memcmp(&id1, &id2, sizeof(YNID));
}




/////////////////////////////////////////////////////////////////////////////// 
//
//! Yena GUID 비교 함수
//
/*BOOL ynIsEqualIID(const YNID& id1, const YNID& id2)
{
	BOOL res1 = (id1.Data1 == id2.Data1) ? TRUE : FALSE;
	BOOL res2 = (id1.Data2 == id2.Data2) ? TRUE : FALSE;
	BOOL res3 = (id1.Data3 == id2.Data3) ? TRUE : FALSE;
	BOOL res4 = ((UINT)id1.Data4 == (UINT)id2.Data4) ? TRUE : FALSE;
	return (res1 && res2 && res3 && res4) ? TRUE : FALSE;

	///UINT res1 = (id1.Data1 == id2.Data1) ? 0x1000 : 0x00;
	///UINT res2 = (id1.Data2 == id2.Data2) ? 0x0100 : 0x00;
	///UINT res3 = (id1.Data3 == id2.Data3) ? 0x0010 : 0x00;
	///UINT res4 = ((UINT)id1.Data4 == (UINT)id2.Data4) ? 0x0001 : 0x00;
	///return (res1 | res2 | res3 | res4) ? TRUE : FALSE;
	///
}*/





/**************** end of "ynUnknownbase.cpp" ***********************************/
