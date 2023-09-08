#include "SceneTransition.h"
#include <algorithm>
#include "MathUtil.h"

void SceneTransition::Reset(void)
{
    // 巻いてる部分
    pos_rolledScroll_.x = 1350;
    pos_rolledScroll_.y = 360;
    pos_unrollScroll_.x = 640 + 1280;
    pos_unrollScroll_.y = 360;
    pos_TBBeltBG_.x = 640 + 1280;
    pos_TBBeltBG_.y = 30;

    // アルファ値初期化
    alpha_ = 255;

    // フレームカウント初期化
    frameCount_rolled_ = 0;
    frameCount_transBG_ = 0;
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
        // 最初の移動のレート
        float rate_move1 = (std::min)((float)frameCount_rolled_ / kMaxFrameRolled_, 1.f);
        // 巻いてるやつの座標
        pos_rolledScroll_.x = Math::Ease::EaseOutCubic(rate_move1, 1350, -70); // 移動幅 1420
        // 巻いてないやつの座標
        pos_unrollScroll_.x = Math::Ease::EaseOutCubic(rate_move1, 1920, 640); // 移動幅 1280
        // 巻いてないやつの座標
        pos_TBBeltBG_.x = Math::Ease::EaseOutCubic(rate_move1, 1920, 640); // 移動幅 1280

        // フレームカウント
        frameCount_rolled_++;
        if (rate_move1 >= 1.f)
        {
            //End();
            //return;

            // 2番目の画像透過のレート
            float rate_trans = (std::min)((float)frameCount_transBG_ / kMaxFrameRolledTrans_, 1.f);
            alpha_ = Math::Ease::EaseInOutSine(rate_trans, 255, 0); // 移動幅 1420

            // フレームカウント
            frameCount_transBG_++;
            if (rate_trans >= 1.f)
            {
                End();
                return;
            }
        }

    }
}

void SceneTransition::Draw(void)
{
    // 巻物
    DrawRotaGraph((int32_t)pos_rolledScroll_.x, (int32_t)pos_rolledScroll_.y, 1, 0, png_rolledScroll_, true);

    // 背景-真ん中の部分
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
    DrawRotaGraph((int32_t)pos_unrollScroll_.x, (int32_t)pos_unrollScroll_.y, 1, 0, png_unrollScroll_, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

    // 背景-上下の帯
    DrawRotaGraph((int32_t)pos_TBBeltBG_.x, (int32_t)pos_TBBeltBG_.y, 1, 0, png_TBBeltBG_, true);
    DrawRotaGraph((int32_t)pos_TBBeltBG_.x, (int32_t)(720.f - pos_TBBeltBG_.y), 1, 0, png_TBBeltBG_, true, false, true);
}
