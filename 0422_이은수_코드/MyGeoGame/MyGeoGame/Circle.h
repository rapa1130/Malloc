#pragma once
#include "ShapeBase.h"
class Circle : public ShapeBase
{
public :
	Circle(Vector2 pos, float radius) : ShapeBase(pos), m_radius(radius) { }
	Circle(float radius) : ShapeBase(), m_radius(radius) { }

	float GetRadius() { return m_radius; }
	void Draw(HDC hdc) const override;
private:
	float m_radius;
};