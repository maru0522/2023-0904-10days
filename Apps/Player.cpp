#include "Player.h"
#include "DxLib.h"
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"
#include "MathUtil.h"
#include "Vector3.h"

using namespace Util;

Player::Player(CollisionManger* colMPtr, Stage* stagePtr) : IEntity(stagePtr), attack_(colMPtr)
{
    // 衝突マネージャへの登録
    colMPtr->Register(this);

    // 形状設定
    shape_ = Shape::CIRCLE;
    // 名称設定
    id_ = "player";

    // 衝突callback反映
    onCollision_ = std::bind(&Player::OnCollision, this);
}

void Player::Update(void)
{
    // 無敵時間中なら
    if (frameCount_invincible_ != 0)
    {
        // 無敵時間のフレームカウントを加算
        Math::Function::LoopIncrement<int32_t>(frameCount_invincible_, 0, kMaxInvincibleFrame_);
    }

    void (Player:: * FuncTbl[])() =
    {
        &Player::MoveUpdate,
        &Player::MowAttackUpdate,
    };

    (this->*FuncTbl[(size_t)state_])();

    pos4Line_ = position_ + vec_move_ * 30;
}

void Player::Draw(void)
{
    // 描画
    DrawLineAA(position_.x, position_.y, pos4Line_.x, pos4Line_.y, Color::WHITE, 3);

    // 無敵時間中なら
    if (frameCount_invincible_ != 0)
    {
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::YELLOW, true, 1);
        DrawFormatString(1000, 20, Util::Color::YELLOW, "無敵状態");
        DrawFormatString(1000, 40, Util::Color::YELLOW, "frame: %d", kMaxInvincibleFrame_ - frameCount_invincible_);
    }
    else
    {
        DrawFormatString(1000, 20, Util::Color::WHITE, "通常状態");
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);
    }

    if (state_ == State::ATTACK_MOW)
    {
        attack_.Draw();
    }
}

void Player::MoveUpdate(void)
{
    // 入力
    Vector2 input{};
    input += PadStick();
#ifdef _DEBUG
    input.x += KEYS::IsDown(KEY_INPUT_D) - KEYS::IsDown(KEY_INPUT_A);
    input.y += KEYS::IsDown(KEY_INPUT_S) - KEYS::IsDown(KEY_INPUT_W);
#endif // _DEBUG

    // 入力があった時のみ、ベクトルを記録
    if (input.IsNonZero())
    {
        // 移動方向ベクトルを記録
        vec_move_ = input.Normalize();
    }

    // 移動後の座標 = 座標 + (正規化された入力値 * 速度)
    Vector2 moved_pos = position_ + input.Normalize() * kMoveSpeed_;

    // 移動後の座標 (+ 半径)が、ステージの内側なら移動できる
    if (moved_pos.x - radius_.x > stagePtr_->GetLT().x && moved_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
        moved_pos.x + radius_.x < stagePtr_->GetRB().x && moved_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
    {
        // 座標を移動後の値に
        position_ = moved_pos;
    }

    // pad-RでAttack状態に遷移
    if (PadTriggerLorR())
    {
        attack_.Attack(vec_move_, position_);
        state_ = State::ATTACK_MOW;
    }
#ifdef _DEBUG
    // key-SPACEでAttack状態に遷移
    if (KEYS::IsTrigger(KEY_INPUT_SPACE))
    {
        attack_.Attack(vec_move_, position_);
        state_ = State::ATTACK_MOW;
    }
#endif // _DEBUG
}

void Player::MowAttackUpdate(void)
{
    if (attack_.GetFrameCountAttack() == 0)
    {
        state_ = State::MOVE;
    }
    attack_.Update();
}

void Player::OnCollision(void)
{
    // 接触対象の名称が enemy
    if (other_->GetId() == "enemy")
    {
        // 敵から自分までの方向ベクトル
        Vector2 vec_enemy2player = (position_ - other_->GetPos()).Normalize();

        // 無敵時間中でないなら
        if (frameCount_invincible_ == 0)
        {
            // ノックバック後の座標 = 座標 + (正規化されたノックバック方向 * 速度)
            Vector2 knockbacked_pos = position_ + vec_enemy2player * kKnockbackDist_;

            // ノックバック後の座標 (+ 半径)が、ステージの内側なら座標反映
            if (knockbacked_pos.x - radius_.x > stagePtr_->GetLT().x && knockbacked_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
                knockbacked_pos.x + radius_.x < stagePtr_->GetRB().x && knockbacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
            {
                // 反映
                position_ = knockbacked_pos;
                // 無敵時間に入る
                frameCount_invincible_++;
            }
        }
        else // 無敵時間中なら押し戻し
        {
            // 押し戻し後の座標 = 座標 + (正規化された押し戻し方向 * 速度)
            Vector2 pushBacked_pos = position_ + vec_enemy2player * kPushBackDist_;

            // ノックバック後の座標 (+ 半径)が、ステージの内側なら座標反映
            if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
                pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
            {
                // 反映
                position_ = pushBacked_pos;
            }
        }
    }
}
