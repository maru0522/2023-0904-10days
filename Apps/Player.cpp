#include "Player.h"
#include "DxLib.h"
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"
#include "MathUtil.h"
#include "Vector3.h"
#include "Enemy.h"
#include "SceneManager.h"
#include "EnemyManager.h"

using namespace Util;

const float Player::kMowDist_{ 15.f };// 薙ぎ払いで吹き飛ばす距離 こっち変更するならenemy.hの割合も弄らないと瞬間移動になっちまう

Player::Player(CollisionManger* colMPtr, Stage* stagePtr) : IEntity(stagePtr), mow_(colMPtr), mow_support_(colMPtr), skewer_(colMPtr), colMPtr_(colMPtr)
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

Player::~Player(void)
{
    // 登録の抹消
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
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
        &Player::SkewerAttackUpdate,
    };

    (this->*FuncTbl[(size_t)state_])();

    // 吹き飛ばされるフラグオンだったら
    if (isKnockback_)
    {
        // イージング用のタイムレート
        float rate = (std::min)((float)frameCount_knockback_ / kMaxKnockbackFrame_, 1.f);

        // 吹き飛ばされる速さ を イージングで調整
        const float mowSpeed = (1 - Math::Ease::EaseInSine(rate)) * Player::kKnockbackDist_;
        // 座標に加算
        position_ += vec_contactE2P_ * mowSpeed;

        if (rate >= 1.f)
        {
            frameCount_knockback_ = 0;
            isKnockback_ = false;
            return;
        }

        // フレームカウント加算
        Math::Function::LoopIncrement<int32_t>(frameCount_knockback_, 0, kMaxKnockbackFrame_);
    }

    // デバッグライン用記録
    pos4Line_ = position_ + vec_move_ * 30;

    // 向きから回転角を取得
    rotation_ = std::acos(Vector2(0, -1).Dot(vec_move_));
    //反転しないように
    if (vec_move_.x < 0)
    {
        rotation_ = -rotation_;
    }
    // 右向き方向を取得
    isRight_ = Vector2(0, -1).Cross(vec_move_.Normalize()) > 0.f;
}

void Player::Draw(void)
{
    // 描画
    DrawLineAA(position_.x, position_.y, pos4Line_.x, pos4Line_.y, Color::WHITE, 3);

    // 現在のプレイヤーの状態（数字のみ）
    DrawFormatString(0, 140, 0xffffff, "state ;%d", static_cast<int32_t>(state_));


    // skewerの為にボタン長押ししてるなら
    if (frameCount_4Skewer_ > 0)
    {
        //矢印描画
        Vector2 pos4PredictionLine = position_ + vec_move_ * 1000;

        DrawLine((int32_t)position_.x, (int32_t)position_.y, (int32_t)pos4PredictionLine.x, (int32_t)pos4PredictionLine.y, Color::RED, 2);
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_ * 3.0f, rotation_, png_arrow_, true);

        DrawFormatString(1000, 60, Util::Color::GREEN, "溜め状態");
        DrawFormatString(1000, 80, Util::Color::GREEN, "frame: %d/%d", frameCount_4Skewer_, kChargeFrame4Skewer_);
    }
    else if (state_ != State::ATTACK_SKEWER) // 串刺し攻撃のために溜めてる間や、串刺し攻撃中は半円を表示しない ※それ以外の時に表示
    {
        // 攻撃範囲とdebugの表示
        mow_.Draw();
        mow_support_.Draw();
    }

    // skewerの為にボタン長押ししてない && 串刺し攻撃中でない && 薙ぎ払い攻撃中なら
    // 薙ぎ払い状態 && 薙ぎ払いフレームカウントが0以外
    if (state_ == State::ATTACK_MOW && mow_.GetFrameCountAttack() > 1)
    {
        // 串を描画
        DrawRotaGraph((int32_t)pos4Sword_.x, (int32_t)pos4Sword_.y, kPngScale_, rot4Sword2_, png_sword_, true);
        //DrawCircle((int32_t)pos4Sword_.x, (int32_t)pos4Sword_.y, 1, Util::Color::BLUE, true, 1);
    }
    DrawFormatString(1000, 120, Util::Color::GREEN, "rot4s: %f", rot4RotationSword_);

    if (state_ == State::ATTACK_SKEWER) // 串刺し攻撃中、串刺しの描画関数を呼び出す
    {
        pos4Sword_ += vec_move_ * 5;
        pos4SwordUp_ += vec_move_ * (6 + EnemyManager::GetInstance().GetSkewerEnemiesLength());
        pos4SwordBottom_ -= vec_move_ * 12;

        // 串
        DrawRotaGraph((int32_t)pos4Sword_.x, (int32_t)pos4Sword_.y, kPngScale_, rotation_, png_sword_, true);
        DrawRotaGraph((int32_t)pos4SwordUp_.x, (int32_t)pos4SwordUp_.y, kPngScale_, rotation_, png_swordUp_, true);
        DrawRotaGraph((int32_t)pos4SwordBottom_.x, (int32_t)pos4SwordBottom_.y, kPngScale_, rotation_, png_swordBottom_, true);
        skewer_.Draw();
    }

    // 無敵時間中なら
    if (frameCount_invincible_ != 0)
    {
        //DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::YELLOW, true, 1);

        // playerの描画
        SetDrawBright(255, 255, 80); // 変色量カス
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_player_, true);
        SetDrawBright(255, 255, 255);
        DrawFormatString(1000, 20, Util::Color::YELLOW, "無敵状態");
        DrawFormatString(1000, 40, Util::Color::YELLOW, "frame: %d", kMaxInvincibleFrame_ - frameCount_invincible_);
    }
    else // 無敵時間じゃないなら
    {
        DrawFormatString(1000, 20, Util::Color::WHITE, "通常状態");
        //DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);

        // playerの描画
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_player_, true);
    }

    // 串刺し攻撃時の判定座標
    DrawFormatString(1000, 100, Util::Color::GREEN, "pos(%f,%f)", skewer_.GetPos().x, skewer_.GetPos().y);
    DrawFormatString(0, 500, Util::Color::WHITE, "pos(%f,%f)", position_.x, position_.y);
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

    //　pad-Aを押していない時は移動できる。（串刺しの為に溜めてる時は動けない）
    if (frameCount_4Skewer_ == 0) // 串刺しカウントが0なら（=溜めてない)
    {
        // 座標 += (正規化された入力値 * 速度)
        position_ += input.Normalize() * kMoveSpeed_;

        // 押し戻しっつーか、それ以上いかないようにってだけ
        position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
        position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);
    }

#pragma region 薙ぎ払い攻撃の範囲を移動させてる
    mow_.SetPos(position_);
    mow_.SetRot(rotation_);
    // サポートの矩形の中心点 = プレイヤーの座標 + 正面vec * kMowSupportCenterDist_
    Vector2 center4MowSupport = position_ + vec_move_ * kMowSupportCenterDist_;
    // サポートの座標設定
    mow_support_.SetPos(center4MowSupport);
#pragma endregion

    // 無敵中じゃなければ攻撃できる
    if (frameCount_invincible_ == 0)
    {
        // pad-A押してない時 && pad-R||RB でAttack_MOW状態に遷移
        if (PadDownA() == false && PadTriggerLorR() || PadTriggerRB())
        {
            mow_.Attack(vec_move_, position_);
            state_ = State::ATTACK_MOW;
        }

        // pad-A長押しでATTACK_SKEWER状態に遷移
        if (PadDownA())
        {
            // ATTACK_SKEWER状態に入るための溜め計測フレームを加算
            //frameCount_4Skewer_++;
            frameCount_4Skewer_ += 5; // スローモーション回避のため力技だけど5フレーム分ずつカウントします。

            // ↑仕様上押してからスローモーション開始になるので、最初のフレーム分カウントが +n されてしまうのを簡単に回避する方法思いつきません。

            // スローモーション開始
            SceneManager::GetInstance()->StartSlowMotion();
        }
        else
        {
            // 規定フレーム以上触れてたら遷移
            if (frameCount_4Skewer_ >= kChargeFrame4Skewer_)
            {
                // 遷移して初期化
                skewer_.Attack();
                state_ = State::ATTACK_SKEWER;
                frameCount_4Skewer_ = 0;
                pos4Sword_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
                pos4SwordUp_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
                pos4SwordBottom_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
            }
            // 離した瞬間に初期化
            frameCount_4Skewer_ = 0;

            // スローモーション解除
            SceneManager::GetInstance()->EndSlowMotion();
        }
    }

#ifdef _DEBUG
    // key-SPACEでAttack_MOW状態に遷移
    if (KEYS::IsTrigger(KEY_INPUT_SPACE))
    {
        mow_.Attack(vec_move_, position_);
        state_ = State::ATTACK_MOW;
    }
#endif // _DEBUG
}

void Player::MowAttackUpdate(void)
{
    //プレイヤーの前方半円分にいる敵を吹き飛ばす仕様
    // 実現のため、プレイヤーの前方に長方形の当たり判定を出して、かつ円状の当たり判定にも引っかかってるやつを吹っ飛ばす

    // 攻撃判定のフレームが0になったら
    if (mow_.GetFrameCountAttack() == 0)
    {
        // 状態遷移
        state_ = State::MOVE;
        // 関数終了
        return;
    }

    // フレーム換算で進行割合を算出
    float rate = (std::min)((float)(mow_.GetFrameCountAttack() - 1) / PlayerMowAttack::kMaxAttackFrame_, 1.f);
    // 角度で今どのくらいか当てはめる rad = ToRad(割合 * 180°)
    rot4RotationSword_ = Math::Function::ToRadian(kMaxRangeSwordDegree_ * rate);

    // プレイヤーの右方向を出す
    Vector3 vec3_move = { vec_move_.x,vec_move_.y,0 };
    Vector3 vec3_right = Vector3(0, 0, 1).Cross(vec3_move.Normalize());
    Vector2 vec2_right = { vec3_right.x,vec3_right.y };
    // 初期座標 = 右方向 * 規定距離
    const Vector2 initPos = vec2_right * kMowSwordCenterDist_; // 回転時の初期座標
    // 回転移動の座標計算
    pos4Sword_.x = -(initPos.x * std::cos(rot4RotationSword_) - initPos.y * std::sin(rot4RotationSword_));
    pos4Sword_.y = -(initPos.x * std::sin(rot4RotationSword_) + initPos.y * std::cos(rot4RotationSword_));
    // 回転後プレイヤーの位置まで移動させる。
    pos4Sword_.x += position_.x;
    pos4Sword_.y += position_.y;

    // 串の絵自体の回転角を計算する
    rot4Sword2_ = (rotation_ - Math::Function::ToRadian(90)) + rate * Math::Function::ToRadian(kMaxRangeSwordDegree_); // 角度ちょい深めに

    // 薙ぎ払い攻撃本体のUpdate()
    mow_.Update();
}

void Player::SkewerAttackUpdate(void)
{
    // isSkewerがfalseならMOVE状態へ遷移
    if (skewer_.GetIsSkewer() == false)
    {
        state_ = State::MOVE;
        // 判定がその場に残り続けちゃうから、絶対に引っかからない座標に転送するごり押し。 pos(-10万,-10万)
        skewer_.SetPos({ -100000.f, -100000.f });
        // 関数終了
        return;
    }

    // 串刺しの上半分の1フレーム後の座標 = 1フレーム前の上半分の座標 + (正規化されたプレイヤーの向き * 速度)
    Vector2 skewerd_swordUp_pos = pos4SwordUp_ /*+ vec_move_ * skewer_.GetKMoveSpeed()*/;

    // 串刺し1フレーム後の座標 = 座標 + (正規化されたプレイヤーの向き * 速度)
    Vector2 skewered_pos = position_ + vec_move_ * skewer_.GetKMoveSpeed();

    // 串刺しの上半分の1フレーム後の座標 (+ 半径)が、ステージの内側ならプレイヤーの座標を更新
    if (skewerd_swordUp_pos.x - skewer_.GetRad().x > stagePtr_->GetLT().x && skewerd_swordUp_pos.y - skewer_.GetRad().y > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので、yが使われていないのは意図的
        skewerd_swordUp_pos.x + skewer_.GetRad().x < stagePtr_->GetRB().x && skewerd_swordUp_pos.y + skewer_.GetRad().y < stagePtr_->GetRB().y)   // [2023/09/10]仕様が変わり、串の判定位置で止まるか判断するので、矩形の判定をもつ串は、yも使う
    {
        // 反映
        position_ = skewered_pos;
    }
    else // 串刺し1フレーム後の座標 (+ 半径)が、ステージ外なら串刺し状態終了
    {
        skewer_.End(); // isSkewerをfalseにする。
    }

    const float eRange = EnemyManager::GetInstance().GetSkewerEnemiesLength();

    // 串刺し1フレーム後の座標 + (正規化されたプレイヤーの向き * (規定距離 + 串刺してる敵の"直径"))
    skewer_.SetPos(skewered_pos + vec_move_ * (kSkewerAttackCenterDist_ + eRange));
    skewer_.Update();
    // 串刺し絵の座標 = プレイヤーの座標 + 正規化されたプレイヤーの向き * 規定距離)
    pos4Sword_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
    pos4SwordUp_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
    pos4SwordBottom_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
}

void Player::OnCollision(void)
{
    // 接触対象の名称が enemy
    if (other_->GetId() == "enemy")
    {
        // キャストしてenemyとして取得
        Enemy* enemyPtr = static_cast<Enemy*>(other_);

        // 敵死んでんなら押し戻し要らん
        if (enemyPtr->GetIsAlive() == false) return;
        // 串刺し攻撃中も押し戻し要らん
        if (skewer_.GetIsSkewer()) return;

        // 敵から自分までの方向ベクトル
        vec_contactE2P_ = (position_ - other_->GetPos()).Normalize();

        // 無敵時間中でないなら
        if (frameCount_invincible_ == 0)
        {
            // ノクバしま～す
            isKnockback_ = true;
            
            // 無敵時間に入る
            frameCount_invincible_++;

            // 長押し中に被弾した場合リセット(?)
            if (frameCount_4Skewer_)
            {
                frameCount_4Skewer_ = 0;
                SceneManager::GetInstance()->EndSlowMotion();
            }
        }
        else // 無敵時間中なら押し戻し
        {
            // 押し戻し後の座標 = 座標 + (正規化された押し戻し方向 * 速度)
            Vector2 pushBacked_pos = position_ + vec_contactE2P_ * kPushBackDist_;

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
