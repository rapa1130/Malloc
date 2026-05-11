#pragma once
#include "ShapeBase.h"
class RectangleBox : public ShapeBase
{
public:
	RectangleBox(Vector2 pos, float w, float h) : ShapeBase(pos), m_width(w), m_height(h) {}

	float GetWidth() { return m_width; }
	float GetHeight() { return m_height; }
	void Draw(HDC hdc) const override;
private:
	float m_width;
	float m_height;
};