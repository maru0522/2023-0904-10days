#include "PlayerMowAttackSupport.h"
#include "Util.h"

PlayerMowAttackSupport::PlayerMowAttackSupport(CollisionManger* colMPtr) : IEntity(nullptr), colMPtr_(colMPtr)
{
    // Õ“Ëƒ}ƒl[ƒWƒƒ‚Ö‚Ì“o˜^
    colMPtr->Register(this);

    // Œ`óÝ’è
    shape_ = Shape::SQUARE;
    // –¼ÌÝ’è
    id_ = "player_mowAttackSupport";

    radius_ = kRadius_;
}

PlayerMowAttackSupport::~PlayerMowAttackSupport(void)
{
    // “o˜^‚Ì–•Á
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
}

void PlayerMowAttackSupport::Draw(void)
{
#ifdef _DEBUG
    DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), Util::Color::GREEN, false);
#endif // _DEBUG

}