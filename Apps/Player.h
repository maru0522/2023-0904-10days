#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class Player final : public IEntity
{
public:
    // �萔
    float kMoveSpeed_{ 3.f }; // ���x
    float kKnockbackDist_{ 30.f }; // �m�b�N�o�b�N����
    int32_t kMaxInvincibleFrame_{ 120 }; // ���G����(�t���[��)
    float kPushBackDist_{ 2.f }; // �����߂�����

    // �֐�
    Player(CollisionManger* colMPtr, Stage* stagePtr);
    ~Player(void) override = default;

    void Update(void);
    void Draw(void);

private:
    void OnCollision(void);

    // �ϐ�
    int32_t frameCount_invincible_;
    Vector2 vec_move_;
    State state_;

    Vector2 pos4Line_;

    PlayerMowAttack attack_;

public:
    const Vector2& GetDirectionVec() { return pos4Line_; }
};
