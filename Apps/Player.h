#pragma once
#include "Vector2.h"

class Player
{
public:
    // 定数
    float kMoveSpeed_{3.f};

    // 関数
    Player(void) = default;
    ~Player(void) = default;

    void Update(void);
    void Draw(void);

private:
    // 変数
    Vector2 position_; // 中心点
    Vector2 rotation_; // 回転角(rad)
    Vector2 radius_;   // 半径(xy)※円形の場合、x値のみを参照する

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

