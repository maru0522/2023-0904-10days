#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class PlayerAttack :
    public IEntity
{
public:
    // �萔
    const int32_t kMaxAttackFrame_{ 10 }; // �U���͈̗͂L������(�t���[��)
    const float kBlewDist_{ 50.f }; // ������΂�����
    const Vector2 kRadius_{ 10,10 }; // �U���͈�

    // ��`
    enum class Direction
    {
        RIGHT,
        LEFT,
    };

    // �֐�
    PlayerAttack(CollisionManger* colMPtr);
    ~PlayerAttack(void) override = default;

    void Attack(const Vector2& vec_move, const Vector2& attackRangeCenter);
    void Update(void) override;
    void Draw(void) override;

private:

    // �ϐ�
    int32_t frameCount_attack_; // �U���͈͂̔���P�\(�t���[��)
    Vector2 vec_playerMove_;
    Direction state_dir_;

public:
    // setter

    //getter
    int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
    const Vector2& GetVecMove(void) { return vec_playerMove_; }
    float GetKBlewDist(void) { return kBlewDist_; }
    Direction GetDirection(void) { return state_dir_; }
};

