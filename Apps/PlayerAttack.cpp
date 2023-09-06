#include "PlayerAttack.h"
#include <DxLib.h>
#include "Util.h"
#include "MathUtil.h"
#include "Vector3.h"

PlayerMowAttack::PlayerMowAttack(CollisionManger* colMPtr) : IEntity(nullptr), colMPtr_(colMPtr)
{
    // �Փ˃}�l�[�W���ւ̓o�^
    colMPtr->Register(this);

    // �`��ݒ�
    shape_ = Shape::CIRCLE;
    // ���̐ݒ�
    id_ = "player_mowAttack";

    radius_ = kRadius_;
}

PlayerMowAttack::~PlayerMowAttack(void)
{
    // �o�^�̖���
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
}

void PlayerMowAttack::Attack(const Vector2& vec_move, const Vector2& attackRangeCenter,float rot)
{
    // player�̐i�s�������擾
    vec_playerMove_ = vec_move;

    // ��]�p���擾
    rotation_ = rot;

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
    //DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), Util::Color::BLUE, true);
    //DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::BLUE, true, 1);
    DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 70, Util::Color::YELLOW, state_dir_ == Direction::RIGHT ? "�E" : "��");
    DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 90, Util::Color::YELLOW, "%f",rotation_);

    //// �U���̕`��
    state_dir_ == Direction::RIGHT ?
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_mowAttack_, true) :
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_mowAttack_, true,true,true);
}
