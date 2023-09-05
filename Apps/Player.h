#pragma once
#include "Vector2.h"
#include "Stage.h"

class Player final
{
public:
    // �萔
    float kMoveSpeed_{3.f};

    // �֐�
    Player(Stage* stagePtr) :stagePtr_(stagePtr) {}
    ~Player(void) = default;

    void Update(void);
    void Draw(void);

private:
    // �ϐ�
    Vector2 position_; // ���S�_
    Vector2 rotation_; // ��]�p(rad)
    Vector2 radius_;   // ���a(xy)���~�`�̏ꍇ�Ax�l�݂̂��Q�Ƃ���

    Stage* stagePtr_;

public:
    // setter
    void SetPos(const Vector2& pos) { position_ = pos; }
    void SetRot(const Vector2& rot) { rotation_ = rot; }
    void SetRad(const Vector2& rad) { radius_ = rad; }

    // getter
    const Vector2& GetPos(void) { return position_; }
    const Vector2& GetRot(void) { return rotation_; }
    const Vector2& GetRad(void) { return radius_; }
};

