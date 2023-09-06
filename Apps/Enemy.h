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
    static const float kMowRatio_; // 吹っ飛ばされたとき距離に対して、どのくらいの割合で移動していくか
    static const float kPushBackDist_; // 押し戻す距離
    static const float kPngScale_; // 画像の拡大率
    const float KRadius_{ 10.f }; // 半径

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
    bool isContactMowSupport_{}; // 薙ぎ払いの検知サポートクラスの当たり判定に触れたか

    // 薙ぎ払われたか
    bool isMowDown_ = false;
    Vector2 vec_mow_{}; // 吹っ飛ばされる方向
    // 吹き飛ばされる距離だけちゃんと移動したか計測する変数
    float tortalMowDist{};

    // 薙ぎ払われた最中に合体OKフラグ
    bool isDocking_{};

    // resource
    int32_t png_enemy_ = LoadGraph("Resources/texture/enemy.png");

public:
    // setter
    void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }
    void SetIsDocking(bool isDocking) { isDocking_ = isDocking; }

    // getter
    bool GetIsMowDown() { return isMowDown_; }
    bool GetIsAlive(void) { return isAlive_; }
    bool GetIsDocking() { return isDocking_; }
    const Vector2& GetMowDownVec() { return vec_mow_; }
    
public:
    //合体したときに外部で呼び出す
    void Docking();
};

