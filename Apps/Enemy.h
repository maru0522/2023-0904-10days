#pragma once
#include "Player.h"
#include "IEntity.h"
#include <DxLib.h>

class Enemy final : public IEntity
{
public:
    // �萔
    static const float kMoveSpeed_; // ���x
    static const int32_t kMoveTimes_{ 5 }; // ����ɕ����Ĉړ����邩(�u�Ԉړ��Ɍ�����̂�����邽�߁j
    static const int32_t kMoveInterval_{ 100 }; // �G�������܂ł̊Ԋu(�t���[���P��)
    static const float kMowRatio_; // ������΂��ꂽ�Ƃ������ɑ΂��āA�ǂ̂��炢�̊����ňړ����Ă�����
    static const float kPushBackDist_; // �����߂�����
    static const float kPngScale_; // �摜�̊g�嗦
    const float KRadius_{ 10.f }; // ���a

    // �֐�
    Enemy(CollisionManger* colMPtr, Player* playerPtr, Stage* stagePtr);
    ~Enemy(void) override;

    void Update(void) override;
    void Draw(void) override;

private:
    void OnCollision(void);

    // �ϐ�
    Player* playerPtr_;
    CollisionManger* colMPtr_;

    int32_t frameCount_wait_; // �ҋ@�p�t���[���J�E���g
    int32_t frameCount_move_; // �ړ��p�t���[���J�E���g

    bool isAlive_{ true }; // �����Ă邩
    bool isContactMowSupport_{}; // �ガ�����̌��m�T�|�[�g�N���X�̓����蔻��ɐG�ꂽ��

    // �ガ����ꂽ��
    bool isMowDown_ = false;
    Vector2 vec_mow_{}; // ������΂�������
    // ������΂���鋗�����������ƈړ��������v������ϐ�
    float tortalMowDist{};

    // �ガ����ꂽ�Œ��ɍ���OK�t���O
    bool isDocking_{};

    // resource
    int32_t png_enemy_ = LoadGraph("Resources/texture/enemy.png");

public:
    // setter
    void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }
    void SetIsDocking(bool isDocking) { isDocking_ = isDocking; }

    // getter
    bool GetIsMowDown() { return isMowDown_; }
    bool GetIsAlive(void) { return isAlive_; }
    bool GetIsDocking() { return isDocking_; }
    const Vector2& GetMowDownVec() { return vec_mow_; }
    
public:
    //���̂����Ƃ��ɊO���ŌĂяo��
    void Docking();
};

