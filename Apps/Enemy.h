#pragma once
#include "Player.h"
#include "IEntity.h"

class Enemy final : public IEntity
{
public:
    // 定数
    const float kMoveSpeed_{30.f}; // 速度
    const int32_t kMoveTimes_{ 5 }; // 何回に分けて移動するか(瞬間移動に見えるのを避けるため）
    const int32_t kMoveInterval_{ 100 }; // 敵が動くまでの間隔(フレーム単位)
    const float kPushBackDist_{ 2.f }; // 押し戻す距離

    // 関数
    Enemy(CollisionManger* colMPtr, Player* playerPtr, Stage* stagePtr);
    ~Enemy(void) override = default;

    void Update(void) override;
    void Draw(void) override;

private:
    void OnCollision(void);

    // 変数
    Player* playerPtr_;

    int32_t frameCount_wait_; // 待機用フレームカウント
    int32_t frameCount_move_; // 移動用フレームカウント

public:
    // setter
    void SetPos(const Vector2& pos) { position_ = pos; }
    void SetRot(const Vector2& rot) { rotation_ = rot; }
    void SetRad(const Vector2& rad) { radius_ = rad; }

    // getter
    const Vector2& GetPos(void) { return position_; }
    const Vector2& GetRot(void) { return rotation_; }
    const Vector2& GetRad(void) { return radius_; }
};

