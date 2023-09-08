#pragma once
#include "Vector2.h"
#include <memory>

class Stage final
{
public:
    // 関数
    Stage(void) = default;
    ~Stage(void) = default;

    void Initialize(const Vector2& lt,const Vector2& rb);
    void Update(void);
    void Draw(void);

private:
    // 変数
    Vector2 leftTop_{};
    Vector2 RightBottom_{};

    uint32_t png_Stage_ = 0;        //ステージ画像

public:
    // getter
    const Vector2& GetLT(void) { return leftTop_; }
    const Vector2& GetRB(void) { return RightBottom_; }

    // setter
    void SetLT(const Vector2& lt) { leftTop_ = lt; }
    void SetRB(const Vector2& rb) { RightBottom_ = rb; }
};

