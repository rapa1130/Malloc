#pragma once
#include "INC_Windows.h"

namespace simplegeo
{
    class ShapeBase
    {
    public:
        virtual void Draw(HDC hdc) = 0;
        virtual bool IsInside(int x, int y) = 0;
        virtual ~ShapeBase() {}
    };

    class Circle : public ShapeBase
    {
    public:
        Circle(int centerX, int centerY, int radius, COLORREF color)
            : m_centerX(centerX), m_centerY(centerY), m_radius(radius), m_color(color) {
        }

        bool IsInside(int x, int y) override {
            int dx = x - m_centerX;
            int dy = y - m_centerY;
            return (dx * dx + dy * dy) <= (m_radius * m_radius);
        } // 마우스가 들어옴을 판별 원에서

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

        bool IsInside(int x, int y) override {
            return x >= m_left && x <= m_right &&
                y >= m_top && y <= m_bottom;
        } 

        // 마우스가 들어옴을 판별 네모에서


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

        bool IsInside(int x, int y) override {
            return false;
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

        int FindShapeIndex(int x, int y) {
            for (int i = 0; i < m_shapeCount; i++) 
            {
                if (m_shapes[i] && m_shapes[i]->IsInside(x, y))
                {
                    return i;
                }
            }
            return -1;
        }

        void RemoveShape(int index) {
            if (index < 0 || index >= m_shapeCount) return;
            delete m_shapes[index];


            for (int i = index; i < m_shapeCount - 1; i++) {
                m_shapes[i] = m_shapes[i + 1];
            }
            m_shapes[m_shapeCount - 1] = nullptr;
            m_shapeCount--;
        }


    private:
        ShapeBase* m_shapes[MAX_SHAPES] = { nullptr, };
        int m_shapeCount = 0;
    };

    extern GeoShapeManager g_GeoShapeManager;
}