#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include "PlayerAttack.h"
#include "PlayerSkewerAttack.h"

class Player final : public IEntity
{
public:
    // �萔
    const float kMoveSpeed_{ 3.f }; // ���x
    const float kKnockbackDist_{ 30.f }; // �m�b�N�o�b�N����
    const int32_t kMaxInvincibleFrame_{ 120 }; // ���G����(�t���[��)
    const float kPushBackDist_{ 2.f }; // �����߂�����
    //const float kMowAttackCenterDist_{ 10.f }; // �ガ�����U���͈̔͂̒��S���v���C���[�̒��S����ǂꂾ������Ă邩
    const int32_t kChargeFrame4Skewer_{ 60 }; // ���h���U���ׂ̈ɁA���t���[���{�^�������������Ȃ���΂Ȃ�Ȃ���
    const float kSkewerAttackCenterDist_{ 35.f }; // ���h���U���͈̔͂̒��S���v���C���[�̒��S����ǂꂾ������Ă邩 ���d�l��A�ڕW+5frame�ɂ���K�v������B
    const int32_t kSlowMotionFrameUntilStart_{ 0 }; // �X���[���[�V�������n�܂�܂ł̃t���[��

    // ��`
    enum class State
    {
        MOVE,
        ATTACK_MOW,
        ATTACK_SKEWER,
    };

    // �֐�
    Player(CollisionManger* colMPtr, Stage* stagePtr);
    ~Player(void) override = default;

    void Update(void) override;
    void Draw(void) override;

private:
    void MoveUpdate(void);
    void MowAttackUpdate(void);
    void SkewerAttackUpdate(void);
    void OnCollision(void);

    // �ϐ�
    int32_t frameCount_invincible_;
    int32_t frameCount_4Skewer_;
    Vector2 vec_move_;
    State state_;

    Vector2 pos4Line_{};

    PlayerMowAttack mow_;
    PlayerSkewerAttack skewer_;

public:
    const Vector2& GetDirectionVec() { return pos4Line_; }
};
