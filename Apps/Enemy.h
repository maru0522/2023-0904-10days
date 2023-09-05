#pragma once
#include "Vector2.h"
#include "Player.h"
#include <cstdint>
#include "Stage.h"

class Enemy final
{
public:
    // 定数
    float kMoveSpeed_{30.f}; // 速度
    int32_t kMoveTimes_{ 5 }; // 何回に分けて移動するか(瞬間移動に見えるのを避けるため）
    int32_t kMoveInterval_{ 180 }; // 敵が動くまでの間隔(フレーム単位)

    // 関数
    Enemy(Player* playerPtr, Stage* stagePtr);
    ~Enemy(void) = default;

    void Update(void);
    void Draw(void);

private:
    // 変数
    Player* playerPtr_;
    Stage* stagePtr_;

    Vector2 position_; // 中心点
    Vector2 rotation_; // 回転角(rad)
    Vector2 radius_;   // 半径(xy)※円形の場合、x値のみを参照する

    int32_t frameCount_wait_; // 待機用フレームカウント
    int32_t frameCount_move_; // 移動用フレームカウント

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

