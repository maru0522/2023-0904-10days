#include "Player.h"
#include "DxLib.h"
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"

using namespace Util;

void Player::Update(void)
{
    // 入力
    Vector2 input{};
    input += PadStick();
#ifdef _DEBUG
    input.x += KEYS::IsDown(KEY_INPUT_D) - KEYS::IsDown(KEY_INPUT_A);
    input.y += KEYS::IsDown(KEY_INPUT_S) - KEYS::IsDown(KEY_INPUT_W);
#endif // _DEBUG

    // 移動後の座標 = 座標 + (正規化された入力値 * 速度)
    Vector2 moved_pos = position_ + input.Normalize() * kMoveSpeed_;

    // 移動後の座標 (+ 半径)が、ステージの内側なら移動できる
    if (moved_pos.x - radius_.x > stagePtr_->GetLT().x && moved_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
        moved_pos.x + radius_.x < stagePtr_->GetRB().x && moved_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
    {
        // 座標を移動後の値に
        position_ = moved_pos;
    }
}

void Player::Draw(void)
{
    // 描画
    DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);
}
