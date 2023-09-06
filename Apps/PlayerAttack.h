#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include <DxLib.h>

class PlayerMowAttack :
    public IEntity
{
public:
    // 定数
    const int32_t kMaxAttackFrame_{ 10 }; // 攻撃範囲の有効時間(フレーム)
    const float kBlewDist_{ 80.f }; // 吹き飛ばす距離
    const Vector2 kRadius_{ 50,0 }; // 攻撃範囲 ※現在判定が円なのでy使いません
    const float kPngScale_{ 0.06f }; // 画像の拡大率

    // 定義
    enum class Direction
    {
        RIGHT,
        LEFT,
    };

    // 関数
    PlayerMowAttack(CollisionManger* colMPtr);
    ~PlayerMowAttack(void) override;

    void Attack(const Vector2& vec_move, const Vector2& attackRangeCenter,float rot);
    void Update(void) override;
    void Draw(void) override;

private:

    // 変数
    CollisionManger* colMPtr_;

    int32_t frameCount_attack_; // 攻撃範囲の判定猶予(フレーム)
    Vector2 vec_playerMove_;
    Direction state_dir_;

    // resource
    int32_t png_mowAttack_ = LoadGraph("Resources/texture/attack_area.png");

public:
    // setter

    //getter
    int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
    const Vector2& GetVecMove(void) { return vec_playerMove_; }
    float GetKBlewDist(void) { return kBlewDist_; }
    Direction GetDirection(void) { return state_dir_; }
};

