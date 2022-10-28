#include "framework.h"
#include "PaintScene.h"

PaintScene::PaintScene()
{
    _brushes[0] = CreateSolidBrush(RED); // ����
    _brushes[1] = CreateSolidBrush(GREEN); // �ʷ�
    _brushes[2] = CreateSolidBrush(BLUE); // �Ķ�

    _pens[0] = CreatePen(0, 3, RED);
    _pens[1] = CreatePen(0, 3, GREEN);
    _pens[2] = CreatePen(0, 3, BLUE);

    _rect1 = make_shared<RectCollider>(Vector2(225, 255), Vector2(150, 150));
    _rect2 = make_shared<RectCollider>(Vector2(600, 255), Vector2(150, 150));

    _circle1 = make_shared<CircleCollider>(Vector2(100.0f, 600.0f), 50);
    _circle2 = make_shared<CircleCollider>(Vector2(400.0f, 600.0f), 50);
    _circle3 = make_shared<CircleCollider>(Vector2(700.0f, 600.0f), 50);
}

PaintScene::~PaintScene()
{
}

void PaintScene::Update()
{
    // _rect->GetCenter()._x += 0.5f;
    // 
    // Vector2 start = _elli->GetCenter();
    // Vector2 dest = mousePos;
    // Vector2 result = dest - start;
    // Vector2 normal = result.Normalize();
    // 
    // _elli->GetCenter() = LERP(start,dest,0.01f);

    // ��������

    _circle3->SetCenter(mousePos);

    if (_rect1->IsCollision(mousePos))
        _rect1->SetRED();
    else
        _rect1->SetGREEN();

    if (_rect2->IsCollision(_circle3))
        _rect2->SetRED();
    else
        _rect2->SetGREEN();

    if (_circle1->IsCollision(mousePos))
        _circle1->SetRED();
    else
        _circle1->SetGREEN();

    if (_circle2->IsCollision(_circle3))
        _circle2->SetRED();
    else
        _circle2->SetGREEN();

}

void PaintScene::Render(HDC hdc)
{
    _circle3->Render(hdc);

    _rect1->Render(hdc);
    _rect2->Render(hdc);

    _circle1->Render(hdc);
    _circle2->Render(hdc);

}