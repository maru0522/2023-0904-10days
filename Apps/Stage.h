#pragma once
#include "Vector2.h"

class Stage final
{
public:
    // ä÷êî
    Stage(void) = default;
    ~Stage(void) = default;

    void Initialize(const Vector2& lt,const Vector2& rb);
    void Update(void);
    void Draw(void);

private:
    // ïœêî
    Vector2 leftTop_{};
    Vector2 RightBottom_{};

public:
    // getter
    const Vector2& GetLT(void) { return leftTop_; }
    const Vector2& GetRB(void) { return RightBottom_; }

    // setter
    void SetLT(const Vector2& lt) { leftTop_ = lt; }
    void SetRB(const Vector2& rb) { RightBottom_ = rb; }
};

