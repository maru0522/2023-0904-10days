#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class Player final : public IEntity
{
public:
    // 定数
    float kMoveSpeed_{ 3.f }; // 速度
    float kKnockbackDist_{ 30.f }; // ノックバック距離
    int32_t kMaxInvincibleFrame_{ 120 }; // 無敵時間(フレーム)
    float kPushBackDist_{ 2.f }; // 押し戻す距離

    // 関数
    Player(CollisionManger* colMPtr, Stage* stagePtr);
    ~Player(void) override = default;

    void Update(void);
    void Draw(void);

private:
    void OnCollision(void);

    // 変数
    int32_t frameCount_invincible_;
    Vector2 vec_move_;
    State state_;

    Vector2 pos4Line_;

    PlayerMowAttack attack_;

public:
    const Vector2& GetDirectionVec() { return pos4Line_; }
};
