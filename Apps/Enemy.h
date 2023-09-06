#pragma once
#include "Player.h"
#include "IEntity.h"
#include <DxLib.h>

class Enemy final : public IEntity
{
public:
    // 定数
    static const float kMoveSpeed_; // 速度
    static const int32_t kMoveTimes_{ 5 }; // 何回に分けて移動するか(瞬間移動に見えるのを避けるため）
    static const int32_t kMoveInterval_{ 100 }; // 敵が動くまでの間隔(フレーム単位)
    const float kPushBackDist_{ 2.f }; // 押し戻す距離
    const float kPngScale_{ 0.07f }; // 画像の拡大率

    // 関数
    Enemy(CollisionManger* colMPtr, Player* playerPtr, Stage* stagePtr);
    ~Enemy(void) override;

    void Update(void) override;
    void Draw(void) override;

private:
    void OnCollision(void);

    // 変数
    Player* playerPtr_;
    CollisionManger* colMPtr_;

    int32_t frameCount_wait_; // 待機用フレームカウント
    int32_t frameCount_move_; // 移動用フレームカウント

    bool isAlive_{ true }; // 生きてるか
    //薙ぎ払われたか
    bool isMowDown_ = false;

    // resource
    int32_t png_enemy_ = LoadGraph("Resources/texture/enemy.png");

public:
    // setter
    void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }

    // getter
    bool GetIsMowDown() { return isMowDown_; }
    bool GetIsAlive(void) { return isAlive_; }
};

