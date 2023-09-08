#include "SceneTransition.h"
#include <algorithm>
#include "MathUtil.h"

void SceneTransition::Reset(void)
{
    // �����Ă镔��
    pos_rolledScroll_.x = 1350;
    pos_rolledScroll_.y = 360;
    pos_unrollScroll_.x = 640 + 1280;
    pos_unrollScroll_.y = 360;
    pos_TBBeltBG_.x = 640 + 1280;
    pos_TBBeltBG_.y = 30;

    // �A���t�@�l������
    alpha_ = 255;

    // �t���[���J�E���g������
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
        // �ŏ��̈ړ��̃��[�g
        float rate_move1 = (std::min)((float)frameCount_rolled_ / kMaxFrameRolled_, 1.f);
        // �����Ă��̍��W
        pos_rolledScroll_.x = Math::Ease::EaseOutCubic(rate_move1, 1350, -70); // �ړ��� 1420
        // �����ĂȂ���̍��W
        pos_unrollScroll_.x = Math::Ease::EaseOutCubic(rate_move1, 1920, 640); // �ړ��� 1280
        // �����ĂȂ���̍��W
        pos_TBBeltBG_.x = Math::Ease::EaseOutCubic(rate_move1, 1920, 640); // �ړ��� 1280

        // �t���[���J�E���g
        frameCount_rolled_++;
        if (rate_move1 >= 1.f)
        {
            //End();
            //return;

            // 2�Ԗڂ̉摜���߂̃��[�g
            float rate_trans = (std::min)((float)frameCount_transBG_ / kMaxFrameRolledTrans_, 1.f);
            alpha_ = Math::Ease::EaseInOutSine(rate_trans, 255, 0); // �ړ��� 1420

            // �t���[���J�E���g
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
    // ����
    DrawRotaGraph((int32_t)pos_rolledScroll_.x, (int32_t)pos_rolledScroll_.y, 1, 0, png_rolledScroll_, true);

    // �w�i-�^�񒆂̕���
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
    DrawRotaGraph((int32_t)pos_unrollScroll_.x, (int32_t)pos_unrollScroll_.y, 1, 0, png_unrollScroll_, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

    // �w�i-�㉺�̑�
    DrawRotaGraph((int32_t)pos_TBBeltBG_.x, (int32_t)pos_TBBeltBG_.y, 1, 0, png_TBBeltBG_, true);
    DrawRotaGraph((int32_t)pos_TBBeltBG_.x, (int32_t)(720.f - pos_TBBeltBG_.y), 1, 0, png_TBBeltBG_, true, false, true);
}
