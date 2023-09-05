#pragma once
#include "Vector2.h"
#include <cstdint>
#include "Stage.h"

class IEntity
{
public:
    // ŠÖ”
    IEntity(Stage* stagePtr) : stagePtr_(stagePtr) {};
    virtual ~IEntity(void) = default;

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

protected:
    // •Ï”
    Stage* stagePtr_;

    Vector2 position_; // ’†S“_
    Vector2 rotation_; // ‰ñ“]Šp(rad)
    Vector2 radius_;   // ”¼Œa(xy)¦‰~Œ`‚Ìê‡Ax’l‚Ì‚İ‚ğQÆ‚·‚é

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

