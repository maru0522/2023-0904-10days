#include "Enemy.h"
#include "DxLib.h"
#include <cstdint>
#include "Util.h"
#include "MathUtil.h"

Enemy::Enemy(Player* playerPtr, Stage* stagePtr) : IEntity(stagePtr), playerPtr_(playerPtr)
{
    // �ҋ@�t���[�����������ɂ΂炯��悤��
    frameCount_wait_ = Math::Function::Random<int32_t>(5, kMoveInterval_ - 5);
}

void Enemy::Update(void)
{
    // �ړ��p�t���[����0�Ȃ�(�ړ����I������Ȃ�)
    if (frameCount_move_ == 0)
    {
        // �ҋ@�pframe�����Z�i�k�ݏ�ԁj
        Math::Function::LoopIncrement<int32_t>(frameCount_wait_, 0, kMoveInterval_);
    }

    // �ҋ@�p�t���[�����K��l�ȏ�Ȃ�i�ҋ@���I������Ȃ�j
    if (frameCount_wait_ >= kMoveInterval_)
    {
        // �ړ��p�t���[�������Z�i�L�я�ԁj���ő�t���[�����͈ړ��񐔂Ɠ���
        Math::Function::LoopIncrement<int32_t>(frameCount_move_, 0, kMoveTimes_);

        // �G(���g)����v���C���[�܂ł̕����x�N�g��
        Vector2 vec_enemy2player = (playerPtr_->GetPos() - position_).Normalize();

        // �ړ���̍��W = ���W + (���K�����ꂽ���͒l * (���x/�ړ���))
        Vector2 moved_pos = position_ + vec_enemy2player * (kMoveSpeed_ / (float)kMoveTimes_);

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
    // �k�ݏ�ԂȂ�
    if (frameCount_move_ == 0)
    {
        // �G�̐F�͐ԐF��
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::RED, true, 1);
    }
    
    // �L�я�ԂȂ�
    if (frameCount_wait_ >= kMoveInterval_)
    {
        // �G�̐F�͗ΐF��
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, true, 1);
    }
}
