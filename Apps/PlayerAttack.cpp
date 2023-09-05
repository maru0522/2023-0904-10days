#include "PlayerAttack.h"
#include <DxLib.h>
#include "Util.h"
#include "MathUtil.h"

PlayerMowAttack::PlayerMowAttack(CollisionManger* colMPtr) : IEntity(nullptr)
{
    // �Փ˃}�l�[�W���ւ̓o�^
    colMPtr->Register(this);

    // �`��ݒ�
    shape_ = Shape::SQUARE;
    // ���̐ݒ�
    id_ = "player_mowAttack";

    radius_ = kRadius_;
}

void PlayerMowAttack::Attack(const Vector2& vec_move, const Vector2& attackRangeCenter)
{
    // player�̐i�s�������擾
    vec_playerMove_ = vec_move;

    // ���S�n�_���擾�i����A�v���C���[�̉E�ʒu�����ʒu���ς��̂ŁA�擾���Ȃ����K�v������j
    position_ = attackRangeCenter;

    // �U�����łȂ��Ȃ�
    if (frameCount_attack_ == 0)
    {
        // �U���J�n
        frameCount_attack_++;
    }

    // ������RIGHT�Ȃ玟�̂��߂�LEFT�ɂ���B�t���܂��R��B
    state_dir_ == Direction::RIGHT ?
        state_dir_ = Direction::LEFT :
        state_dir_ = Direction::RIGHT;
}

void PlayerMowAttack::Update(void)
{
    // �U�����Ȃ�
    if (frameCount_attack_)
    {
        // �U�����ԃJ�E���g�t���[�������Z
        Math::Function::LoopIncrement(frameCount_attack_, 0, kMaxAttackFrame_);
    }
}

void PlayerMowAttack::Draw(void)
{
    // �U���͈͂������B
    DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), Util::Color::BLUE, true);
}
