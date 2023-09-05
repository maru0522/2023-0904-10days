#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class PlayerSkewerAttack :
    public IEntity
{
public:
    // �萔
    const Vector2 kRadius_{ 10.f,10.f }; // �U���͈�

    // �֐�
    PlayerSkewerAttack(CollisionManger* colMPtr);
    ~PlayerSkewerAttack(void) override = default;

    void Attack(const Vector2& attackRangeCenter);
    void Update(const Vector2& attackRangeCenter);
    void Draw(void) override;

private:
    // �ϐ�
    bool isSkewer_; // ���h���U������
};

