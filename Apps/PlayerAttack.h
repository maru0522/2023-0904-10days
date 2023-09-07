#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include <DxLib.h>
#include "PlayerMowAttackSupport.h"

class PlayerMowAttack :
    public IEntity
{
public:
    // �萔
    static const int32_t kMaxAttackFrame_{ 10 }; // �U���͈̗͂L������(�t���[��)
    const float kBlewDist_{ 80.f }; // ������΂�����
    const Vector2 kRadius_{ 50,0 }; // �U���͈� �����ݔ��肪�~�Ȃ̂�y�g���܂���
    const float kPngScale_{ 0.06f }; // �摜�̊g�嗦

    // �֐�
    PlayerMowAttack(CollisionManger* colMPtr);
    ~PlayerMowAttack(void) override;

    void Attack(const Vector2& vec_move, const Vector2& attackRangeCenter);
    void Update(void) override;
    void Draw(void) override;

private:

    // �ϐ�
    CollisionManger* colMPtr_;

    int32_t frameCount_attack_; // �U���͈͂̔���P�\(�t���[��)
    Vector2 vec_playerMove_;

    // resource
    int32_t png_mowAttack_ = LoadGraph("Resources/texture/attack_area.png");
    uint32_t attack_SE_ = 0;   //�U����

public:
    // setter

    //getter
    int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
    const Vector2& GetVecMove(void) { return vec_playerMove_; }
    const Vector2& GetKRadius(void) { return kRadius_; }
    float GetKBlewDist(void) { return kBlewDist_; }
    
};

