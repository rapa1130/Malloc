#pragma once
#include"ShapeBase.h"
#include"INC_Windows.h"
#include"RectangleShape.h"

class CircleShape :public ShapeBase
{
	friend class RectangleShape;
public:
	CircleShape(int centerX, int centerY, int radius, COLORREF color);
	virtual void Draw(HDC hdc) const override;
	virtual bool IsCollide(ShapeBase* other) const override;
	virtual ShapeType GetType() const override;
private:
	int centerX;
	int centerY;
	int radius;
	COLORREF color;
};