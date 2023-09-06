#include "Player.h"
#include "DxLib.h"
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"
#include "MathUtil.h"
#include "Vector3.h"
#include "Enemy.h"
#include "SceneManager.h"

using namespace Util;

Player::Player(CollisionManger* colMPtr, Stage* stagePtr) : IEntity(stagePtr), mow_(colMPtr), skewer_(colMPtr), colMPtr_(colMPtr)
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

	pos4Line_ = position_ + vec_move_ * 30;

	rotation_ = std::acos(Vector2(0, -1).Dot(vec_move_));
	//反転しないように
	if (vec_move_.x < 0)
	{
		rotation_ = -rotation_;
	}
	isRight_ = Vector2(0, -1).Cross(vec_move_.Normalize()) > 0.f;
}

void Player::Draw(void)
{
	// 描画
	DrawLineAA(position_.x, position_.y, pos4Line_.x, pos4Line_.y, Color::WHITE, 3);

	// 無敵時間中なら
	if (frameCount_invincible_ != 0)
	{
		//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::YELLOW, true, 1);

		// playerの描画
		SetDrawBright(255, 255, 80); // 変色量カス
		isRight_ ?
			DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_player_, true, false) :
			DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, -rotation_, png_player_, true);
		SetDrawBright(255, 255, 255);
		DrawFormatString(1000, 20, Util::Color::YELLOW, "無敵状態");
		DrawFormatString(1000, 40, Util::Color::YELLOW, "frame: %d", kMaxInvincibleFrame_ - frameCount_invincible_);
	}
	else
	{
		DrawFormatString(1000, 20, Util::Color::WHITE, "通常状態");
		//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);

		// playerの描画
		isRight_ ?
			DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_player_, true, false) :
			DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, -rotation_, png_player_, true);
	}

	if (state_ == State::ATTACK_MOW)
	{
		mow_.Draw();
	}

	// skewerの為にボタン長押ししてるなら
	if (frameCount_4Skewer_ > 0)
	{
		//矢印描画
		DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_ * 3.0f, rotation_, png_arrow_, true);

		DrawFormatString(1000, 60, Util::Color::GREEN, "溜め状態");
		DrawFormatString(1000, 80, Util::Color::GREEN, "frame: %d/%d", frameCount_4Skewer_, kChargeFrame4Skewer_);
	}

	if (state_ == State::ATTACK_SKEWER)
	{
		skewer_.Draw();
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

	//　pad-Aを押していない時は移動できる。（串刺しの為に溜めてる時は動けない）
	if (!PadDownA())
	{
		// 移動後の座標 = 座標 + (正規化された入力値 * 速度)
		Vector2 moved_pos = position_ + input.Normalize() * kMoveSpeed_;

		// 移動後の座標 (+ 半径)が、ステージの内側なら移動できる
		if (moved_pos.x - radius_.x > stagePtr_->GetLT().x && moved_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
			moved_pos.x + radius_.x < stagePtr_->GetRB().x && moved_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
		{
			// 座標を移動後の値に
			position_ = moved_pos;
		}
	}


	// pad-RでAttack_MOW状態に遷移
	if (PadTriggerLorR())
	{
		mow_.Attack(vec_move_, position_, rotation_);
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
		}
		// 離した瞬間に初期化
		frameCount_4Skewer_ = 0;

		// スローモーション解除
		SceneManager::GetInstance()->EndSlowMotion();
	}

#ifdef _DEBUG
	// key-SPACEでAttack_MOW状態に遷移
	if (KEYS::IsTrigger(KEY_INPUT_SPACE))
	{
		mow_.Attack(vec_move_, position_, rotation_);
		state_ = State::ATTACK_MOW;
	}
#endif // _DEBUG
}

void Player::MowAttackUpdate(void)
{
	if (mow_.GetFrameCountAttack() == 0)
	{
		state_ = State::MOVE;
	}
	mow_.Update();
}

void Player::SkewerAttackUpdate(void)
{
	// isSkewerがfalseならMOVE状態へ遷移
	if (skewer_.GetIsSkewer() == false)
	{
		state_ = State::MOVE;
	}

	// 串刺し1フレーム後の座標 = 座標 + (正規化されたプレイヤーの向き * 速度)
	Vector2 skewered_pos = position_ + vec_move_ * skewer_.GetKMoveSpeed();

	// 串刺し1フレーム後の座標 (+ 半径)が、ステージの内側なら座標反映
	if (skewered_pos.x - radius_.x > stagePtr_->GetLT().x && skewered_pos.y - radius_.x > stagePtr_->GetLT().y && // 現在、半径は円としてxしか使っていないので
		skewered_pos.x + radius_.x < stagePtr_->GetRB().x && skewered_pos.y + radius_.x < stagePtr_->GetRB().y)   // yが使われていないのは意図的
	{
		// 反映
		position_ = skewered_pos;
	}
	else // 串刺し1フレーム後の座標 (+ 半径)が、ステージ外なら串刺し状態終了
	{
		skewer_.End(); // isSkewerをfalseにする。
	}

	// 串刺し1フレーム後の座標 + (正規化されたプレイヤーの向き * 規定距離)
	skewer_.SetPos(skewered_pos + vec_move_ * kSkewerAttackCenterDist_);
	skewer_.Update();
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
