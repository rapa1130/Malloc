
[ 04_Face(User) ]
DrawLine 을 3가지로 구현함
DrawLine 함수들만 보시면 될것 같습니다!

1 -> DrawLineLineFunc () 
    : 직선 방정식으로 매번 새로 x,y좌표 계산
2 -> DrawLineDDA ()
    : x,y중 해당하는 좌표 축에 기울기값을 더해 계산
3 -> DrawLineBresenhum ()
    : 브레젠험 알고리즘을 구현함. 
      MidPoint값으로 비교, midPoint에 대한 비교 후, x,y중 해당하는 좌표를 증감하여 계산
    : 8개의 분기로 나뉘어 같은 함수가 처리되고 있으니 참고바랍니다!

[ 05_Face_Fill ]
FaceFill 을 3가지로 구현함
FaceFill 함수들만 보시면 될 것 같습니다.

1,2번 방식 모두 middle Point까지

1-> FillFace
    :매번 직선 방정식을 계산해서 x,y좌표를 구해 
2-> FillFaceImprove
    :현재 교점 좌표에 기울기값만큼 더해 다음 교점 좌표를 계산
3-> FillFaceBoundingBox
    :삼각형외각을 채우는 사각형의 범위를 그리는데, 각 직선과의 연산을 통해 내부인지 판정하여 그릴지 말지 결정하는



※아래는 스터디에서 다루지 않습니다! 참고하고 싶은 분만 보세요

[ 06_Line_Intersection ]


[ 07_Line_Intersection_Slope ]