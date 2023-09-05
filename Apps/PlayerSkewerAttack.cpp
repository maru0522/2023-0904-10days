#include "PlayerSkewerAttack.h"
#include <DxLib.h>
#include "Util.h"

PlayerSkewerAttack::PlayerSkewerAttack(CollisionManger* colMPtr) : IEntity(nullptr)
{
    // �Փ˃}�l�[�W���ւ̓o�^
    colMPtr->Register(this);

    // �`��ݒ�
    shape_ = Shape::SQUARE;
    // ���̐ݒ�
    id_ = "player_skewerAttack";

    radius_ = kRadius_;
}

void PlayerSkewerAttack::Attack(void)
{
    // ���S�n�_���擾�i�����n�_�j
    isSkewer_ = true;
}

void PlayerSkewerAttack::Update(void)
{
    // ���S�n�_���擾�i�����n�_�j
    //position_ = attackRangeCenter;
}

void PlayerSkewerAttack::Draw(void)
{
    // �U���͈͂������B
    DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), Util::Color::BLUE, false);
}

void PlayerSkewerAttack::End(void)
{
    isSkewer_ = false;
}
