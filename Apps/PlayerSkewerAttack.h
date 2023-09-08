#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class PlayerSkewerAttack :
    public IEntity
{
public:
    // ’è”
    const Vector2 kRadius_{ 20.f,20.f }; // UŒ‚”ÍˆÍ
    const float kMoveSpeed_{ 20.f }; // ‘¬“x
    const float kPngScale_{ 0.06f }; // ‰æ‘œ‚ÌŠg‘å—¦

    // ŠÖ”
    PlayerSkewerAttack(CollisionManger* colMPtr);
    ~PlayerSkewerAttack(void) override;

    void Attack(void);
    void Update(void) override;
    void Draw(void) override;
    void End(void);

private:
    // •Ï”
    CollisionManger* colMPtr_;

    bool isSkewer_; // ‹øh‚µUŒ‚’†‚©

    uint32_t skeweredEnemiesCount_; //‹øh‚µ‚½“G‚Ì”‚ğƒJƒEƒ“ƒg‚·‚é

public:
    void AddSkeweredEnemiesCount(uint32_t addNum) { skeweredEnemiesCount_ += addNum; }

    // getter
    bool GetIsSkewer(void) { return isSkewer_; }
    float GetKMoveSpeed(void) { return kMoveSpeed_; }
    uint32_t GetSkeweredEnemiesCount(void) { return skeweredEnemiesCount_; }
};

