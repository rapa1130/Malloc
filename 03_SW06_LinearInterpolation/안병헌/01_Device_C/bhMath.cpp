#include"bhMath.h"



float B3BXVec2Cross(B3BXVECTOR2* v0, B3BXVECTOR2* v1)
{
	return v0->x * v1->y - v0->y * v1->x;
}

void bhLerp(int* v, int v0, int v1, float a)
{
	a = min(max(a, 0), 1);
	*v = v0 * (1.0f-a) + v1 * a;
}

void bhLerp(DWORD* v, DWORD v0, DWORD v1, float a)
{
	a = min(max(a, 0), 1);

	BYTE a0 = (v0 >> 24) & 0xff;
	BYTE r0 = (v0 >> 16) & 0xff;
	BYTE g0 = (v0 >> 8) & 0xff;
	BYTE b0 = (v0 >> 0) & 0xff;

	BYTE a1 = (v1 >> 24) & 0xff;
	BYTE r1 = (v1 >> 16) & 0xff;
	BYTE g1 = (v1 >> 8) & 0xff;
	BYTE b1 = (v1 >> 0) & 0xff;

	BYTE aByte = a0 * (1.0f - a) + a1 * a;
	BYTE rByte = r0 * (1.0f - a) + r1 * a;
	BYTE gByte = g0  * (1.0f - a) + g1  * a;
	BYTE bByte = b0 * (1.0f - a) + b1 * a;

	*v = (aByte << 24) | (rByte << 16) | (gByte << 8) | bByte;
}

void bhLerp(DWORD* v, DWORD v0, DWORD v1, DWORD v2, float alpha, float beta, float gamma)
{
	BYTE a0 = (v0 >> 24) & 0xff;
	BYTE r0 = (v0 >> 16) & 0xff;
	BYTE g0 = (v0 >> 8) & 0xff;
	BYTE b0 = (v0 >> 0) & 0xff;

	BYTE a1 = (v1 >> 24) & 0xff;
	BYTE r1 = (v1 >> 16) & 0xff;
	BYTE g1 = (v1 >> 8) & 0xff;
	BYTE b1 = (v1 >> 0) & 0xff;

	BYTE a2 = (v2 >> 24) & 0xff;
	BYTE r2 = (v2 >> 16) & 0xff;
	BYTE g2 = (v2 >> 8) & 0xff;
	BYTE b2 = (v2 >> 0) & 0xff;

	BYTE aByte = a0 * alpha + a1 * beta + a2 * gamma;
	BYTE rByte = r0 * alpha + r1 * beta + r2 * gamma;
	BYTE gByte = g0 * alpha + g1 * beta + g2 * gamma;
	BYTE bByte = b0 * alpha + b1 * beta + b2 * gamma;

	*v = (aByte << 24) | (rByte << 16) | (gByte << 8) | (bByte >> 0);
}

void bhLerp(B3BXCOLOR* v, B3BXCOLOR v0, B3BXCOLOR v1, B3BXCOLOR v2, float alpha, float beta, float gamma)
{
	v->a = (v0.a * alpha + v1.a * beta + v2.a * gamma);
	v->r = (v0.r * alpha + v1.r * beta + v2.r * gamma);
	v->g = (v0.g * alpha + v1.g * beta + v2.g * gamma);
	v->b = (v0.b * alpha + v1.b * beta + v2.b * gamma);
}


void bhLerp(B3BXCOLOR* c, B3BXCOLOR c0, B3BXCOLOR c1, float a)
{
	a = min(max(a, 0), 1);					
	*c = c0 * (1 - a) + c1 * (a);
}

B3BXVECTOR2 B3BXVECTOR2::operator+(B3BXVECTOR2 rhs)
{
	B3BXVECTOR2 v;
	v.x = this->x + rhs.x;
	v.y = this->y + rhs.y;
	return v;
}

B3BXVECTOR2 B3BXVECTOR2::operator-(B3BXVECTOR2 rhs)
{
	B3BXVECTOR2 v;
	v.x = this->x - rhs.x;
	v.y = this->y - rhs.y;
	return v;
}

B3BXVECTOR2 B3BXVECTOR2::operator=(B3BXVECTOR2 rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	return *this;
}

B3BXVECTOR2::operator B3BXVECTOR2* ()
{
	return (B3BXVECTOR2*)this;
}

B3BXCOLOR::B3BXCOLOR(DWORD col)
{
	a = ((col >> 24)  & 0xff) / 255.0f;
	r = ((col >> 0)   & 0xff) / 255.0f;
	g = ((col >> 8)   & 0xff) / 255.0f;
	b = ((col >> 16)  & 0xff) / 255.0f;
}

B3BXCOLOR B3BXCOLOR::operator*(B3BXCOLOR rhs)
{
	B3BXCOLOR v;
	v.a = this->a * rhs.a;
	v.r = this->r * rhs.r;
	v.g = this->g * rhs.g;
	v.b = this->b * rhs.b;
	return v;
}

B3BXCOLOR B3BXCOLOR::operator+(B3BXCOLOR rhs)
{
	B3BXCOLOR v;
	v.a = this->a + rhs.a;
	v.r = this->r + rhs.r;
	v.g = this->g + rhs.g;
	v.b = this->b + rhs.b;
	return v;
}

B3BXCOLOR B3BXCOLOR::operator-(B3BXCOLOR rhs)
{
	B3BXCOLOR v;
	v.a = this->a - rhs.a;
	v.r = this->r - rhs.r;
	v.g = this->g - rhs.g;
	v.b = this->b - rhs.b;
	return v;
}

B3BXCOLOR B3BXCOLOR::operator*(float f)
{
	B3BXCOLOR v;
	v.a = this->a * f;
	v.r = this->r * f;
	v.g = this->g * f;
	v.b = this->b * f;
	return v;
}

B3BXCOLOR::operator DWORD()
{
	COLORREF col;

	BYTE rByte = (BYTE)(r * 255.0f);
	BYTE gByte = (BYTE)(g * 255.0f);
	BYTE bByte = (BYTE)(b * 255.0f);
	BYTE aByte = (BYTE)(a * 255.0f);
	
	col = RGB(rByte, gByte, bByte);
	return col;
	//return (aByte << 24) | (bByte << 16) | (gByte << 8) | (rByte << 0);
}
