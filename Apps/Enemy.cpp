#include "Enemy.h"
#include "DxLib.h"
#include <cstdint>
#include "Util.h"
#include "MathUtil.h"

Enemy::Enemy(Player* playerPtr, Stage* stagePtr) : IEntity(stagePtr), playerPtr_(playerPtr)
{
    // 待機フレームが生成時にばらけるように
    frameCount_wait_ = Math::Function::Random<int32_t>(5, kMoveInterval_ - 5);
}

void Enemy::Update(void)
{
    // 移動用フレームが0なら(移動が終わったなら)
    if (frameCount_move_ == 0)
    {
        // 待機用frameを加算（縮み状態）
        Math::Function::LoopIncrement<int32_t>(frameCount_wait_, 0, kMoveInterval_);
    }

    // 待機用フレームが規定値以上なら（待機が終わったなら）
    if (frameCount_wait_ >= kMoveInterval_)
    {
        // 移動用フレームを加算（伸び状態）※最大フレーム数は移動回数と同じ
        Math::Function::LoopIncrement<int32_t>(frameCount_move_, 0, kMoveTimes_);

        // 敵(自身)からプレイヤーまでの方向ベクトル
        Vector2 vec_enemy2player = (playerPtr_->GetPos() - position_).Normalize();

        // 移動後の座標 = 座標 + (正規化された入力値 * (速度/移動回数))
        Vector2 moved_pos = position_ + vec_enemy2player * (kMoveSpeed_ / (float)kMoveTimes_);

        // 移動後の座標 (+ 半径)が、ステージの内側なら移動できる
        if (moved_pos.x - radius_.x > stagePtr_->GetLT().x && moved_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
            moved_pos.x + radius_.x < stagePtr_->GetRB().x && moved_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
        {
            // 座標を移動後の値に
            position_ = moved_pos;
        }
    }
}

void Enemy::Draw(void)
{
    // 縮み状態なら
    if (frameCount_move_ == 0)
    {
        // 敵の色は赤色に
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::RED, true, 1);
    }
    
    // 伸び状態なら
    if (frameCount_wait_ >= kMoveInterval_)
    {
        // 敵の色は緑色に
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, true, 1);
    }
}
