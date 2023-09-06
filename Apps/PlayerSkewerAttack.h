#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class PlayerSkewerAttack :
    public IEntity
{
public:
    // �萔
    const Vector2 kRadius_{ 20.f,20.f }; // �U���͈�
    const float kMoveSpeed_{ 20.f }; // ���x
    const float kPngScale_{ 0.06f }; // �摜�̊g�嗦

    // �֐�
    PlayerSkewerAttack(CollisionManger* colMPtr);
    ~PlayerSkewerAttack(void) override = default;

    void Attack(void);
    void Update(void) override;
    void Draw(void) override;
    void End(void);

private:
    // �ϐ�
    bool isSkewer_; // ���h���U������

public:
    // getter
    bool GetIsSkewer(void) { return isSkewer_; }
    float GetKMoveSpeed(void) { return kMoveSpeed_; }
};

