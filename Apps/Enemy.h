#pragma once
#include "Vector2.h"
#include "Player.h"
#include <cstdint>
#include "Stage.h"

class Enemy final
{
public:
    // ’è”
    float kMoveSpeed_{3.f};
    int32_t kMoveInterval_{ 5 }; // “G‚ª“®‚­‚Ü‚Å‚ÌŠÔŠu(ƒtƒŒ[ƒ€’PˆÊ)

    // ŠÖ”
    Enemy(Player* playerPtr, Stage* stagePtr) : playerPtr_(playerPtr), stagePtr_(stagePtr) {};
    ~Enemy(void) = default;

    void Update(void);
    void Draw(void);

private:
    // •Ï”
    Player* playerPtr_;
    Stage* stagePtr_;

    Vector2 position_; // ’†S“_
    Vector2 rotation_; // ‰ñ“]Šp(rad)
    Vector2 radius_;   // ”¼Œa(xy)¦‰~Œ`‚Ìê‡Ax’l‚Ì‚İ‚ğQÆ‚·‚é

    int32_t frameCount_;

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

