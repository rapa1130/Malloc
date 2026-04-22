#pragma once
#include"ShapeBase.h"
#include"RectangleShape.h"
#include"CircirShape.h"
#include"Line.h"
#include"INC_Windows.h"
#include<iostream>
#define MAX_SHAPE_COUNT 100

namespace simpleGeo
{
	class GeoShapeManager
	{
	public:
		GeoShapeManager();
		void AddCircle(int centerX, int centerY, int radius, COLORREF color);
		void AddRectangle(int left, int right, int top, int bottom, COLORREF color);
		void AddLine(int startX, int startY, int endX, int endY, COLORREF color);
		void Draw(HDC hdc);
		~GeoShapeManager();

	private:
		void _addShape(ShapeType type, ShapeBase* shape);

		ShapeBase* shapes[MAX_SHAPE_COUNT] = { nullptr, };
		int shapeCount;
	};

	extern GeoShapeManager g_geoShapeManager;
}
