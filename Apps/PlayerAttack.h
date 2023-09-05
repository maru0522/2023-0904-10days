#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include <DxLib.h>

class PlayerMowAttack :
    public IEntity
{
public:
    // �萔
    const int32_t kMaxAttackFrame_{ 10 }; // �U���͈̗͂L������(�t���[��)
    const float kBlewDist_{ 80.f }; // ������΂�����
    const Vector2 kRadius_{ 50,0 }; // �U���͈� �����ݔ��肪�~�Ȃ̂�y�g���܂���
    const float kPngScale_{ 0.06f }; // �摜�̊g�嗦

    // ��`
    enum class Direction
    {
        RIGHT,
        LEFT,
    };

    // �֐�
    PlayerMowAttack(CollisionManger* colMPtr);
    ~PlayerMowAttack(void) override = default;

    void Attack(const Vector2& vec_move, const Vector2& attackRangeCenter,float rot);
    void Update(void) override;
    void Draw(void) override;

private:

    // �ϐ�
    int32_t frameCount_attack_; // �U���͈͂̔���P�\(�t���[��)
    Vector2 vec_playerMove_;
    Direction state_dir_;

    // resource
    int32_t png_mowAttack_ = LoadGraph("Resources/attack_area.png");

public:
    // setter

    //getter
    int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
    const Vector2& GetVecMove(void) { return vec_playerMove_; }
    float GetKBlewDist(void) { return kBlewDist_; }
    Direction GetDirection(void) { return state_dir_; }
};

