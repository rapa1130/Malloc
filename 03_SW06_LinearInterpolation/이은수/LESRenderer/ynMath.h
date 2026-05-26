//! 
//! \file	ynMath.h
//! \brief	Yena Math Library for DirectX / OpenGL
//!			Yens SW Renderer v2.x
//! 
//! \author	Kihong Kim / mad_dog@hanmail.net
//! \date	2004.05.07. Updated.
//! \date	2010.07.20. Updatee.
//! \date	2015.11.20. Updated.
//! \date	2025.04.28. Updated. (v1.x)(VS22)
//
#pragma once


/////////////////////////////////////////////////////////////////////////////// 
//
//OpenGL 표준 헤더파일. 필수요소.
//#include <GL/gl.h>
//#include <GL/glu.h>

#define YN_PI   3.141592f
#define ynToRadian(a)  (a/180.0f * YN_PI)
#define ynToAngle(r)   (r * 180.0f / YN_PI)
#define ynToDegree(r)  ynToAngle(r)

//구형 호환성 재정의
#define YNToRadian	ynToRadian
#define YNToAngle	ynToAngle
#define YNToDegree	ynToDegree






/////////////////////////////////////////////////////////////////////////////// 
//
//! 벡터 데이터.
//!
//! 멤버데이터를 '수평:행'으로 나열했지만,
//! 벡터/행렬 연산시에는 '수직:열'기준 처리됨을 기억합시다.
//! 아래의 벡터 표현식 참조.
//!
//!  x   or   x
//!  y        y
//!  z        z
//!           w
//
/////////////////////////////////////////////////////////////////////////////// 

//! 2성분 벡터 구조체.
//
struct B3YVECTOR2 {
	float x, y;  
};


//! 3성분 벡터 구조체.
//
struct B3YVECTOR3 {
	float x, y, z;  
};


//! 4성분 벡터.구조체.
//
struct B3YVECTOR4 {
	float x, y, z, w; 
};




////////////////////////////////////////////////////////////////////////////////
// 
//! VECTOR2 일반 구조체 + 생성자/연산자 오버로딩.
//
class B3YXVECTOR2  
{
public:
	float x, y; 
public:
	B3YXVECTOR2()					{ x = 0;	y = 0;	 }
	B3YXVECTOR2(float _x, float _y) { x = _x;	y = _y;	 }
	B3YXVECTOR2(B3YVECTOR2 v)		{ x = v.x;  y = v.y; } 

	//연산자 오버로딩.
	B3YXVECTOR2 operator + (B3YXVECTOR2 rhs);
	B3YXVECTOR2 operator - (B3YXVECTOR2 rhs);
	B3YVECTOR2	operator = (B3YXVECTOR2 rhs);
	operator B3YVECTOR2* ();


	operator POINT() { return POINT{ (LONG)x, (LONG)y }; }
	
};
typedef B3YXVECTOR2 VECTOR2;


//! 2성분 외적 : 외적의 결과가 양수면, 'CCW' .Back-Face Culling 에 사용.
//!			   : D3DXVec2CCW 대응 (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec2ccw)
//! 
float B3YXVec2CCW(B3YXVECTOR2* v0, B3YXVECTOR2* v1);	




////////////////////////////////////////////////////////////////////////////////
// 
//! VECTOR3 일반 구조체 + 생성자/연산자 오버로딩.
//
class B3YXVECTOR3  
{
public:
	float x, y, z; 
public:
	B3YXVECTOR3(float _x, float _y, float _z){ x = _x;  y = _y;   z = _z; 	}
	B3YXVECTOR3(B3YVECTOR3 v)				 { x = v.x;  y = v.y;  z = v.z; }  
};
typedef B3YXVECTOR3 VECTOR3;




////////////////////////////////////////////////////////////////////////////////
// 
//! VECTOR4 일반 구조체 + 생성자/연산자 오버로딩.
//
class B3YXVECTOR4  
{
public:
	float x, y, z, w; 
public:
	B3YXVECTOR4(float _x, float _y, float _z, float _w)	{ x = _x;  y = _y;   z = _z;  w = _w; 	}
	B3YXVECTOR4(B3YVECTOR4 v)							{ x = v.x;  y = v.y;  z = v.z; w = v.w; }  

};
typedef B3YXVECTOR4 VECTOR4;


 

////////////////////////////////////////////////////////////////////////////////
//  
//! 컬러 구조체.
//! 
struct B3YCOLOR 
{
	float r, g, b, a;
};
typedef B3YCOLOR COLOR;



////////////////////////////////////////////////////////////////////////////////
//
//! 컬러 클래스. : B3YCOLOR 확장 클래스 
//!             : 필요한 메소드/연사자 를 완성할 것.★
//
class B3YXCOLOR {
public:
	float r, g, b, a;

	B3YXCOLOR() { r = g = b = a = 1.0f; }
	B3YXCOLOR(float _r, float _g, float _b, float _a = 1) { r = _r; g = _g; b = _b; a = _a; }
	//B3YXCOLOR(BYTE _r, BYTE _g, BYTE _b, BYTE _a);	
	//B3YXCOLOR(COLORREF col);						
	B3YXCOLOR(DWORD col);						//★

	B3YXCOLOR operator* (B3YXCOLOR rhs);		//색상 채널별 곱.★
	B3YXCOLOR operator* (float rhs);			//색상 스칼라 곱.★
	B3YXCOLOR operator+ (B3YXCOLOR rhs);		//색상 덧셈 (채널별)★
	B3YXCOLOR operator- (B3YXCOLOR rhs);		//색상 뺄셈 (채널별)★
	operator DWORD ();							//DWORD 형변환.★
};


//사용 안함.
//#define TOCOLOR(col)  (*(COLOR*)&(col))
//COLOR   B3YXColor(float r, float g, float b, float a);

 


////////////////////////////////////////////////////////////////////////////////
//  
//! 헬퍼 함수들 
//

//선형보간 (Linear Interpolation) 함수 
void  ynLerp	(int* v, int v0, int v1, float a);
//void  ynLerp	(DWORD* c, DWORD c0, DWORD c1, float a); 			 //색상(DWORD)
void  ynLerp	(B3YXCOLOR* c, B3YXCOLOR c0, B3YXCOLOR c1, float a); //색상(B3YXCOLOR)★


//!구형 호환성 유지용 재정의
//! 
#define YnLerp		ynLerp






 
/************* end of file "ynMath.h" *******************************************/



