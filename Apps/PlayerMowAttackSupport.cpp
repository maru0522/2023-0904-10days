#include "PlayerMowAttackSupport.h"
#include "Util.h"

PlayerMowAttackSupport::PlayerMowAttackSupport(CollisionManger* colMPtr) : IEntity(nullptr), colMPtr_(colMPtr)
{
    // �Փ˃}�l�[�W���ւ̓o�^
    colMPtr->Register(this);

    // �`��ݒ�
    shape_ = Shape::SQUARE;
    // ���̐ݒ�
    id_ = "player_mowAttackSupport";

    radius_ = kRadius_;
}

PlayerMowAttackSupport::~PlayerMowAttackSupport(void)
{
    // �o�^�̖���
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
}

void PlayerMowAttackSupport::Draw(void)
{
#ifdef _DEBUG
    DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), Util::Color::GREEN, false);
#endif // _DEBUG

}