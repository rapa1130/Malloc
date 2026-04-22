#pragma once
#include "INC_Windows.h"
#include "Enum.h"
#include <algorithm>

namespace simplegeo
{
    class ShapeBase
    {
    public:
        virtual void Draw(HDC hdc) = 0;
        virtual ~ShapeBase() {}
    };

    class Circle : public ShapeBase
    {
    public:
        Circle(int centerX, int centerY, int radius, COLORREF color)
            : m_centerX(centerX), m_centerY(centerY), m_radius(radius), m_color(color) {
        }

        void Draw(HDC hdc) override
        {
            // 펜 생성 및 선택
            HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            // 브러시는 내부 채우지 않도록 NULL_BRUSH 사용
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

            Ellipse(hdc,
                m_centerX - m_radius, m_centerY - m_radius,
                m_centerX + m_radius, m_centerY + m_radius);

            // 이전 객체 복원 및 펜 삭제
            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hPen);
        }

		int GetCenterX() const { return m_centerX; }
		int GetCenterY() const { return m_centerY; }
		int GetRadius() const { return m_radius; } 

    private:
        int m_centerX, m_centerY, m_radius;
        COLORREF m_color;
    };


    class RectangleShape : public ShapeBase
    {
    public:
        RectangleShape(int left, int top, int right, int bottom, COLORREF color)
            : m_left(left), m_top(top), m_right(right), m_bottom(bottom), m_color(color) {
        }

        void Draw(HDC hdc) override
        {
            HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

            Rectangle(hdc, m_left, m_top, m_right, m_bottom);

            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hPen);
        }

		int GetLeft() const { return m_left; }
		int GetTop() const { return m_top; }
		int GetRight() const { return m_right; }
		int GetBottom() const { return m_bottom; }

    private:
        int m_left, m_top, m_right, m_bottom;
        COLORREF m_color;
    };


    class Line : public ShapeBase
    {
    public:
        Line(int startX, int startY, int endX, int endY, COLORREF color)
            : m_startX(startX), m_startY(startY), m_endX(endX), m_endY(endY), m_color(color) {
        }

        void Draw(HDC hdc) override
        {
            HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

            MoveToEx(hdc, m_startX, m_startY, NULL);
            LineTo(hdc, m_endX, m_endY);

            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
        }
    private:
        int m_startX, m_startY, m_endX, m_endY;
        COLORREF m_color;
    };

    constexpr int MAX_SHAPES = 100; // 최대 도형 개수
    class GeoShapeManager
    {
    public:
        GeoShapeManager() = default;
        ~GeoShapeManager()
        {
            for (int i = 0; i < MAX_SHAPES; ++i)
            {
                if (m_shapes[i] != nullptr)
                {
                    delete m_shapes[i];
                    m_shapes[i] = nullptr;
                }
            }
        }

        void AddCircle(int centerX, int centerY, int radius, COLORREF color)
        {
            if (m_shapeCount >= MAX_SHAPES) return;

            m_shapes[m_shapeCount] = new Circle(centerX, centerY, radius, color);
            ++m_shapeCount;
        }

        void AddRectangle(int left, int top, int right, int bottom, COLORREF color)
        {
            if (m_shapeCount >= MAX_SHAPES) return;

            m_shapes[m_shapeCount] = new RectangleShape(left, top, right, bottom, color);
            ++m_shapeCount;
        }

        void AddLine(int startX, int startY, int endX, int endY, COLORREF color)
        {
            if (m_shapeCount >= MAX_SHAPES) return;

            m_shapes[m_shapeCount] = new Line(startX, startY, endX, endY, color);
            ++m_shapeCount;
        }

        void Draw(HDC hdc)
        {
            for (int i = 0; i < MAX_SHAPES; ++i)
            {
                if (m_shapes[i] == nullptr) break;

                m_shapes[i]->Draw(hdc);
            }
        }

        void RemoveShape(int index)
        {
            delete m_shapes[index];

            m_shapes[index] = m_shapes[m_shapeCount - 1];
            m_shapes[m_shapeCount - 1] = nullptr;

            m_shapeCount--;
        }


		//마우스 위치에 그리는 도형이 다른 도형과 겹치는지 판별하여 삭제
        bool IsOverlapping(int x, int y, ShapeType shapetype)
        {
            bool isOverlapping = false;
            for (int i = m_shapeCount - 1; i >= 0 ; --i)
            {
                Circle* circle = dynamic_cast<Circle*>(m_shapes[i]);
                if (circle != nullptr)
                {
                    int centerX = circle->GetCenterX();
                    int centerY = circle->GetCenterY();
                    int radius = circle->GetRadius();
                    switch (shapetype)
                    {
                    case ShapeType::Circle:
                    {
                        if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= (radius + radius) * (radius + radius))
                        {
                            RemoveShape(i);
                            isOverlapping = true;
                        }
                        break;
                    }
                    case ShapeType::Rectangle:
                    {
                        int closetX = std::max(x - 10, std::min(centerX, x + 10));
                        int closetY = std::max(y - 10, std::min(centerY, y + 10));
                        if ((centerX - closetX) * (centerX - closetX) + (centerY - closetY) * (centerY - closetY) <= (radius * radius))
                        {
                            RemoveShape(i);
                            isOverlapping = true;
                        }
                        break;
                    }
                    }
                }
                RectangleShape* rect = dynamic_cast<RectangleShape*>(m_shapes[i]);
                if (rect != nullptr)
                {
                    int left = rect->GetLeft();
                    int top = rect->GetTop();
                    int right = rect->GetRight();
                    int bottom = rect->GetBottom();
                    switch (shapetype)
                    {
                    case ShapeType::Circle:
                    {
                        int radius = 10;

                        int closestX = std::max(left, std::min(x, right));
                        int closestY = std::max(top, std::min(y, bottom));

                        int dx = x - closestX;
                        int dy = y - closestY;

                        if (dx * dx + dy * dy <= radius * radius)
                        {
                            RemoveShape(i);
                            isOverlapping = true;
                        }
                        break;
                    }
                    case ShapeType::Rectangle:
                    {
                        // 새로 생성될 사각형
                        int newLeft = x - 10;
                        int newTop = y - 10;
                        int newRight = x + 10;
                        int newBottom = y + 10;

                        // 겹치지 않는 경우 4가지
                        bool isoverap = (newRight < left || newLeft > right || newBottom < top || newTop > bottom);
                        if (!isoverap)
                        {
                            RemoveShape(i);
                            isOverlapping = true;
                        }
                        break;
                    }
                    }
                }
            }
            return isOverlapping;
        }
        //마우스 클릭 위치가 도형의 안인지 판별하여 삭제
        bool IsOverlapping(int x, int y)
        {
            for (int i = 0; i < m_shapeCount; ++i)
            {
                Circle* circle = dynamic_cast<Circle*>(m_shapes[i]);
                if (circle != nullptr)
                {
                    int centerX = circle->GetCenterX();
                    int centerY = circle->GetCenterY();
                    int radius = circle->GetRadius();
                    if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius)
                    {
                        RemoveShape(i);
                        return true;
                    }
                }
                RectangleShape* rect = dynamic_cast<RectangleShape*>(m_shapes[i]);
                if (rect != nullptr)
                {
                    int left = rect->GetLeft();
                    int top = rect->GetTop();
                    int right = rect->GetRight();
                    int bottom = rect->GetBottom();
                    if (x >= left && x <= right && y >= top && y <= bottom)
                    {
                        RemoveShape(i);
                        return true;
                    }
                }
            }
            return false;
        }

    private:
        ShapeBase* m_shapes[MAX_SHAPES] = { nullptr, };
        int m_shapeCount = 0;
    };

    extern GeoShapeManager g_GeoShapeManager;
}