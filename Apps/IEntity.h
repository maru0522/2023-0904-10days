#pragma once
#include "Vector2.h"
#include <cstdint>
#include "Stage.h"

class IEntity
{
public:
    // �֐�
    IEntity(Stage* stagePtr) : stagePtr_(stagePtr) {};
    virtual ~IEntity(void) = default;

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

protected:
    // �ϐ�
    Stage* stagePtr_;

    Vector2 position_; // ���S�_
    Vector2 rotation_; // ��]�p(rad)
    Vector2 radius_;   // ���a(xy)���~�`�̏ꍇ�Ax�l�݂̂��Q�Ƃ���

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

