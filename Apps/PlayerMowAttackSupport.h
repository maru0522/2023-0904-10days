#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include <DxLib.h>

class PlayerMowAttackSupport : // 地獄過ぎて自分でも失笑が出るクラス
    public IEntity
{
public:
    // 定数
    const Vector2 kRadius_{ 40,40 }; // 攻撃範囲 { mowの等倍,1/2倍 }

    // 関数
    PlayerMowAttackSupport(CollisionManger* colMPtr);
    ~PlayerMowAttackSupport(void) override;

    void Update(void) override {};
    void Draw(void) override;

private:
    // 変数
    CollisionManger* colMPtr_;
};

