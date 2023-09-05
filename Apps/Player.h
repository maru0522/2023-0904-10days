#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include "PlayerAttack.h"

class Player final : public IEntity
{
public:
    // �萔
    const float kMoveSpeed_{ 3.f }; // ���x
    const float kKnockbackDist_{ 30.f }; // �m�b�N�o�b�N����
    const int32_t kMaxInvincibleFrame_{ 120 }; // ���G����(�t���[��)
    const float kPushBackDist_{ 2.f }; // �����߂�����
    const float kAttackCenterDist_{ 10.f }; // �U���͈͂̒��S���v���C���[�̒��S����ǂꂾ������Ă邩

    // ��`
    enum class State
    {
        MOVE,
        ATTACK,
    };

    // �֐�
    Player(CollisionManger* colMPtr, Stage* stagePtr);
    ~Player(void) override = default;

    void Update(void) override;
    void Draw(void) override;

private:
    void MoveUpdate(void);
    void AttackUpdate(void);
    void OnCollision(void);

    // �ϐ�
    int32_t frameCount_invincible_;
    Vector2 vec_move_;
    State state_;

    PlayerAttack attack_;
};
