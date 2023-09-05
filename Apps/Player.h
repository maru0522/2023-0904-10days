#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include "PlayerAttack.h"

class Player final : public IEntity
{
public:
    // 定数
    const float kMoveSpeed_{ 3.f }; // 速度
    const float kKnockbackDist_{ 30.f }; // ノックバック距離
    const int32_t kMaxInvincibleFrame_{ 120 }; // 無敵時間(フレーム)
    const float kPushBackDist_{ 2.f }; // 押し戻す距離
    const float kAttackCenterDist_{ 10.f }; // 攻撃範囲の中心がプレイヤーの中心からどれだけ離れてるか

    // 定義
    enum class State
    {
        MOVE,
        ATTACK,
    };

    // 関数
    Player(CollisionManger* colMPtr, Stage* stagePtr);
    ~Player(void) override = default;

    void Update(void) override;
    void Draw(void) override;

private:
    void MoveUpdate(void);
    void AttackUpdate(void);
    void OnCollision(void);

    // 変数
    int32_t frameCount_invincible_;
    Vector2 vec_move_;
    State state_;

    PlayerAttack attack_;
};
