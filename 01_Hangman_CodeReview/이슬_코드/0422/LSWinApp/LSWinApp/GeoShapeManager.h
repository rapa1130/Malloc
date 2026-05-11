#pragma once
#include "INC_Windows.h"
#include <math.h>

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

        int GetX() { return m_centerX;}
        int GetY() { return m_centerY; }
        int GetR() { return m_radius; }


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

        int GetL() { return m_left; }
        int GetT() { return m_top; }
        int GetR() { return m_right; }
        int GetB() { return m_bottom; }

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

        int GetSX() { return m_startX; }
        int GetSY() { return m_startY; }
        int GetEX() { return m_endX; }
        int GetEY() { return m_endY; }

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


            // 추가한 코드
            for (int i = 0; i < m_shapeCount; i++)
            {
                //원이랑 비교
                Circle* circle = dynamic_cast<Circle*>(m_shapes[i]);
                if (circle)
                {
                    if (sqrt(pow(centerX - circle->GetX(), 2) + pow(centerY - circle->GetY(), 2)) < radius + circle->GetR())
                    {
                        delete m_shapes[i];
                        m_shapes[i] = nullptr;

                        for (int j = i; j < m_shapeCount-1; j++)
                        {
                            m_shapes[j] = m_shapes[j + 1];
                        }
                        m_shapeCount--;
                        break;
                    }
                }

                //상자랑 비교
                RectangleShape* rectangle = dynamic_cast<RectangleShape*>(m_shapes[i]);
                if (rectangle)
                {
                    int CX = (rectangle->GetL() + rectangle->GetR()) / 2;
                    int CY = (rectangle->GetT() + rectangle->GetB()) / 2;
                    if (abs(CX - centerX) < (10 + radius) && abs(CY - centerY) < (10 + radius))
                    {
                        delete m_shapes[i];
                        m_shapes[i] = nullptr;

                        for (int j = i; j < m_shapeCount - 1; j++)
                        {
                            m_shapes[j] = m_shapes[j + 1];
                        }
                        m_shapeCount--;
                        break;
                    }
                }
            }

            m_shapes[m_shapeCount] = new Circle(centerX, centerY, radius, color);
            ++m_shapeCount;
        }

        void AddRectangle(int left, int top, int right, int bottom, COLORREF color)
        {
            if (m_shapeCount >= MAX_SHAPES) return;


            // 추가한 코드
            for (int i = 0; i < m_shapeCount; i++)
            {
                //상자 중심 = 마우스 커서 위치
                int CX = (left + right) / 2;
                int CY = (top + bottom) / 2;

                //상자랑 비교
                RectangleShape* rectangle = dynamic_cast<RectangleShape*>(m_shapes[i]);
                if (rectangle)
                {
                    int RX = (rectangle->GetL() + rectangle->GetR()) / 2;
                    int RY = (rectangle->GetT() + rectangle->GetB()) / 2;

                    if (abs(CX- RX)<20 && abs(CY - RY) < 20)
                    {
                        delete m_shapes[i];
                        m_shapes[i] = nullptr;

                        for (int j = i; j < m_shapeCount - 1; j++)
                        {
                            m_shapes[j] = m_shapes[j + 1];
                        }
                        m_shapeCount--;
                        break;
                    }
                }

                //원이랑 비교
                Circle* circle = dynamic_cast<Circle*>(m_shapes[i]);
                if (circle)
                {
                    if (abs(CX - circle->GetX()) < (10 + circle->GetR()) && abs(CY - circle->GetY()) < (10 + circle->GetR()))
                    {
                        delete m_shapes[i];
                        m_shapes[i] = nullptr;

                        for (int j = i; j < m_shapeCount - 1; j++)
                        {
                            m_shapes[j] = m_shapes[j + 1];
                        }
                        m_shapeCount--;
                        break;
                    }
                }
            }

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

    private:
        ShapeBase* m_shapes[MAX_SHAPES] = { nullptr, };
        int m_shapeCount = 0;
    };

    extern GeoShapeManager g_GeoShapeManager;
}

