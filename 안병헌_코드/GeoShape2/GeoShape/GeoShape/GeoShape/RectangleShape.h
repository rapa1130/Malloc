#pragma once
#include"ShapeBase.h"
#include"INC_Windows.h"
#include<iostream>
#include"CircirShape.h"

class RectangleShape : public ShapeBase
{
	friend class CircleShape;
public:
	RectangleShape(int left, int right, int top, int bottom, COLORREF color);
	virtual void Draw(HDC hdc) const  override;
	virtual bool IsCollide(ShapeBase* other) const override;
	virtual ShapeType GetType() const override;
	
private:
	int left;
	int right;
	int top;
	int bottom;
	COLORREF color;
};
