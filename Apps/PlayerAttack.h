#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include <DxLib.h>

class PlayerMowAttack :
    public IEntity
{
public:
    // 定数
    static const int32_t kMaxAttackFrame_{ 10 }; // 攻撃範囲の有効時間(フレーム)
    const float kBlewDist_{ 80.f }; // 吹き飛ばす距離
    const Vector2 kRadius_{ 50,0 }; // 攻撃範囲 ※現在判定が円なのでy使いません
    const float kPngScale_{ 0.07f }; // 画像の拡大率

    // 関数
    PlayerMowAttack(CollisionManger* colMPtr);
    ~PlayerMowAttack(void) override;

    void Attack(const Vector2& vec_move, const Vector2& attackRangeCenter);
    void Update(void) override;
    void Draw(void) override;

private:

    // 変数
    CollisionManger* colMPtr_;

    int32_t frameCount_attack_; // 攻撃範囲の判定猶予(フレーム)
    Vector2 vec_playerMove_;

    // resource
    int32_t png_mowAttack_ = LoadGraph("Resources/texture/attack_area.png");
    uint32_t attack_SE_ = 0;   //攻撃音

public:
    // setter

    //getter
    int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
    const Vector2& GetVecMove(void) { return vec_playerMove_; }
    const Vector2& GetKRadius(void) { return kRadius_; }
    float GetKBlewDist(void) { return kBlewDist_; }
    
};

