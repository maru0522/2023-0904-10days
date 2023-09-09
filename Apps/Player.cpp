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

const float Player::kMowDist_{ 15.f };// �ガ�����Ő�����΂����� �������ύX����Ȃ�enemy.h�̊������M��Ȃ��Əu�Ԉړ��ɂȂ����܂�

Player::Player(CollisionManger* colMPtr, Stage* stagePtr) : IEntity(stagePtr), mow_(colMPtr), mow_support_(colMPtr), skewer_(colMPtr), colMPtr_(colMPtr)
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

    // ������΂����t���O�I����������
    if (isKnockback_)
    {
        // �C�[�W���O�p�̃^�C�����[�g
        float rate = (std::min)((float)frameCount_knockback_ / kMaxKnockbackFrame_, 1.f);

        // ������΂���鑬�� �� �C�[�W���O�Œ���
        const float mowSpeed = (1 - Math::Ease::EaseInSine(rate)) * Player::kKnockbackDist_;
        // ���W�ɉ��Z
        position_ += vec_contactE2P_ * mowSpeed;

        if (rate >= 1.f)
        {
            frameCount_knockback_ = 0;
            isKnockback_ = false;
            return;
        }

        // �t���[���J�E���g���Z
        Math::Function::LoopIncrement<int32_t>(frameCount_knockback_, 0, kMaxKnockbackFrame_);
    }

    // �f�o�b�O���C���p�L�^
    pos4Line_ = position_ + vec_move_ * 30;

    // ���������]�p���擾
    rotation_ = std::acos(Vector2(0, -1).Dot(vec_move_));
    //���]���Ȃ��悤��
    if (vec_move_.x < 0)
    {
        rotation_ = -rotation_;
    }
    // �E�����������擾
    isRight_ = Vector2(0, -1).Cross(vec_move_.Normalize()) > 0.f;
}

void Player::Draw(void)
{
    // �`��
    DrawLineAA(position_.x, position_.y, pos4Line_.x, pos4Line_.y, Color::WHITE, 3);

    // ���݂̃v���C���[�̏�ԁi�����̂݁j
    DrawFormatString(0, 140, 0xffffff, "state ;%d", static_cast<int32_t>(state_));


    // skewer�ׂ̈Ƀ{�^�����������Ă�Ȃ�
    if (frameCount_4Skewer_ > 0)
    {
        //���`��
        Vector2 pos4PredictionLine = position_ + vec_move_ * 1000;

        DrawLine((int32_t)position_.x, (int32_t)position_.y, (int32_t)pos4PredictionLine.x, (int32_t)pos4PredictionLine.y, Color::RED, 2);
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_ * 3.0f, rotation_, png_arrow_, true);

        DrawFormatString(1000, 60, Util::Color::GREEN, "���ߏ��");
        DrawFormatString(1000, 80, Util::Color::GREEN, "frame: %d/%d", frameCount_4Skewer_, kChargeFrame4Skewer_);
    }
    else if (state_ != State::ATTACK_SKEWER) // ���h���U���̂��߂ɗ��߂Ă�Ԃ�A���h���U�����͔��~��\�����Ȃ� ������ȊO�̎��ɕ\��
    {
        // �U���͈͂�debug�̕\��
        mow_.Draw();
        mow_support_.Draw();
    }

    // skewer�ׂ̈Ƀ{�^�����������ĂȂ� && ���h���U�����łȂ� && �ガ�����U�����Ȃ�
    // �ガ������� && �ガ�����t���[���J�E���g��0�ȊO
    if (state_ == State::ATTACK_MOW && mow_.GetFrameCountAttack() > 1)
    {
        // ����`��
        DrawRotaGraph((int32_t)pos4Sword_.x, (int32_t)pos4Sword_.y, kPngScale_, rot4Sword2_, png_sword_, true);
        //DrawCircle((int32_t)pos4Sword_.x, (int32_t)pos4Sword_.y, 1, Util::Color::BLUE, true, 1);
    }
    DrawFormatString(1000, 120, Util::Color::GREEN, "rot4s: %f", rot4RotationSword_);

    if (state_ == State::ATTACK_SKEWER) // ���h���U�����A���h���̕`��֐����Ăяo��
    {
        pos4Sword_ += vec_move_ * 5;
        pos4SwordUp_ += vec_move_ * (6 + EnemyManager::GetInstance().GetSkewerEnemiesLength());
        pos4SwordBottom_ -= vec_move_ * 12;

        // ��
        DrawRotaGraph((int32_t)pos4Sword_.x, (int32_t)pos4Sword_.y, kPngScale_, rotation_, png_sword_, true);
        DrawRotaGraph((int32_t)pos4SwordUp_.x, (int32_t)pos4SwordUp_.y, kPngScale_, rotation_, png_swordUp_, true);
        DrawRotaGraph((int32_t)pos4SwordBottom_.x, (int32_t)pos4SwordBottom_.y, kPngScale_, rotation_, png_swordBottom_, true);
        skewer_.Draw();
    }

    // ���G���Ԓ��Ȃ�
    if (frameCount_invincible_ != 0)
    {
        //DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::YELLOW, true, 1);

        // player�̕`��
        SetDrawBright(255, 255, 80); // �ϐF�ʃJ�X
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_player_, true);
        SetDrawBright(255, 255, 255);
        DrawFormatString(1000, 20, Util::Color::YELLOW, "���G���");
        DrawFormatString(1000, 40, Util::Color::YELLOW, "frame: %d", kMaxInvincibleFrame_ - frameCount_invincible_);
    }
    else // ���G���Ԃ���Ȃ��Ȃ�
    {
        DrawFormatString(1000, 20, Util::Color::WHITE, "�ʏ���");
        //DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Color::WHITE, true, 1);

        // player�̕`��
        DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, rotation_, png_player_, true);
    }

    // ���h���U�����̔�����W
    DrawFormatString(1000, 100, Util::Color::GREEN, "pos(%f,%f)", skewer_.GetPos().x, skewer_.GetPos().y);
    DrawFormatString(0, 500, Util::Color::WHITE, "pos(%f,%f)", position_.x, position_.y);
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
    if (frameCount_4Skewer_ == 0) // ���h���J�E���g��0�Ȃ�i=���߂ĂȂ�)
    {
        // ���W += (���K�����ꂽ���͒l * ���x)
        position_ += input.Normalize() * kMoveSpeed_;

        // �����߂����[���A����ȏア���Ȃ��悤�ɂ��Ă���
        position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
        position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);
    }

#pragma region �ガ�����U���͈̔͂��ړ������Ă�
    mow_.SetPos(position_);
    mow_.SetRot(rotation_);
    // �T�|�[�g�̋�`�̒��S�_ = �v���C���[�̍��W + ����vec * kMowSupportCenterDist_
    Vector2 center4MowSupport = position_ + vec_move_ * kMowSupportCenterDist_;
    // �T�|�[�g�̍��W�ݒ�
    mow_support_.SetPos(center4MowSupport);
#pragma endregion

    // ���G������Ȃ���΍U���ł���
    if (frameCount_invincible_ == 0)
    {
        // pad-A�����ĂȂ��� && pad-R||RB ��Attack_MOW��ԂɑJ��
        if (PadDownA() == false && PadTriggerLorR() || PadTriggerRB())
        {
            mow_.Attack(vec_move_, position_);
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
                pos4Sword_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
                pos4SwordUp_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
                pos4SwordBottom_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
            }
            // �������u�Ԃɏ�����
            frameCount_4Skewer_ = 0;

            // �X���[���[�V��������
            SceneManager::GetInstance()->EndSlowMotion();
        }
    }

#ifdef _DEBUG
    // key-SPACE��Attack_MOW��ԂɑJ��
    if (KEYS::IsTrigger(KEY_INPUT_SPACE))
    {
        mow_.Attack(vec_move_, position_);
        state_ = State::ATTACK_MOW;
    }
#endif // _DEBUG
}

void Player::MowAttackUpdate(void)
{
    //�v���C���[�̑O�����~���ɂ���G�𐁂���΂��d�l
    // �����̂��߁A�v���C���[�̑O���ɒ����`�̓����蔻����o���āA���~��̓����蔻��ɂ������������Ă��𐁂���΂�

    // �U������̃t���[����0�ɂȂ�����
    if (mow_.GetFrameCountAttack() == 0)
    {
        // ��ԑJ��
        state_ = State::MOVE;
        // �֐��I��
        return;
    }

    // �t���[�����Z�Ői�s�������Z�o
    float rate = (std::min)((float)(mow_.GetFrameCountAttack() - 1) / PlayerMowAttack::kMaxAttackFrame_, 1.f);
    // �p�x�ō��ǂ̂��炢�����Ă͂߂� rad = ToRad(���� * 180��)
    rot4RotationSword_ = Math::Function::ToRadian(kMaxRangeSwordDegree_ * rate);

    // �v���C���[�̉E�������o��
    Vector3 vec3_move = { vec_move_.x,vec_move_.y,0 };
    Vector3 vec3_right = Vector3(0, 0, 1).Cross(vec3_move.Normalize());
    Vector2 vec2_right = { vec3_right.x,vec3_right.y };
    // �������W = �E���� * �K�苗��
    const Vector2 initPos = vec2_right * kMowSwordCenterDist_; // ��]���̏������W
    // ��]�ړ��̍��W�v�Z
    pos4Sword_.x = -(initPos.x * std::cos(rot4RotationSword_) - initPos.y * std::sin(rot4RotationSword_));
    pos4Sword_.y = -(initPos.x * std::sin(rot4RotationSword_) + initPos.y * std::cos(rot4RotationSword_));
    // ��]��v���C���[�̈ʒu�܂ňړ�������B
    pos4Sword_.x += position_.x;
    pos4Sword_.y += position_.y;

    // ���̊G���̂̉�]�p���v�Z����
    rot4Sword2_ = (rotation_ - Math::Function::ToRadian(90)) + rate * Math::Function::ToRadian(kMaxRangeSwordDegree_); // �p�x���傢�[�߂�

    // �ガ�����U���{�̂�Update()
    mow_.Update();
}

void Player::SkewerAttackUpdate(void)
{
    // isSkewer��false�Ȃ�MOVE��Ԃ֑J��
    if (skewer_.GetIsSkewer() == false)
    {
        state_ = State::MOVE;
        // ���肪���̏�Ɏc�葱�����Ⴄ����A��΂Ɉ���������Ȃ����W�ɓ]�����邲�艟���B pos(-10��,-10��)
        skewer_.SetPos({ -100000.f, -100000.f });
        // �֐��I��
        return;
    }

    // ���h���̏㔼����1�t���[����̍��W = 1�t���[���O�̏㔼���̍��W + (���K�����ꂽ�v���C���[�̌��� * ���x)
    Vector2 skewerd_swordUp_pos = pos4SwordUp_ /*+ vec_move_ * skewer_.GetKMoveSpeed()*/;

    // ���h��1�t���[����̍��W = ���W + (���K�����ꂽ�v���C���[�̌��� * ���x)
    Vector2 skewered_pos = position_ + vec_move_ * skewer_.GetKMoveSpeed();

    // ���h���̏㔼����1�t���[����̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ�v���C���[�̍��W���X�V
    if (skewerd_swordUp_pos.x - skewer_.GetRad().x > stagePtr_->GetLT().x && skewerd_swordUp_pos.y - skewer_.GetRad().y > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂ŁAy���g���Ă��Ȃ��͈̂Ӑ}�I
        skewerd_swordUp_pos.x + skewer_.GetRad().x < stagePtr_->GetRB().x && skewerd_swordUp_pos.y + skewer_.GetRad().y < stagePtr_->GetRB().y)   // [2023/09/10]�d�l���ς��A���̔���ʒu�Ŏ~�܂邩���f����̂ŁA��`�̔���������́Ay���g��
    {
        // ���f
        position_ = skewered_pos;
    }
    else // ���h��1�t���[����̍��W (+ ���a)���A�X�e�[�W�O�Ȃ���h����ԏI��
    {
        skewer_.End(); // isSkewer��false�ɂ���B
    }

    const float eRange = EnemyManager::GetInstance().GetSkewerEnemiesLength();

    // ���h��1�t���[����̍��W + (���K�����ꂽ�v���C���[�̌��� * (�K�苗�� + ���h���Ă�G��"���a"))
    skewer_.SetPos(skewered_pos + vec_move_ * (kSkewerAttackCenterDist_ + eRange));
    skewer_.Update();
    // ���h���G�̍��W = �v���C���[�̍��W + ���K�����ꂽ�v���C���[�̌��� * �K�苗��)
    pos4Sword_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
    pos4SwordUp_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
    pos4SwordBottom_ = position_ + vec_move_ * Player::kMowSwordCenterDist_;
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
        // ���h���U�����������߂��v���
        if (skewer_.GetIsSkewer()) return;

        // �G���玩���܂ł̕����x�N�g��
        vec_contactE2P_ = (position_ - other_->GetPos()).Normalize();

        // ���G���Ԓ��łȂ��Ȃ�
        if (frameCount_invincible_ == 0)
        {
            // �m�N�o���܁`��
            isKnockback_ = true;
            
            // ���G���Ԃɓ���
            frameCount_invincible_++;

            // ���������ɔ�e�����ꍇ���Z�b�g(?)
            if (frameCount_4Skewer_)
            {
                frameCount_4Skewer_ = 0;
                SceneManager::GetInstance()->EndSlowMotion();
            }
        }
        else // ���G���Ԓ��Ȃ牟���߂�
        {
            // �����߂���̍��W = ���W + (���K�����ꂽ�����߂����� * ���x)
            Vector2 pushBacked_pos = position_ + vec_contactE2P_ * kPushBackDist_;

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
