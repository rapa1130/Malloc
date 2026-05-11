#include"CircirShape.h"

void CircleShape::Draw(HDC hdc) const
{
	HPEN hPen = CreatePen(PS_SOLID, 2, color);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}


bool CircleShape::IsCollide(ShapeBase* other) const
{
	switch (other->GetType())
	{
	case ShapeType::CircleType:
	{
		CircleShape* cir = (CircleShape*)other;
		int xDiff = centerX - cir->centerX;
		int yDiff = centerY - cir->centerY;
		int distSquared = xDiff * xDiff + yDiff * yDiff;
		return distSquared <= (radius + cir->radius) * (radius + cir->radius);
	}
	break;
	case ShapeType::RectangleType:
	{
		RectangleShape* rect = (RectangleShape*)other;
		int cenX = (rect->left + rect->right) / 2;
		int cenY = (rect->top + rect->bottom) / 2;
		int xDiff = abs(cenX - centerX);
		int yDiff = abs(cenY - centerY);

		double ratio = (xDiff > yDiff) ? (double)(rect->right - cenX) / xDiff : (double)(rect->bottom - cenY) / yDiff;

		double distBetweenCenters = sqrt(xDiff * xDiff + yDiff * yDiff);
		double distInRect = distBetweenCenters * ratio;

		return distInRect + radius >= distBetweenCenters;
	}break;

	default:
		break;
	}
	return true;
}

ShapeType CircleShape::GetType() const
{
	return ShapeType::CircleType;
}

CircleShape::CircleShape(int centerX, int centerY, int radius, COLORREF color)
	:centerX(centerX), centerY(centerY), radius(radius), color(color)
{
}