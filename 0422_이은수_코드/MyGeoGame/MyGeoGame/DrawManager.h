#pragma once
#include <vector>
#include "INC_Windows.h"

class ShapeBase;

class DrawManager
{
public : 
	DrawManager() : m_geos() { }
	~DrawManager();

	void Draw(HDC hdc);
	void Add(ShapeBase* geo);
	void CheckIntesectAndAdd(ShapeBase* geo);
private :
	std::vector<ShapeBase*> m_geos;
};