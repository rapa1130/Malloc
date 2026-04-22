#include "Circle.h"
#include "RectangleBox.h"

void Circle::Draw(HDC hdc) const
{
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	
	Ellipse(hdc, Pos.x - m_radius, 
		Pos.y - m_radius,
		Pos.x + m_radius,
		Pos.y + m_radius);

	// 이전 객체 복원 및 펜 삭제
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}