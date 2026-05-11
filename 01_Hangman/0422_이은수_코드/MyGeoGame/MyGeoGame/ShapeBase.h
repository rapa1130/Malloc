#pragma once
#include "Vector2.h"
#include "INC_Windows.h"

class ShapeBase
{
public :
	ShapeBase() : Pos() { }
	ShapeBase(Vector2 pos) : Pos(pos) { }
	virtual ~ShapeBase() = default;

	virtual void Draw(HDC hdc) const = 0;
public :
	Vector2 Pos;
};