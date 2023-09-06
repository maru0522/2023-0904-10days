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
	// �Փ˃}�l�[�W���ւ̓o�^
	colMPtr->Register(this);

	// �`��ݒ�
	shape_ = Shape::CIRCLE;
	// ���̐ݒ�
	id_ = "player";

	// �Փ�callback���f
	onCollision_ = std::bind(&Player::OnCollision, this);
}

Player::~Player(void)
{
    // �o�^�̖���
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
}

void Player::Update(void)
{
	// ���G���Ԓ��Ȃ�
	if (frameCount_invincible_ != 0)
	{
		// ���G���Ԃ̃t���[���J�E���g�����Z
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
	//���]���Ȃ��悤��
	if (vec_move_.x < 0)
	{
		rotation_ = -rotation_;
	}
	isRight_ = Vector2(0, -1).Cross(vec_move_.Normalize()) > 0.f;
}

void Player::Draw(void)
{
	// �`��
	DrawLineAA(position_.x, position_.y, pos4Line_.x, pos4Line_.y, Color::WHITE, 3);

	// ���G���Ԓ��Ȃ�
	if (frameCount_invincible_ != 0)
	{
		//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::YELLOW, true, 1);

		// player�̕`��
		SetDrawBright(255, 255, 80); // �ϐF�ʃJ�X
		isRight_ ?
			DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_player_, true, false) :
			DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, -rotation_, png_player_, true);
		SetDrawBright(255, 255, 255);
		DrawFormatString(1000, 20, Util::Color::YELLOW, "���G���");
		DrawFormatString(1000, 40, Util::Color::YELLOW, "frame: %d", kMaxInvincibleFrame_ - frameCount_invincible_);
	}
	else
	{
		DrawFormatString(1000, 20, Util::Color::WHITE, "�ʏ���");
		//DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);

		// player�̕`��
		isRight_ ?
			DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_player_, true, false) :
			DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, -rotation_, png_player_, true);
	}

	if (state_ == State::ATTACK_MOW)
	{
		mow_.Draw();
	}

	// skewer�ׂ̈Ƀ{�^�����������Ă�Ȃ�
	if (frameCount_4Skewer_ > 0)
	{
		//���`��
		DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_ * 3.0f, rotation_, png_arrow_, true);

		DrawFormatString(1000, 60, Util::Color::GREEN, "���ߏ��");
		DrawFormatString(1000, 80, Util::Color::GREEN, "frame: %d/%d", frameCount_4Skewer_, kChargeFrame4Skewer_);
	}

	if (state_ == State::ATTACK_SKEWER)
	{
		skewer_.Draw();
	}
}

void Player::MoveUpdate(void)
{
	// ����
	Vector2 input{};
	input += PadStick();
#ifdef _DEBUG
	input.x += KEYS::IsDown(KEY_INPUT_D) - KEYS::IsDown(KEY_INPUT_A);
	input.y += KEYS::IsDown(KEY_INPUT_S) - KEYS::IsDown(KEY_INPUT_W);
#endif // _DEBUG

	// ���͂����������̂݁A�x�N�g�����L�^
	if (input.IsNonZero())
	{
		// �ړ������x�N�g�����L�^
		vec_move_ = input.Normalize();
	}

	//�@pad-A�������Ă��Ȃ����͈ړ��ł���B�i���h���ׂ̈ɗ��߂Ă鎞�͓����Ȃ��j
	if (!PadDownA())
	{
		// �ړ���̍��W = ���W + (���K�����ꂽ���͒l * ���x)
		Vector2 moved_pos = position_ + input.Normalize() * kMoveSpeed_;

		// �ړ���̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ�ړ��ł���
		if (moved_pos.x - radius_.x > stagePtr_->GetLT().x && moved_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
			moved_pos.x + radius_.x < stagePtr_->GetRB().x && moved_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
		{
			// ���W���ړ���̒l��
			position_ = moved_pos;
		}
	}


	// pad-R��Attack_MOW��ԂɑJ��
	if (PadTriggerLorR())
	{
		mow_.Attack(vec_move_, position_, rotation_);
		state_ = State::ATTACK_MOW;
	}
	// pad-A��������ATTACK_SKEWER��ԂɑJ��
	if (PadDownA())
	{
		// ATTACK_SKEWER��Ԃɓ��邽�߂̗��ߌv���t���[�������Z
		//frameCount_4Skewer_++;
		frameCount_4Skewer_ += 5; // �X���[���[�V��������̂��ߗ͋Z������5�t���[�������J�E���g���܂��B

		// ���d�l�㉟���Ă���X���[���[�V�����J�n�ɂȂ�̂ŁA�ŏ��̃t���[�����J�E���g�� +n ����Ă��܂��̂��ȒP�ɉ��������@�v�����܂���B

		// �X���[���[�V�����J�n
		SceneManager::GetInstance()->StartSlowMotion();
	}
	else
	{
		// �K��t���[���ȏ�G��Ă���J��
		if (frameCount_4Skewer_ >= kChargeFrame4Skewer_)
		{
			// �J�ڂ��ď�����
			skewer_.Attack();
			state_ = State::ATTACK_SKEWER;
			frameCount_4Skewer_ = 0;
		}
		// �������u�Ԃɏ�����
		frameCount_4Skewer_ = 0;

		// �X���[���[�V��������
		SceneManager::GetInstance()->EndSlowMotion();
	}

#ifdef _DEBUG
	// key-SPACE��Attack_MOW��ԂɑJ��
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
	// isSkewer��false�Ȃ�MOVE��Ԃ֑J��
	if (skewer_.GetIsSkewer() == false)
	{
		state_ = State::MOVE;
	}

	// ���h��1�t���[����̍��W = ���W + (���K�����ꂽ�v���C���[�̌��� * ���x)
	Vector2 skewered_pos = position_ + vec_move_ * skewer_.GetKMoveSpeed();

	// ���h��1�t���[����̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
	if (skewered_pos.x - radius_.x > stagePtr_->GetLT().x && skewered_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
		skewered_pos.x + radius_.x < stagePtr_->GetRB().x && skewered_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
	{
		// ���f
		position_ = skewered_pos;
	}
	else // ���h��1�t���[����̍��W (+ ���a)���A�X�e�[�W�O�Ȃ���h����ԏI��
	{
		skewer_.End(); // isSkewer��false�ɂ���B
	}

	// ���h��1�t���[����̍��W + (���K�����ꂽ�v���C���[�̌��� * �K�苗��)
	skewer_.SetPos(skewered_pos + vec_move_ * kSkewerAttackCenterDist_);
	skewer_.Update();
}

void Player::OnCollision(void)
{
	// �ڐG�Ώۂ̖��̂� enemy
	if (other_->GetId() == "enemy")
	{
		// �L���X�g����enemy�Ƃ��Ď擾
		Enemy* enemyPtr = static_cast<Enemy*>(other_);

		// �G����ł�Ȃ牟���߂��v���
		if (enemyPtr->GetIsAlive() == false) return;

		// �G���玩���܂ł̕����x�N�g��
		Vector2 vec_enemy2player = (position_ - other_->GetPos()).Normalize();

		// ���G���Ԓ��łȂ��Ȃ�
		if (frameCount_invincible_ == 0)
		{
			// �m�b�N�o�b�N��̍��W = ���W + (���K�����ꂽ�m�b�N�o�b�N���� * ���x)
			Vector2 knockbacked_pos = position_ + vec_enemy2player * kKnockbackDist_;

			// �m�b�N�o�b�N��̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
			if (knockbacked_pos.x - radius_.x > stagePtr_->GetLT().x && knockbacked_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
				knockbacked_pos.x + radius_.x < stagePtr_->GetRB().x && knockbacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
			{
				// ���f
				position_ = knockbacked_pos;
				// ���G���Ԃɓ���
				frameCount_invincible_++;
			}
		}
		else // ���G���Ԓ��Ȃ牟���߂�
		{
			// �����߂���̍��W = ���W + (���K�����ꂽ�����߂����� * ���x)
			Vector2 pushBacked_pos = position_ + vec_enemy2player * kPushBackDist_;

			// �m�b�N�o�b�N��̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
			if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
				pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
			{
				// ���f
				position_ = pushBacked_pos;
			}
		}
	}
}
