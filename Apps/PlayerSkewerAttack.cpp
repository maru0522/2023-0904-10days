#include "PlayerSkewerAttack.h"
#include <DxLib.h>
#include "Util.h"

PlayerSkewerAttack::PlayerSkewerAttack(CollisionManger* colMPtr) : IEntity(nullptr), colMPtr_(colMPtr)
{
    // 衝突マネージャへの登録
    colMPtr->Register(this);

    // 形状設定
    shape_ = Shape::SQUARE;
    // 名称設定
    id_ = "player_skewerAttack";

    radius_ = kRadius_;
}

PlayerSkewerAttack::~PlayerSkewerAttack(void)
{
    // 登録の抹消
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
}

void PlayerSkewerAttack::Attack(void)
{
    // 串刺し攻撃中フラグオン
    isSkewer_ = true;
}

void PlayerSkewerAttack::Update(void)
{
    // 中心地点を取得（初期地点）
    //position_ = attackRangeCenter;
}

void PlayerSkewerAttack::Draw(void)
{
    // 攻撃範囲を可視化。
    DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), Util::Color::BLUE, false);
}

void PlayerSkewerAttack::End(void)
{
    isSkewer_ = false;
}
