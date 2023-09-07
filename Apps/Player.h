#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include "PlayerAttack.h"
#include "PlayerSkewerAttack.h"
#include <DxLib.h>

class Player final : public IEntity
{
public:
    // 定数
    const float kMoveSpeed_{ 3.f }; // 速度
    const int32_t kMaxInvincibleFrame_{ 120 }; // 無敵時間(フレーム)
    const float kPushBackDist_{ 2.f }; // 押し戻す距離
    const float kKnockbackDist_{ 80.f }; // 敵に触れられてノックバックする距離

    // 薙ぎ払い
    static const float kMowDist_; // 薙ぎ払いで吹き飛ばす距離
    const float kMowSupportCenterDist_{ 35.f }; // mowsupportの中心が、プレイヤーの中心からどんくらいはなれてるか
    //const float kMowAttackCenterDist_{ 10.f }; // 薙ぎ払い攻撃の範囲の中心がプレイヤーの中心からどれだけ離れてるか

    // 串刺し
    const int32_t kChargeFrame4Skewer_{ 60 }; // 串刺し攻撃の為に、何フレームボタンを押し続けなければならないか
    const float kSkewerAttackCenterDist_{ 35.f }; // 串刺し攻撃の範囲の中心がプレイヤーの中心からどれだけ離れてるか ※仕様上、目標+5frameにする必要がある。
    const int32_t kSlowMotionFrameUntilStart_{ 0 }; // スローモーションが始まるまでのフレーム
    const float kPngScale_{ 0.07f }; // 画像の拡大

    // 定義
    enum class State
    {
        MOVE,
        ATTACK_MOW,
        ATTACK_SKEWER,
    };

    // 関数
    Player(CollisionManger* colMPtr, Stage* stagePtr);
    ~Player(void) override;

    void Update(void) override;
    void Draw(void) override;

private:
    void MoveUpdate(void);
    void MowAttackUpdate(void);
    void SkewerAttackUpdate(void);
    void OnCollision(void);

    // 変数
    CollisionManger* colMPtr_;

    int32_t frameCount_invincible_;
    int32_t frameCount_4Skewer_;
    Vector2 vec_move_;
    State state_;

    Vector2 pos4Line_{};

    PlayerMowAttack mow_;
    PlayerMowAttackSupport mow_support_;
    PlayerSkewerAttack skewer_;

    // resources
    bool isRight_{};
    int32_t png_player_ = LoadGraph("Resources/texture/player.png");
    int32_t png_arrow_ = LoadGraph("Resources/texture/arrow.png");  //矢印画像

public:
    const Vector2& GetDirectionVec() { return pos4Line_; }
};
