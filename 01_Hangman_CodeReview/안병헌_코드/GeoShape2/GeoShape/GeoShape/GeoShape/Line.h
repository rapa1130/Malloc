#pragma once
#include"INC_Windows.h"
#include"ShapeBase.h"

class Line :public ShapeBase
{
public:
	Line(int startX,int startY,int endX,int endY,COLORREF color)
		:startX(startX),startY(startY),endX(endX),endY(endY),color(color)
	{ }

	virtual void Draw(HDC hdc) const  override
	{
		HPEN hPen = CreatePen(PS_SOLID, 2, color);
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		
		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, endX, endY);

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hPen);
	}

	virtual bool IsCollide(ShapeBase* other) const
	{
		return false;
	}

	virtual ShapeType GetType() const override
	{
		return ShapeType::LineType;
	}
private:
	int startX;
	int startY;
	int endX;
	int endY;
	COLORREF color;
};