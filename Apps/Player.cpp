#include "Player.h"
#include "DxLib.h"
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"

using namespace Util;

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
}

void Player::Draw(void)
{
    // �`��
    DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);
}
