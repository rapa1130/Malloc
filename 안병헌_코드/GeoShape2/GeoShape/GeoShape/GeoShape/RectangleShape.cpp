#include"RectangleShape.h"

RectangleShape::RectangleShape(int left, int right, int top, int bottom, COLORREF color)
	:left(left), right(right), top(top), bottom(bottom), color(color)
{
}

void RectangleShape::Draw(HDC hdc) const 
{
	HPEN hPen = CreatePen(PS_SOLID, 2, color);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Rectangle(hdc, left, top, right, bottom);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}

bool RectangleShape::IsCollide(ShapeBase* other) const
{
	switch (other->GetType())
	{
	case ShapeType::CircleType:
	{
		CircleShape* cir = (CircleShape*)other;
		int cenX = (left + right) / 2;
		int cenY = (top + bottom) / 2;
		int xDiff = abs(cenX - cir->centerX);
		int yDiff = abs(cenY - cir->centerY);

		double ratio = (xDiff > yDiff) ? (double)(right - cenX) / xDiff : (double)(bottom - cenY) / yDiff;

		double distBetweenCenters = sqrt(xDiff * xDiff + yDiff * yDiff);
		double distInRect = distBetweenCenters * ratio;

		return distInRect + cir->radius >= distBetweenCenters;
	}break;
	case ShapeType::RectangleType:
	{
		RectangleShape* rec = (RectangleShape*)other;
		bool xAxisOverlapped = (left >= rec->left && left <= rec->right) || (right >= rec->left && right <= rec->right);
		bool yAxisOverlapped = (top >= rec->top && top <= rec->bottom) || (bottom >= rec->top && bottom <= rec->bottom);
		return xAxisOverlapped && yAxisOverlapped;
	}break;
	default:
		break;
	}
}

ShapeType RectangleShape::GetType() const
{
	return ShapeType::RectangleType;
}