#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include <DxLib.h>

class PlayerMowAttack :
    public IEntity
{
public:
    // ’è”
    const int32_t kMaxAttackFrame_{ 10 }; // UŒ‚”ÍˆÍ‚Ì—LŒøŠÔ(ƒtƒŒ[ƒ€)
    const float kBlewDist_{ 80.f }; // ‚«”ò‚Î‚·‹——£
    const Vector2 kRadius_{ 50,0 }; // UŒ‚”ÍˆÍ ¦Œ»İ”»’è‚ª‰~‚È‚Ì‚Åyg‚¢‚Ü‚¹‚ñ
    const float kPngScale_{ 0.06f }; // ‰æ‘œ‚ÌŠg‘å—¦

    // ’è‹`
    enum class Direction
    {
        RIGHT,
        LEFT,
    };

    // ŠÖ”
    PlayerMowAttack(CollisionManger* colMPtr);
    ~PlayerMowAttack(void) override = default;

    void Attack(const Vector2& vec_move, const Vector2& attackRangeCenter,float rot);
    void Update(void) override;
    void Draw(void) override;

private:

    // •Ï”
    int32_t frameCount_attack_; // UŒ‚”ÍˆÍ‚Ì”»’è—P—\(ƒtƒŒ[ƒ€)
    Vector2 vec_playerMove_;
    Direction state_dir_;

    // resource
    int32_t png_mowAttack_ = LoadGraph("Resources/attack_area.png");

public:
    // setter

    //getter
    int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
    const Vector2& GetVecMove(void) { return vec_playerMove_; }
    float GetKBlewDist(void) { return kBlewDist_; }
    Direction GetDirection(void) { return state_dir_; }
};

