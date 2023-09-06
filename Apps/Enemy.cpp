#include "Enemy.h"
#include "DxLib.h"
#include <cstdint>
#include "Util.h"
#include "MathUtil.h"
#include "PlayerAttack.h"
#include "Vector3.h"
#include "ParticleManager.h"

const float Enemy::kMoveSpeed_ = 30.0f;

Enemy::Enemy(CollisionManger* colMPtr, Player* playerPtr, Stage* stagePtr) : IEntity(stagePtr), playerPtr_(playerPtr), colMPtr_(colMPtr)
{
    // �Փ˃}�l�[�W���ւ̓o�^
    colMPtr->Register(this);

    // �`��ݒ�
    shape_ = Shape::CIRCLE;
    // ���̐ݒ�
    id_ = "enemy";
    // �Փ�callback���f
    onCollision_ = std::bind(&Enemy::OnCollision, this);

    // �ҋ@�t���[�����������ɂ΂炯��悤��
    frameCount_wait_ = Math::Function::Random<int32_t>(5, kMoveInterval_ - 5);
}

Enemy::~Enemy(void)
{
    // �o�^�̖���
    onCollision_ = nullptr;
    colMPtr_->UnRegister(this);
}

void Enemy::Update(void)
{
    // �����Ă�Ȃ�
    if (isAlive_)
    {
        // �O�t���[�����玝���z���͂��Ȃ��B
        isContactMowSupport_ = false;

        // �ガ�����Ă���
        if (isMowDown_)
        {
            // ������я����ƈړ�������isMowDown�̃I�t

            // ������΂���鑬��
            const float mowSpeed = Player::kKnockbackDist_ / kMowRatio_;
            // ���W�ɉ��Z
            position_ += vec_mow_ * mowSpeed;
            // �Ƃ肠�������񂾂��ړ����܂����̂ŁA���v�ɉ��Z���܂�
            tortalMowDist += mowSpeed;

            // �����߂����[���A����ȏア���Ȃ��悤�ɂ��Ă���
            position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
            position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);

            // ���W�̂��ꂱ��I������̂ŁA���v���ӂ݂āA�I��邩�ǂ����m�F���܂��B
            if (tortalMowDist >= Player::kKnockbackDist_)
            {
                tortalMowDist = 0;
                isMowDown_ = false;
            }
        }
        else // �ガ�����ĂȂ����͂����ʂ肷�����Ă���Č��\
        {

            // �ړ��p�t���[����0�Ȃ�(�ړ����I������Ȃ�)
            if (frameCount_move_ == 0)
            {
                // �ҋ@�pframe�����Z�i�k�ݏ�ԁj
                Math::Function::LoopIncrement<int32_t>(frameCount_wait_, 0, kMoveInterval_);
            }

            // �ҋ@�p�t���[�����K��l�ȏ�Ȃ�i�ҋ@���I������Ȃ�j
            if (frameCount_wait_ >= kMoveInterval_)
            {
                // �ړ��p�t���[�������Z�i�L�я�ԁj���ő�t���[�����͈ړ��񐔂Ɠ���
                Math::Function::LoopIncrement<int32_t>(frameCount_move_, 0, kMoveTimes_);

                // �G(���g)����v���C���[�܂ł̕����x�N�g��
                Vector2 vec_enemy2player = (playerPtr_->GetPos() - position_).Normalize();

                // �ړ���̍��W = ���W + (���K�����ꂽ���͒l * (���x/�ړ���))
                Vector2 moved_pos = position_ + vec_enemy2player * (kMoveSpeed_ / (float)kMoveTimes_);

                // �ړ���̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ�ړ��ł���
                if (moved_pos.x - radius_.x > stagePtr_->GetLT().x && moved_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
                    moved_pos.x + radius_.x < stagePtr_->GetRB().x && moved_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
                {
                    // ���W���ړ���̒l��
                    position_ = moved_pos;
                }
            }
        }
    }
}

void Enemy::Draw(void)
{
    // �����Ă�Ȃ�
    if (isAlive_)
    {
        // �k�ݏ�ԂȂ�
        if (frameCount_move_ == 0)
        {
            // �G�̐F�͐ԐF��
            //DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::RED, true, 1);
            SetDrawBright(255, 50, 50);
            DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, 0.f, png_enemy_, true);
            SetDrawBright(255, 255, 255);
        }

        // �L�я�ԂȂ�
        if (frameCount_wait_ >= kMoveInterval_)
        {
            // �G�̐F�͗ΐF��
            //DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, true, 1);
            SetDrawBright(50, 255, 50);
            DrawRotaGraph((int32_t)position_.x, (int32_t)position_.y, kPngScale_, 0.f, png_enemy_, true);
            SetDrawBright(255, 255, 255);
        }
    }
}

void Enemy::OnCollision(void)
{
    // �ڐG�Ώۂ̖��̂� player_skewerAttack
    if (other_->GetId() == "player_skewerAttack")
    {
        ParticleManager::GetInstance()->BossHitEnemyEffect(position_);
        isAlive_ = false;
    }

    // �ڐG�Ώۂ̖��̂� enemy
    if (other_->GetId() == "enemy")
    {
        // �ガ�����Ă�Œ��ɑ��̓G�ƐG�ꂽ��
        if (isMowDown_)
        {
            Enemy* enemyPtr = static_cast<Enemy*>(other_);

            // ����OK�t���O�I��
            isDocking_ = true;
            enemyPtr->isDocking_ = true;
            // �֐��I��
            return;
        }

        // ���̓G���玩���܂ł̕����x�N�g��
        Vector2 vec_enemy2enemyself = (position_ - other_->GetPos()).Normalize();

        // �����߂���̍��W = ���W + (���K�����ꂽ�����߂����� * ���x)
        Vector2 pushBacked_pos = position_ + vec_enemy2enemyself * kPushBackDist_;

        // �m�b�N�o�b�N��̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
        if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
            pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
        {
            // ���f
            position_ = pushBacked_pos;
        }
    }

    // �ڐG�Ώۂ̖��̂� player
    if (other_->GetId() == "player")
    {
        // player���玩���܂ł̕����x�N�g��
        Vector2 vec_player2enemy = (position_ - other_->GetPos()).Normalize();

        // �����߂���̍��W = ���W + (���K�����ꂽ�����߂����� * ���x)
        Vector2 pushBacked_pos = position_ + vec_player2enemy * kPushBackDist_;

        // �m�b�N�o�b�N��̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
        if (pushBacked_pos.x - radius_.x > stagePtr_->GetLT().x && pushBacked_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
            pushBacked_pos.x + radius_.x < stagePtr_->GetRB().x && pushBacked_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
        {
            // ���f
            position_ = pushBacked_pos;
        }
    }

    // �ڐG�Ώۂ̖��̂� player_mowAttackSupport
    if (other_->GetId() == "player_mowAttackSupport") // ��΂�support����ɐG���B�t�ɐG��Ȃ��Ɛ���Ȕ���͊��҂ł��Ȃ�
    {
        isContactMowSupport_ = true;
    }

    // �ڐG�Ώۂ̖��̂� player_attack && support�Ɋ��ɐG��Ă������
    if (other_->GetId() == "player_mowAttack" && isContactMowSupport_)
    {
        // ptr���L���X�g���ĕ���
        PlayerMowAttack* paPtr = static_cast<PlayerMowAttack*>(other_);

        // �U���P�\���Ȃ�
        if (paPtr->GetFrameCountAttack()) //** ���󉺋L�̏��������ƁA�͈͓��ɒ��S�_�͂��Ȃ����ǁA���a�͐G��Ă���݂����ȏ�Ԃ��l������Ă��Ȃ��B�������킩��Ȃ��B
        {
            // �ガ����ꂽ�t���O�I��
            isMowDown_ = true;
            // ������΂����������L�^
            vec_mow_ = paPtr->GetVecMove();
        }
    }
}
