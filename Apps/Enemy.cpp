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

        // 座標に加算
        position_ += vec_enemy2player * kMoveSpeed_;
    }
}

void Enemy::Draw(void)
{
    DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, true, 1);
}
