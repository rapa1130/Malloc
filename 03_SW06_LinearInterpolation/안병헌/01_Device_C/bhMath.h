#pragma once

#include<Windows.h>

struct B3BVECTOR2
{
	float x, y;
};

struct B3BVECTOR3
{
	float x, y, z;
};

struct B3BVECTOR4
{
	float x, y, z, w;
};



class B3BXVECTOR2 : public B3BVECTOR2
{
public:
	B3BXVECTOR2() { x = 0; y = 0; }
	B3BXVECTOR2(float _x, float _y) { x = _x; y = _y; }
	B3BXVECTOR2(const B3BVECTOR2& v){ x = v.x; y = v.y; }

	B3BXVECTOR2 operator+(B3BXVECTOR2 rhs);
	B3BXVECTOR2 operator-(B3BXVECTOR2 rhs);
	B3BXVECTOR2 operator=(B3BXVECTOR2 rhs);
	operator B3BXVECTOR2* ();

	operator POINT() { return POINT{ (LONG)x,(LONG)y }; }
};
typedef B3BXVECTOR2 VECTOR2;

float B3BXVec2Cross(B3BXVECTOR2* v0, B3BXVECTOR2* v1);



class B3BXVECTOR3 : public B3BVECTOR3
{
public:
	B3BXVECTOR3() { x = 0; y = 0; z = 0; }
	B3BXVECTOR3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	B3BXVECTOR3(const B3BVECTOR3& v) { x = v.x; y = v.y; z = v.y; }

};
typedef B3BXVECTOR3 VECTOR3;





class B3BXVECTOR4 : public B3BVECTOR4
{
public:
	B3BXVECTOR4() { x = 0; y = 0; z = 0; w = 0; }
	B3BXVECTOR4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
	B3BXVECTOR4(const B3BVECTOR4& v) { x = v.x; y = v.y; z = v.y; w = v.w; }
};
typedef B3BXVECTOR4 VECTOR4;






struct B3BCOLOR
{
	float r, g, b, a;
};

class B3BXCOLOR : public B3BCOLOR
{
public:
	B3BXCOLOR() { r = 0; g = 0; b = 0; a = 1.0f; }
	B3BXCOLOR(float _r, float _g, float _b, float _a) { r = _r; g = _g; b = _b; a = _a; }

	B3BXCOLOR(DWORD col);

	B3BXCOLOR operator*(B3BXCOLOR rhs);
	B3BXCOLOR operator+(B3BXCOLOR rhs);
	B3BXCOLOR operator-(B3BXCOLOR rhs);
	B3BXCOLOR operator*(float f);
	operator DWORD();
};
typedef B3BXCOLOR COLOR;

#define TOCOLOR(col) (*(COLOR*)&(col))

//lerp Color
void bhLerp(int* v, int v0, int v1, float a);
void bhLerp(DWORD* v, DWORD v0, DWORD v1, float a);
void bhLerp(DWORD* v, DWORD v0, DWORD v1, DWORD v2, float alpha, float beta, float gamma);
void bhLerp(B3BXCOLOR* v, B3BXCOLOR v0, B3BXCOLOR v1, B3BXCOLOR v2, float alpha, float beta, float gamma);
void bhLerp(B3BXCOLOR* c, B3BXCOLOR c0, B3BXCOLOR c1, float a);





