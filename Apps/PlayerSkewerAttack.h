#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class PlayerSkewerAttack :
    public IEntity
{
public:
    // ’è”
    const Vector2 kRadius_{ 10.f,10.f }; // UŒ‚”ÍˆÍ

    // ŠÖ”
    PlayerSkewerAttack(CollisionManger* colMPtr);
    ~PlayerSkewerAttack(void) override = default;

    void Attack(const Vector2& attackRangeCenter);
    void Update(const Vector2& attackRangeCenter);
    void Draw(void) override;

private:
    // •Ï”
    bool isSkewer_; // ‹øh‚µUŒ‚’†‚©
};

