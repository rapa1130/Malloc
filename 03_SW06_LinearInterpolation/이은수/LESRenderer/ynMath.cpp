//! 
//! \file	ynMath.cpp
//! \brief	Yena Math Library for DirectX / OpenGL
//!			Yens SW Renderer v2.x
//! 
//! \author	Kihong Kim / mad_dog@hanmail.net
//! \date	2004.05.07. Updated.
//! \date	2010.07.20. Updatee.
//! \date	2010.10.10. Updated. glyVec3Add, glyVec3Sub
//! \date	2015.11.20. Updated.
//! \date	2025.04.28. Updated. (v1.x)(VS22)
//
//
//
//
#include "Windows.h"  
#include "stdio.h"
#include "stdlib.h" 
#include "math.h"

#include "vector"
#include "algorithm"
using namespace std;

#include "ynMath.h"

#define GLYMATH_VERSION  "2010.10.10"




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// struct COLOR
//
///////////////////////////////////////////////////////////////////////////////

//
//! 생성자 오버로딩.
//!
//! BYTE(r, g, b, a) -> float (r, g, b, a)
// 
/*B3YXCOLOR::B3YXCOLOR(BYTE _r, BYTE _g, BYTE _b, BYTE _a)
{
	b = _b/255.0f;					//실수형으로 전환.  0~ 1.0f
	g = _g/255.0f;
	r = _r/255.0f;
	a = _a/255.0f;
}
*/

//! DWORD 형 색상에서 float 타입으로 색상정보를 전환합니다.★
//! DWORD(a, r, g, b) --> float(r, g, b, a) 로 전환. (GDI용)  
//!   0x AA RR GG BB 
//! 각 채널별 시작 비트..
//! Blue 채널: 0번비트  
//! Gree 채널: 8번비트 
//! Red  채널: 16번비트 
//! Alpha 채널: 24번비트
//
B3YXCOLOR::B3YXCOLOR(DWORD col)
{
	// 필요 코드를 완성하십시요.
	//

	//<Blue> 변환
	b = (float)(col & 0x000000ff);		//blue 채널값 얻기. 0 ~ 255
	b = b / 255.0f;						//실수형으로 전환. 0 ~ 1.0f

	//<Green> 변환
	g = (float)((col & 0x0000ff00) >> 8);	//green 채널값 얻기,
	g = g / 255.0f;						//실수형으로 전환. 0 ~ 1.0f

	//<Red> 변환
	r = (float)((col & 0x00ff0000) >> 16);	//red 채널값 얻기,
	r = r / 255.0f;						//실수형으로 전환. 0 ~ 1.0f

	//<Alpha> 변환
	a = (float)((col & 0xff000000) >> 24);	//alpha 채널값 얻기,
	a = a / 255.0f;

}




// 
//! 색상 연산 오버로딩..★
//!
//! 모든 연산에 색상 포화도(Saturation) 제한을 처리합니다.
//! 0 ~ 1.0f ( 0~ 255)
//
 
//! 색상 채널별 곱셈.  
B3YXCOLOR B3YXCOLOR::operator * (B3YXCOLOR rhs)
{
	B3YXCOLOR v;

	//함수의 바디(Body) 를 완성하십시요..★
	//...

	// R 채널 연산 및 제한
	v.r = r * rhs.r;
	if (v.r < 0.0f) v.r = 0.0f;
	else if (v.r > 1.0f) v.r = 1.0f;

	// G 채널 연산 및 제한
	v.g = g * rhs.g;
	if (v.g < 0.0f) v.g = 0.0f;
	else if (v.g > 1.0f) v.g = 1.0f;

	// B 채널 연산 및 제한
	v.b = b * rhs.b;
	if (v.b < 0.0f) v.b = 0.0f;
	else if (v.b > 1.0f) v.b = 1.0f;

	// A 채널 연산 및 제한
	v.a = a * rhs.a;
	if (v.a < 0.0f) v.a = 0.0f;
	else if (v.a > 1.0f) v.a = 1.0f;

	return v;
}


//! 색상 스칼라 곱.: ★ 
B3YXCOLOR B3YXCOLOR::operator * (float rhs)
{
	B3YXCOLOR v;

	//함수의 바디(Body) 를 완성하십시요..★
	//...

	v.r = r * rhs;
	if (v.r < 0.0f) v.r = 0.0f;
	else if (v.r > 1.0f) v.r = 1.0f;

	v.g = g * rhs;
	if (v.g < 0.0f) v.g = 0.0f;
	else if (v.g > 1.0f) v.g = 1.0f;

	v.b = b * rhs;
	if (v.b < 0.0f) v.b = 0.0f;
	else if (v.b > 1.0f) v.b = 1.0f;

	v.a = a * rhs;
	if (v.a < 0.0f) v.a = 0.0f;
	else if (v.a > 1.0f) v.a = 1.0f;

	return v;
}

//! 색상 혼합.★
B3YXCOLOR B3YXCOLOR::operator + (B3YXCOLOR rhs)
{
	B3YXCOLOR v;

	//함수의 바디(Body) 를 완성하십시요..★
	//...

	v.r = r + rhs.r;
	if (v.r < 0.0f) v.r = 0.0f;
	else if (v.r > 1.0f) v.r = 1.0f;

	v.g = g + rhs.g;
	if (v.g < 0.0f) v.g = 0.0f;
	else if (v.g > 1.0f) v.g = 1.0f;

	v.b = b + rhs.b;
	if (v.b < 0.0f) v.b = 0.0f;
	else if (v.b > 1.0f) v.b = 1.0f;

	v.a = a + rhs.a;
	if (v.a < 0.0f) v.a = 0.0f;
	else if (v.a > 1.0f) v.a = 1.0f;

	return v;
}

//! 색상 뺄셈.★
B3YXCOLOR B3YXCOLOR::operator - (B3YXCOLOR rhs)
{
	B3YXCOLOR v;

	//함수의 바디(Body) 를 완성하십시요..★
	//...

	v.r = r - rhs.r;
	if (v.r < 0.0f) v.r = 0.0f;
	else if (v.r > 1.0f) v.r = 1.0f;

	v.g = g - rhs.g;
	if (v.g < 0.0f) v.g = 0.0f;
	else if (v.g > 1.0f) v.g = 1.0f;

	v.b = b - rhs.b;
	if (v.b < 0.0f) v.b = 0.0f;
	else if (v.b > 1.0f) v.b = 1.0f;

	v.a = a - rhs.a;
	if (v.a < 0.0f) v.a = 0.0f;
	else if (v.a > 1.0f) v.a = 1.0f;


	return v;
}


//! [ GDI 대응 형변환 ]★
//! float(r, g, b, a) -> DWORD(a, b, g, r) 로 전환. (GDI용) 
//! Blue 채널이 16번비트..주의.★
//
B3YXCOLOR::operator DWORD ()
{
	COLORREF col{};

	//함수의 바디(Body) 를 완성하십시요..★
	//...
	DWORD R = (int)(r * 255.0f);
	DWORD G = (int)(g * 255.0f);
	DWORD B = (int)(b * 255.0f);
	DWORD A = (int)(a * 255.0f);

	// GDI RGB 형식: 0x00BBGGRR
	col = RGB(R, G, B);

	return col;
}



/* [ DX 대응 형변환] <예시> ★
// float(r, g, b, a) -> DWORD(a, r, g, b) 로 전환. (DX용)
// Blue 채널이 0번비트..임에 주의.
//
B3YXCOLOR::operator DWORD ()
{
	DWORD col;
	DWORD B = (DWORD)(b*255.0f);		//여기서 다시 실수형으로 전환.  0~1.0f -> 0~255.0f;
	col =  B ;							//정수형.blue 넣기.  0 ~ 255

	DWORD G = (DWORD)(g*255.0f);
	col |= (G <<8);

	DWORD R = (DWORD)(r*255.0f);
	col |= (R<<16);

	DWORD A = (DWORD)(a/255.0f);
	col |= (A<<24);

	return col;
}
*/






///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// VECTOR2 :  2성분 벡터 연산자 오버로딩  
//
///////////////////////////////////////////////////////////////////////////////

B3YXVECTOR2 B3YXVECTOR2::operator + (B3YXVECTOR2 rhs)
{
	B3YXVECTOR2 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;

	return v;
}


B3YXVECTOR2 B3YXVECTOR2::operator - (B3YXVECTOR2 rhs)
{
	B3YXVECTOR2 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;

	return v;
}


B3YVECTOR2 B3YXVECTOR2::operator = (B3YXVECTOR2 rhs)
{
	B3YVECTOR2 v = { x, y };
	return v;
}


B3YXVECTOR2::operator B3YVECTOR2* ()
{
	return (B3YVECTOR2*)this;
}



///////////////////////////////////////////////////////////////////////////////
//
//! 2성분 외적 : 외적의 결과가 양수면, 'CCW' .Back-Face Culling 에 사용.
//!			   : D3DXVec2CCW 대응 (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec2ccw)
//!  
//! 2성분으로는 구조적으로 외적이 불가능하므로 3차원으로 확장 사용합니다. 
//!		v0=(x1,y1,0) ⓧ v1=(x2,y2,0). 
//! 또는 아래 수식을 참조하십시오.
//! \code  
//! [3성분 외적] 
//! VECTOR3 v;
//! v.x = v0.y * v1.z - v0.z * v1.y;
//! v.y = v0.z * v1.x - v0.x * v1.z;
//! v.z = v0.x * v1.y - v0.y * v1.x;
//! \endcode
//! 
//
float B3YXVec2CCW(B3YXVECTOR2* v0, B3YXVECTOR2* v1)	
{
	float z = v0->x * v1->y - v0->y * v1->x;
	return z;
}






///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//! ynFaceFill : 페이스 - 정점색으로 채우기
//! 
//! \param	hdc	출력 DC
//! \param	v0	정점 0번 좌표
//! \param	v1	정점 1번 좌표
//! \param	v2	정점 2번 좌표
//! \param	color	색상
//
void ynFaceFill(HDC hdc, B3YVECTOR2 v0, B3YVECTOR2 v1, B3YVECTOR2 v2, COLORREF color)
{
	//...
}




////////////////////////////////////////////////////////////////////////////////
//
//! ynFaceFill : 페이스 - 정점색으로 채우기 (과제)★
//! 
//! \param	hdc	출력 DC
//! \param	v0	정점 0번 좌표
//! \param	v1	정점 1번 좌표
//! \param	v2	정점 2번 좌표
//! \param	c0	정점 0번 색상
//! \param	c1	정점 1번 색상
//! \param	c2	정점 2번 색상
//
void ynFaceFill(HDC hdc, B3YVECTOR2 v0, B3YVECTOR2 v1, B3YVECTOR2 v2, COLOR c0, COLOR c1, COLOR c2)
{
	//<과제> ★★★
	// 정점 3개와 색상 3개를 이용해 삼각형에 색상을 채웁니다.★
	// ...
	//
	// 함수 시그니쳐(Signature : 인자/리턴값/이름) 의 변경없이,
	// 함수의 Body 를 완성 하십시요.★
	// ...
}




////////////////////////////////////////////////////////////////////////////////
// 
//! ynLerp : 선형보간 : 정수형 
//! 
//! \param[out]	v	보간된 값
//! \param		v0	시작 값
//! \param		v1	끝 값
//! \param		a	보간율 (alpha : 0.0~1.0)
//
void ynLerp(int* v, int v0, int v1, float a)
{
	//if (a < 0) a = 0;	if (a > 1) a = 1;
	a = min(max(a, 0), 1);					//0~1 제한

	*v = (int)((float)v0 * (1 - a) + (float)v1 * (a));
}


/*
///////////////////////////////////////////////////////////////////////////////
//
//! ynLerp 색상 선형보간.
//! DWORD 타입은 연산자 오버로딩 필요.
//!
//! \param[out]	c	보간된 색상
//! \param		c0	시작 색
//! \param		c1	끝 색
//! \param		a	보간율 (alpha : 0.0~1.0)
//
void ynLerp(DWORD* c, DWORD c0, DWORD c1, float a)
{
	//if (a < 0)  a = 0;	if (a > 1) a = 1;
	a = min(max(a, 0), 1);					//0~1 제한

	*c = c0 * (1 - a) + c1 * (a);			//<--경고 발생.. 이 수식이 가능하도록 연산자를 재정의(overloading) 하십시요.
}
*/



////////////////////////////////////////////////////////////////////////////////
//
//! ynLerp 색상 선형보간. ★
//! B3YXCOLOR 타입은 연산자 오버로딩 필요.
//! 
//! \param[out]	c	보간된 색상
//! \param		c0	시작 색
//! \param		c1	끝 색
//! \param		a	보간율 (alpha : 0.0~1.0)
//
void ynLerp(B3YXCOLOR* c, B3YXCOLOR c0, B3YXCOLOR c1, float a)  //★
{
	//if (a < 0) a = 0;	if (a > 1) a = 1;
	a = min(max(a, 0), 1);					//0~1 제한

	*c = c0 * (1 - a) + c1 * (a);			//<--경고 발생.. 이 수식이 가능하도록 연산자를 재정의(overloading) 하십시요.★★★
}




 



/************* end of file "ynMath.cpp" *******************************************/

 