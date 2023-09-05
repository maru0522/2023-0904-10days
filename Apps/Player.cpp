#include "Player.h"
#include "DxLib.h"
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"
#include "MathUtil.h"

using namespace Util;

Player::Player(CollisionManger* colMPtr, Stage* stagePtr) : IEntity(stagePtr)
{
    // �Փ˃}�l�[�W���ւ̓o�^
    colMPtr->Register(this);

    // �`��ݒ�
    shape_ = Shape::CIRCLE;
    // ���̐ݒ�
    id_ = "player";

    // �Փ�callback���f
    onCollision_ = std::bind(&Player::OnCollision, this);
}

void Player::Update(void)
{
    // ����
    Vector2 input{};
    input += PadStick();
#ifdef _DEBUG
    input.x += KEYS::IsDown(KEY_INPUT_D) - KEYS::IsDown(KEY_INPUT_A);
    input.y += KEYS::IsDown(KEY_INPUT_S) - KEYS::IsDown(KEY_INPUT_W);
#endif // _DEBUG

    // �ړ���̍��W = ���W + (���K�����ꂽ���͒l * ���x)
    Vector2 moved_pos = position_ + input.Normalize() * kMoveSpeed_;

    // �ړ���̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ�ړ��ł���
    if (moved_pos.x - radius_.x > stagePtr_->GetLT().x && moved_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
        moved_pos.x + radius_.x < stagePtr_->GetRB().x && moved_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
    {
        // ���W���ړ���̒l��
        position_ = moved_pos;
    }

    // ���G���Ԓ��Ȃ�
    if (frameCount_invincible_ != 0)
    {
        // ���G���Ԃ̃t���[���J�E���g�����Z
        Math::Function::LoopIncrement<int32_t>(frameCount_invincible_, 0, kMaxInvincibleFrame_);
    }
}

void Player::Draw(void)
{
    // �`��

    // ���G���Ԓ��Ȃ�
    if (frameCount_invincible_ != 0)
    {
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::YELLOW, true, 1);
        DrawFormatString(1000, 20, Util::Color::YELLOW, "���G���");
        DrawFormatString(1000, 40, Util::Color::YELLOW, "frame: %d", kMaxInvincibleFrame_ - frameCount_invincible_);
    }
    else
    {
        DrawFormatString(1000, 20, Util::Color::WHITE, "�ʏ���");
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);
    }

}

void Player::OnCollision(void)
{
    // �ڐG�Ώۂ̖��̂� enemy
    if (other_->GetId() == "enemy")
    {
        // �G���玩���܂ł̕����x�N�g��
        Vector2 vec_enemy2player = (position_ - other_->GetPos()).Normalize();

        // ���G���Ԓ��łȂ��Ȃ�
        if (frameCount_invincible_ == 0)
        {
            // �m�b�N�o�b�N��̍��W = ���W + (���K�����ꂽ�m�b�N�o�b�N���� * ���x)
            Vector2 knockbacked_pos = position_ + vec_enemy2player * kKnockbackDist_;

            // �m�b�N�o�b�N��̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
            if (knockbacked_pos.x - radius_.x > stagePtr_->GetLT().x && knockbacked_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
                knockbacked_pos.x + radius_.x < stagePtr_->GetRB().x && knockbacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
            {
                // ���f
                position_ = knockbacked_pos;
                // ���G���Ԃɓ���
                frameCount_invincible_++;
            }
        }
        else // ���G���Ԓ��Ȃ牟���߂�
        {
            // �����߂���̍��W = ���W + (���K�����ꂽ�����߂����� * ���x)
            Vector2 pushBacked_pos = position_ + vec_enemy2player * kPushBackDist_;

            // �m�b�N�o�b�N��̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
            if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
                pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
            {
                // ���f
                position_ = pushBacked_pos;
            }
        }
    }
}
