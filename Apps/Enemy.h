#pragma once
#include "Player.h"
#include "IEntity.h"

class Enemy final : public IEntity
{
public:
    // �萔
    float kMoveSpeed_{30.f}; // ���x
    int32_t kMoveTimes_{ 5 }; // ����ɕ����Ĉړ����邩(�u�Ԉړ��Ɍ�����̂�����邽�߁j
    int32_t kMoveInterval_{ 100 }; // �G�������܂ł̊Ԋu(�t���[���P��)

    // �֐�
    Enemy(Player* playerPtr, Stage* stagePtr);
    ~Enemy(void) override = default;

    void Update(void);
    void Draw(void);

private:
    // �ϐ�
    Player* playerPtr_;

    int32_t frameCount_wait_; // �ҋ@�p�t���[���J�E���g
    int32_t frameCount_move_; // �ړ��p�t���[���J�E���g

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

