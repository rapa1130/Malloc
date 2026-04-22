#include "RectangleBox.h"

void RectangleBox::Draw(HDC hdc) const
{
    // 1. 펜 생성 및 설정 (파란색 사각형 예시)
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // 내부를 채우지 않으려면 NULL_BRUSH 사용
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // 2. 사각형 그리기
    // 중심점(Pos)을 기준으로 가로(m_width)와 세로(m_height)만큼 펼쳐서 그립니다.
    int left = (int)(Pos.x - (m_width / 2.0f));
    int top = (int)(Pos.y - (m_height / 2.0f));
    int right = (int)(Pos.x + (m_width / 2.0f));
    int bottom = (int)(Pos.y + (m_height / 2.0f));

    ::Rectangle(hdc, left, top, right, bottom);

    // 3. GDI 자원 복원 및 해제
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}