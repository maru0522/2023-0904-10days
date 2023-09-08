#include "SceneTransition.h"
#include <algorithm>
#include "MathUtil.h"

void SceneTransition::Reset(void)
{
    // Šª‚¢‚Ä‚é•”•ª
    pos_rolledScroll_.x = 1350;
    pos_rolledScroll_.y = 360;
    pos_unrollScroll_.x = 640 + 1280;
    pos_unrollScroll_.y = 360;
    pos_TBBeltBG_.x = 640 + 1280;
    pos_TBBeltBG_.y = 30;

    frameCount_rolled_ = 0;
}

void SceneTransition::Start(void)
{
    Reset();
    isExe_ = true;
}

void SceneTransition::End(void)
{
    isExe_ = false;
    Reset();
}

void SceneTransition::Update(void)
{
    if (isExe_)
    {
        float rate = (std::min)((float)frameCount_rolled_ / kMaxFrameRolled_, 1.f);
        pos_rolledScroll_.x = Math::Ease::EaseOutCubic(rate, 1350, -70);

        frameCount_rolled_++;

        if (rate >= 1.f)
        {
            End();
        }
    }
}

void SceneTransition::Draw(void)
{
    DrawRotaGraph((int32_t)pos_rolledScroll_.x, (int32_t)pos_rolledScroll_.y, 1, 0, png_rolledScroll_, true);
    DrawRotaGraph((int32_t)pos_unrollScroll_.x, (int32_t)pos_unrollScroll_.y, 1, 0, png_unrollScroll_, true);
    DrawRotaGraph((int32_t)pos_TBBeltBG_.x, (int32_t)pos_TBBeltBG_.y, 1, 0, png_TBBeltBG_, true);
    DrawRotaGraph((int32_t)pos_TBBeltBG_.x, (int32_t)720 - pos_TBBeltBG_.y, 1, 0, png_TBBeltBG_, true);
}
