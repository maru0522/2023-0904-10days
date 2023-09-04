#include "Player.h"
#include "DxLib.h"
#include "Util.h"
#include "Keyboard.h"

using namespace Util;

void Player::Update(void)
{
    // ì¸óÕ
    Vector2 input{};
    input.x += KEYS::IsDown(KEY_INPUT_D) - KEYS::IsDown(KEY_INPUT_A);
    input.y += KEYS::IsDown(KEY_INPUT_S) - KEYS::IsDown(KEY_INPUT_W);

    // ç¿ïW += (ê≥ãKâªÇ≥ÇÍÇΩì¸óÕíl * ë¨ìx)
    position_ += input.Normalize() * kMoveSpeed_;
}

void Player::Draw(void)
{
    // ï`âÊ
    DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);
}
