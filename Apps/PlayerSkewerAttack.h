#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class PlayerSkewerAttack :
    public IEntity
{
public:
    // ’è”
    const Vector2 kRadius_{ 20.f,20.f }; // UŒ‚”ÍˆÍ
    const float kMoveSpeed_{ 10.f }; // ‘¬“x

    // ŠÖ”
    PlayerSkewerAttack(CollisionManger* colMPtr);
    ~PlayerSkewerAttack(void) override = default;

    void Attack(void);
    void Update(void) override;
    void Draw(void) override;
    void End(void);

private:
    // •Ï”
    bool isSkewer_; // ‹øh‚µUŒ‚’†‚©

public:
    // getter
    bool GetIsSkewer(void) { return isSkewer_; }
    float GetKMoveSpeed(void) { return kMoveSpeed_; }
};

