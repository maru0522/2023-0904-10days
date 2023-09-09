#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include "PlayerAttack.h"
#include "PlayerSkewerAttack.h"
#include <DxLib.h>

class Player final : public IEntity
{
public:
    // �萔
    const float kMoveSpeed_{ 3.f }; // ���x
    const int32_t kMaxInvincibleFrame_{ 120 }; // ���G����(�t���[��)
    const float kPushBackDist_{ 2.f }; // �����߂�����
    const float kKnockbackDist_{ 20.f }; // �G�ɐG����ăm�b�N�o�b�N���鋗��
    const int32_t kMaxKnockbackFrame_{ 10 }; // �m�N�o����Ƃ����t���[���|���ăm�N�o���邩

    // �ガ����
    static const float kMowDist_; // �ガ�����Ő�����΂�����
    const float kMowSupportCenterDist_{ 35.f }; // mowsupport�̒��S���A�v���C���[�̒��S����ǂ񂭂炢�͂Ȃ�Ă邩
    //const float kMowAttackCenterDist_{ 10.f }; // �ガ�����U���͈̔͂̒��S���v���C���[�̒��S����ǂꂾ������Ă邩
    const float kMowSwordCenterDist_{ 30.f }; // �ガ�����U���̊G�̒��S���v���C���[�̒��S����ǂ̂��炢����Ă邩
    const float kMaxRangeSwordDegree_{ 200.f }; // �����ǂ̂��炢�̊p�x�܂ŐU�邩
    const float kMowArrowXRate_{ 0.3f }; // �펞�\������ガ�����p�̖��̊g�嗦X
    const float kMowArrowYRate_{ 0.24f }; // �펞�\������ガ�����p�̖��̊g�嗦Y
    const float kMowArrowDist2Self_{ 80.f }; // �펞�\������ガ�����p�̖��ƃv���C���[�̋���

    // ���h��
    const int32_t kChargeFrame4Skewer_{ 60 }; // ���h���U���ׂ̈ɁA���t���[���{�^�������������Ȃ���΂Ȃ�Ȃ���
    const float kSkewerAttackCenterDist_{ 35.f }; // ���h���U���͈̔͂̒��S���v���C���[�̒��S����ǂꂾ������Ă邩 ���d�l��A�ڕW+5frame�ɂ���K�v������B
    const int32_t kSlowMotionFrameUntilStart_{ 0 }; // �X���[���[�V�������n�܂�܂ł̃t���[��
    const float kPngScale_{ 0.07f }; // �摜�̊g��
    const int32_t kMaxFrameSkewerEndHitStop_{ 20 }; // ���h���I����̃q�b�g�X�g�b�v�����t���[�������葱���邩
    static bool isSkewerHitStop4SceneM_;

    // ��`
    enum class State
    {
        MOVE,
        ATTACK_MOW,
        ATTACK_SKEWER,
    };

    // �֐�
    Player(CollisionManger* colMPtr, Stage* stagePtr);
    ~Player(void) override;

    void Update(void) override;
    void Draw(void) override;

private:
    void MoveUpdate(void);
    void MowAttackUpdate(void);
    void SkewerAttackUpdate(void);
    void OnCollision(void);

    // �ϐ�
    CollisionManger* colMPtr_;

    int32_t frameCount_invincible_; // ���G���ԃJ�E���g�̂��߂̃t���[���J�E���^
    int32_t frameCount_4Skewer_; // ���h���̗��߂̃t���[���J�E���^
    int32_t frameCount_SkewerEndHitStop_; // ���h���I����̃q�b�g�X�g�b�v�ׂ̈̃t���[���J�E���^
    Vector2 vec_move_; // �v���C���[�̌���
    State state_; // ���݂̏��fsm

    Vector2 pos4Line_{}; // debug�p�v���C���[�̌����`��̗��߂̍��W

    bool isKnockback_{}; // �m�N�o����܁`��
    int32_t frameCount_knockback_;
    Vector2 vec_contactE2P_; // �G���ꂽ�G����v���C���[�ւ̌���

    PlayerMowAttack mow_; // �ガ�����p�N���X
    PlayerMowAttackSupport mow_support_; // �ガ�����T�|�[�g�p�N���X
    PlayerSkewerAttack skewer_; // ���h���p�N���X

    // resources
    bool isRight_{};
    int32_t png_player_ = LoadGraph("Resources/texture/player.png");
    int32_t png_arrow_ = LoadGraph("Resources/texture/arrow.png");  //���摜
    int32_t png_sword_ = LoadGraph("Resources/texture/sword.png");
    int32_t png_swordUp_ = LoadGraph("Resources/texture/sword_up.png");
    int32_t png_swordBottom_ = LoadGraph("Resources/texture/sword_bottom.png");
    Vector2 pos4Sword_; // ���̊G�̂��߂̍��W
    Vector2 pos4SwordUp_; // ���̊G�̂��߂̍��W
    Vector2 pos4SwordBottom_; // ���̊G�̂��߂̍��W
    float rot4Sword2_; // ���̊G���̂���]�����邽�߂̊p�x
    float rot4RotationSword_; // ������]�ړ�������ׂ̊p�x

public:
    // getter
    const Vector2& GetDirectionVec() { return pos4Line_.Normalize(); }
    const Vector2& GetMoveVec() { return vec_move_; }
    bool GetIsSkewer(void) { return skewer_.GetIsSkewer(); }
    int32_t GetFrameCountAttack(void) { return mow_.GetFrameCountAttack(); }
    const Vector2& GetPos4SwordBottom(void) { return pos4SwordBottom_; }
};
