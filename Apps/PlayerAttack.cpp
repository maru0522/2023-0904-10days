#include "PlayerAttack.h"
#include <DxLib.h>
#include "Util.h"
#include "MathUtil.h"
#include "Vector3.h"

PlayerMowAttack::PlayerMowAttack(CollisionManger* colMPtr) : IEntity(nullptr), colMPtr_(colMPtr)
{
    // 衝突マネージャへの登録
    colMPtr->Register(this);

    // 形状設定
    shape_ = Shape::CIRCLE;
    // 名称設定
    id_ = "player_mowAttack";

    radius_ = kRadius_;
}

PlayerMowAttack::~PlayerMowAttack(void)
{
    // 登録の抹消
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
}

void PlayerMowAttack::Attack(const Vector2& vec_move, const Vector2& attackRangeCenter,float rot)
{
    // playerの進行方向を取得
    vec_playerMove_ = vec_move;

    // 回転角を取得
    rotation_ = rot;

    // 中心地点を取得（毎回、プレイヤーの右位置か左位置か変わるので、取得しなおす必要がある）
    position_ = attackRangeCenter;

    // 攻撃中でないなら
    if (frameCount_attack_ == 0)
    {
        // 攻撃開始
        frameCount_attack_++;
    }

    // 方向がRIGHTなら次のためにLEFTにする。逆もまた然り。
    state_dir_ == Direction::RIGHT ?
        state_dir_ = Direction::LEFT :
        state_dir_ = Direction::RIGHT;
}

void PlayerMowAttack::Update(void)
{
    // 攻撃中なら
    if (frameCount_attack_)
    {
        // 攻撃時間カウントフレームを加算
        Math::Function::LoopIncrement(frameCount_attack_, 0, kMaxAttackFrame_);
    }
}

void PlayerMowAttack::Draw(void)
{
    // 攻撃範囲を可視化。
    //DrawBox((int32_t)(position_.x - radius_.x), (int32_t)(position_.y - radius_.y), (int32_t)(position_.x + radius_.x), (int32_t)(position_.y + radius_.y), Util::Color::BLUE, true);
    //DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::BLUE, true, 1);
    DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 70, Util::Color::YELLOW, state_dir_ == Direction::RIGHT ? "右" : "左");
    DrawFormatString((int32_t)position_.x - 5, (int32_t)position_.y - 90, Util::Color::YELLOW, "%f",rotation_);

    //// 攻撃の描画
    state_dir_ == Direction::RIGHT ?
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_mowAttack_, true) :
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_mowAttack_, true,true,true);
}
