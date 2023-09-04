#include "Player.h"
#include "DxLib.h"
#include "Util.h"

using namespace Util;

void Player::Draw(void)
{
    DrawCircle(position_.x, position_.y, radius_.x, Color::WHITE, true, 1);
}
