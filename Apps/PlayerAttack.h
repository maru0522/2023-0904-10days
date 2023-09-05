#pragma once
#include "IEntity.h"
#include "CollisionManger.h"

class PlayerAttack :
    public IEntity
{
public:
    // ’è”
    const int32_t kMaxAttackFrame_{ 10 }; // UŒ‚”ÍˆÍ‚Ì—LŒøŠÔ(ƒtƒŒ[ƒ€)
    const float kBlewDist_{ 50.f }; // ‚«”ò‚Î‚·‹——£
    const Vector2 kRadius_{ 10,10 }; // UŒ‚”ÍˆÍ

    // ’è‹`
    enum class Direction
    {
        RIGHT,
        LEFT,
    };

    // ŠÖ”
    PlayerAttack(CollisionManger* colMPtr);
    ~PlayerAttack(void) override = default;

    void Attack(const Vector2& vec_move, const Vector2& attackRangeCenter);
    void Update(void) override;
    void Draw(void) override;

private:

    // •Ï”
    int32_t frameCount_attack_; // UŒ‚”ÍˆÍ‚Ì”»’è—P—\(ƒtƒŒ[ƒ€)
    Vector2 vec_playerMove_;
    Direction state_dir_;

public:
    // setter

    //getter
    int32_t GetFrameCountAttack(void) { return frameCount_attack_; }
    const Vector2& GetVecMove(void) { return vec_playerMove_; }
    float GetKBlewDist(void) { return kBlewDist_; }
    Direction GetDirection(void) { return state_dir_; }
};

