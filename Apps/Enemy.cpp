#include "Enemy.h"
#include "DxLib.h"
#include <cstdint>
#include "Util.h"
#include "MathUtil.h"

void Enemy::Update(void)
{
    // frame���Z
    Math::Function::LoopIncrement<int32_t>(frameCount_, 0, kMoveInterval_);

    // frame���K��l�ȏ�Ȃ瓮��
    if (frameCount_ >= kMoveInterval_)
    {
        // �G(���g)����v���C���[�܂ł̕����x�N�g��
        Vector2 vec_enemy2player = (playerPtr_->GetPos() - position_).Normalize();

        // �ړ���̍��W = ���W + (���K�����ꂽ���͒l * ���x)
        Vector2 moved_pos = position_ + vec_enemy2player * kMoveSpeed_;

        // �ړ���̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ�ړ��ł���
        if (moved_pos.x - radius_.x > stagePtr_->GetLT().x && moved_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
            moved_pos.x + radius_.x < stagePtr_->GetRB().x && moved_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
        {
            // ���W���ړ���̒l��
            position_ = moved_pos;
        }
    }
}

void Enemy::Draw(void)
{
    DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, true, 1);
}
