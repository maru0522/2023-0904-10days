#include "Stage.h"
#include <DxLib.h>
#include "Util.h"
#include <cstdint>

void Stage::Initialize(const Vector2& lt, const Vector2& rb)
{
    leftTop_ = lt;
    RightBottom_ = rb;
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
    // çsìÆêßå¿îÕàÕï\é¶
    DrawBox((int32_t)leftTop_.x, (int32_t)leftTop_.y, (int32_t)RightBottom_.x, (int32_t)RightBottom_.y, Util::Color::GREEN, false);
}
