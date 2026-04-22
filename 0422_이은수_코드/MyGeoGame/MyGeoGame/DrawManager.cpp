#include "DrawManager.h"
#include "ShapeBase.h"
#include <iostream>
#include "Circle.h"
#include "RectangleBox.h"
DrawManager::~DrawManager()
{
	for (auto geo : m_geos)
	{
		delete geo;
	}
	m_geos.clear();
}

void DrawManager::Draw(HDC hdc)
{
	for (int i = 0; i < m_geos.size(); ++i)
	{
		if (m_geos[i] == nullptr)
			continue;

		m_geos[i]->Draw(hdc);
	}
}

void DrawManager::Add(ShapeBase* geo)
{
	if (geo == nullptr) return; // 잘못된 데이터 추가 방지
	m_geos.push_back(geo);
}


bool IsCircleRectIntersect(Circle* c, RectangleBox* r) 
{
    // 사각형의 경계 구하기
    float rLeft = r->Pos.x - (r->GetWidth() / 2.0f);
    float rRight = r->Pos.x + (r->GetWidth() / 2.0f);
    float rTop = r->Pos.y - (r->GetHeight() / 2.0f);
    float rBottom = r->Pos.y + (r->GetHeight() / 2.0f);

    // 사각형 영역 내에서 원의 중심과 가장 가까운 점
    float closestX = std::max(rLeft, std::min(c->Pos.x, rRight));
    float closestY = std::max(rTop, std::min(c->Pos.y, rBottom));

    // 원의 중심과 해당 점 사이의 거리 계산
    float dx = c->Pos.x - closestX;
    float dy = c->Pos.y - closestY;
    float distanceSq = (dx * dx) + (dy * dy);

    return distanceSq <= (c->GetRadius() * c->GetRadius());
}


void DrawManager::CheckIntesectAndAdd(ShapeBase* geo)
{
    if (geo == nullptr) return;

    for (int i = (int)m_geos.size() - 1; i >= 0; --i)
    {
        ShapeBase* existing = m_geos[i];
        if (existing == nullptr) continue;

        bool isCollided = false;

        Circle* newC = dynamic_cast<Circle*>(geo);
        RectangleBox* oldR = dynamic_cast<RectangleBox*>(existing);
        if (newC && oldR) 
        {
            if (IsCircleRectIntersect(newC, oldR)) isCollided = true;
        }

        RectangleBox* newR = dynamic_cast<RectangleBox*>(geo);
        Circle* oldC = dynamic_cast<Circle*>(existing);
        if (newR && oldC) 
        {
            if (IsCircleRectIntersect(oldC, newR)) isCollided = true;
        }

        // 충돌했다면 기존 객체 제거
        if (isCollided)
        {
            delete existing;                
            m_geos.erase(m_geos.begin() + i); 
        }
    }
    m_geos.push_back(geo);
}

