#include"GeoShapeManager.h"




namespace simpleGeo
{
	GeoShapeManager g_geoShapeManager;

	GeoShapeManager::GeoShapeManager() :shapeCount(0) {}

	void GeoShapeManager::AddCircle(int centerX, int centerY, int radius, COLORREF color)
	{
		_addShape(ShapeType::CircleType, new CircleShape(centerX, centerY, radius, color));
	}

	void GeoShapeManager::AddRectangle(int left, int right, int top, int bottom, COLORREF color)
	{
		_addShape(ShapeType::RectangleType, new RectangleShape(left, right, top, bottom, color));
	}

	void GeoShapeManager::AddLine(int startX, int startY, int endX, int endY, COLORREF color)
	{
		_addShape(ShapeType::LineType, new Line(startX, startY, endX, endY, color));
	}

	void GeoShapeManager::Draw(HDC hdc)
	{
		for (int i = 0; i < shapeCount; i++)
		{
			if (shapes[i] == nullptr) break;
			shapes[i]->Draw(hdc);
		}
	}
	GeoShapeManager::~GeoShapeManager()
	{
		for (int i = 0; i < shapeCount; i++)
		{
			delete shapes[i];
			shapes[i] = nullptr;
		}
	}

	void GeoShapeManager::_addShape(ShapeType type, ShapeBase* shape)
	{
		if (shapeCount >= MAX_SHAPE_COUNT) return;
		for (int i = 0; i < shapeCount; i++)
		{
			if (shape->IsCollide(shapes[i]))
			{
				std::cout << "Collide!~" << std::endl;
				delete shapes[i];
				shapes[i] = shapes[shapeCount - 1];
				shapes[shapeCount - 1] = nullptr;
				shapeCount--;
				i--;
			}
		}
		switch (type)
		{
		case CircleType:
			shapes[shapeCount] = (CircleShape*)shape;
			break;
		case RectangleType:
			shapes[shapeCount] = (RectangleShape*)shape;
			break;
		case LineType:
			shapes[shapeCount] = (Line*)shape;
			break;
		default:
			break;
		}
		shapeCount++;
	}
}