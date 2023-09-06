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
    const float kPushBackDist_{ 2.f }; // �����߂�����
    const float kPngScale_{ 0.07f }; // �摜�̊g�嗦

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
    //�ガ����ꂽ��
    bool isMowDown_ = false;

    // resource
    int32_t png_enemy_ = LoadGraph("Resources/texture/enemy.png");

public:
    // setter
    void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }

    // getter
    bool GetIsMowDown() { return isMowDown_; }
    bool GetIsAlive(void) { return isAlive_; }
};

