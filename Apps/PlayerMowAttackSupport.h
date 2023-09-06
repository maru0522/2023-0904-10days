#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include <DxLib.h>

class PlayerMowAttackSupport : // ’n–‰ß‚¬‚Ä©•ª‚Å‚à¸Î‚ªo‚éƒNƒ‰ƒX
    public IEntity
{
public:
    // ’è”
    const Vector2 kRadius_{ 40,40 }; // UŒ‚”ÍˆÍ { mow‚Ì“™”{,1/2”{ }

    // ŠÖ”
    PlayerMowAttackSupport(CollisionManger* colMPtr);
    ~PlayerMowAttackSupport(void) override;

    void Update(void) override {};
    void Draw(void) override;

private:
    // •Ï”
    CollisionManger* colMPtr_;
};

