#pragma once
#include "Vector2.h"
#include "Player.h"
#include <cstdint>
#include "Stage.h"

class Enemy final
{
public:
    // �萔
    float kMoveSpeed_{3.f};
    int32_t kMoveInterval_{ 5 }; // �G�������܂ł̊Ԋu(�t���[���P��)

    // �֐�
    Enemy(Player* playerPtr, Stage* stagePtr) : playerPtr_(playerPtr), stagePtr_(stagePtr) {};
    ~Enemy(void) = default;

    void Update(void);
    void Draw(void);

private:
    // �ϐ�
    Player* playerPtr_;
    Stage* stagePtr_;

    Vector2 position_; // ���S�_
    Vector2 rotation_; // ��]�p(rad)
    Vector2 radius_;   // ���a(xy)���~�`�̏ꍇ�Ax�l�݂̂��Q�Ƃ���

    int32_t frameCount_;

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

