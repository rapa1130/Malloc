#pragma once
#include"INC_Windows.h"

enum ShapeType
{
	CircleType, RectangleType, LineType
};

class ShapeBase
{
public:
	virtual void Draw (HDC hdc) const = 0;
	virtual ~ShapeBase() {}
	virtual bool IsCollide(ShapeBase* other) const = 0;
	virtual ShapeType GetType() const = 0;
};