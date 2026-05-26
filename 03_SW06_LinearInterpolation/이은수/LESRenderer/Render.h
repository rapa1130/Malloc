//! 
//! \file	Render.h
//! \brief	기본 렌더링/프레임웍 헤더 
//!			Yena S/W Renderer v1.x
//! 
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date   2012.01.17. v0.9   DX9.x, VS08
//! \date   2012.02.01. v1.0   DX9.x, VS10
//! \date   2013.04.10. v1.2.0 DX9.x, VS08
//! \date   2015.10.10. v1.2.1 DX9.x, VS15
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//! \date   2025.05.01. v1.9.0   DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//
//#define _CRT_SECURE_NO_WARNINGS
#pragma once

 
int		DataLoading();
void	DataRelease();

void	SceneRender();	

extern HWND g_hWnd;



/****************** end of file "Render.h" ***********************************/