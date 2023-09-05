#include "Enemy.h"
#include "DxLib.h"
#include <cstdint>
#include "Util.h"
#include "MathUtil.h"

void Enemy::Update(void)
{
    // frame加算
    Math::Function::LoopIncrement<int32_t>(frameCount_, 0, kMoveInterval_);

    // frameが規定値以上なら動く
    if (frameCount_ >= kMoveInterval_)
    {
        // 敵(自身)からプレイヤーまでの方向ベクトル
        Vector2 vec_enemy2player = (playerPtr_->GetPos() - position_).Normalize();

        // 移動後の座標 = 座標 + (正規化された入力値 * 速度)
        Vector2 moved_pos = position_ + vec_enemy2player * kMoveSpeed_;

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
    DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, true, 1);
}
