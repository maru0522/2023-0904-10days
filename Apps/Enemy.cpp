#include "Enemy.h"
#include "DxLib.h"
#include <cstdint>
#include "Util.h"
#include "MathUtil.h"
#include "PlayerAttack.h"
#include "Vector3.h"
#include "ParticleManager.h"
#include "Score.h"
#include <cmath>

const float Enemy::kMoveSpeed_ = 30.0f;
const float Enemy::kPushBackDist_ = 2.0f;
const float Enemy::kPngScale_ = 0.07f;
const float Enemy::KRadius_ = 16.f;

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
        //isContactMowSupport_ = false;

        // �ガ�����Ă���
        if (isMowDown_)
        {
            // �C�[�W���O�^�C�}�[���N������Ă���
            if (frameCount_mow_)
            {
                // ���Z
                Math::Function::LoopIncrement<int32_t>(frameCount_mow_, 0, kMowFrame_);
            }
            else
            {
                // �N������ĂȂ���΋N��
                frameCount_mow_++;
            }

            /* ������я����ƈړ�������isMowDown�̃I�t */

            // �C�[�W���O�p�̃^�C�����[�g
            float rate = (std::min)((float)frameCount_mow_ / kMowFrame_, 1.f);

            // ������΂���鑬�� �� �C�[�W���O�Œ���
            const float mowSpeed = (1 - Math::Ease::EaseInSine(rate)) * Player::kMowDist_;
            // ���W�ɉ��Z
            position_ += vec_mow_ * mowSpeed;

            // �����߂����[���A����ȏア���Ȃ��悤�ɂ��Ă���
            position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
            position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);

            // ���W�̂��ꂱ��I������̂ŁA���v���ӂ݂āA�I��邩�ǂ����m�F���܂��B
            // rate��1�Ȃ�C�[�W���O�I����Ă܂�
            if (rate >= 1.f)
            {
                //tortalMowDist = 0;
                // ������΂���邽�߂̃J�E���^������
                frameCount_mow_ = 0;
                isMowDown_ = false;
            }
        }
        else if (!isDocking_)// �ガ�����ĂȂ����͂����ʂ肷�����Ă���Č��\
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
                // �G(���g)����v���C���[�܂ł̕����x�N�g��
                Vector2 vec_enemy2player = (playerPtr_->GetPos() - position_).Normalize();

                // �C�[�W���O�p�̃^�C�����[�g
                float rate = (std::min)((float)frameCount_move_ / kMoveTimes_, 1.f);

                // ������΂���鑬�� �� �C�[�W���O�Œ���
                const float speed = (1 - Math::Ease::EaseInSine(rate)) * Player::kMowDist_;
                // ���W�ɉ��Z
                position_ += vec_enemy2player * speed;

                // �����߂����[���A����ȏア���Ȃ��悤�ɂ��Ă���
                position_.x = Math::Function::Clamp<float>(position_.x, stagePtr_->GetLT().x + radius_.x * 2, stagePtr_->GetRB().x - radius_.x * 2);
                position_.y = Math::Function::Clamp<float>(position_.y, stagePtr_->GetLT().y + radius_.x * 2, stagePtr_->GetRB().y - radius_.x * 2);

                // ���W�̂��ꂱ��I������̂ŁA���v���ӂ݂āA�I��邩�ǂ����m�F���܂��B
                // rate��1�Ȃ�C�[�W���O�I����Ă܂�
                if (rate >= 1.f)
                {
                    //tortalMowDist = 0;
                    // ������΂���邽�߂̃J�E���^������
                    frameCount_mow_ = 0;
                    isMowDown_ = false;
                }

                // �ړ��p�t���[�������Z�i�L�я�ԁj���ő�t���[�����͈ړ��񐔂Ɠ���
                Math::Function::LoopIncrement<int32_t>(frameCount_move_, 0, kMoveTimes_);
            }
        }
    }

    MowDownFlagUpdate();
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
            SetDrawBright(255, 50, 50);
            DrawRotaGraph3((int32_t)position_.x, (int32_t)position_.y, 250, 250,
                scale_.x * kPngScale_, scale_.y * kPngScale_, rotation_, png_enemy_, true);
            SetDrawBright(255, 255, 255);
#ifdef _DEBUG
            DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::WHITE, false, 1);
#endif
        }

        // �L�я�ԂȂ�
        if (frameCount_wait_ >= kMoveInterval_)
        {
            // �G�̐F�͗ΐF��
            SetDrawBright(50, 255, 50);
            DrawRotaGraph3((int32_t)position_.x, (int32_t)position_.y, 250, 250,
                scale_.x * kPngScale_, scale_.y * kPngScale_, rotation_, png_enemy_, true);
            SetDrawBright(255, 255, 255);
#ifdef _DEBUG
            DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, false, 1);
#endif // _DEBUG
        }
    }
}

void Enemy::MowDownFlagUpdate()
{
    //�G�̍U�����I������獇�̂��Ȃ��悤��
    if (!playerPtr_->GetFrameCountAttack())
    {
        isMowDownTrigger_ = false;
    }
}

void Enemy::OnCollision(void)
{
    // �ڐG�Ώۂ̖��̂� player_skewerAttack
    if (other_->GetId() == "player_skewerAttack")
    {
        // �L���X�g���ĕ���
        PlayerSkewerAttack* skewerPtr = static_cast<PlayerSkewerAttack*>(other_);
        // ���h���U�����͎��ȂȂ�
        if (skewerPtr->GetIsSkewer())
        {
            isSkewer_ = true;
        }
        else
        {
            Dead();
        }

    }

    // �ڐG�Ώۂ̖��̂� enemy
    if (other_->GetId() == "enemy")
    {
        Enemy* enemyPtr = static_cast<Enemy*>(other_);

        // �ガ�����Ă�Œ��ɑ��̓G�ƐG�ꂽ��
        if (isMowDown_ || isSkewer_)
        {
            // ����OK�t���O�I��
            isDocking_ = true;
            enemyPtr->isDocking_ = true;
            // �֐��I��
            return;
        }
        //���������ꒆ�ɂق��̓G�ƐG�ꂽ��
        if (isSkewer_)
        {
            //����������������
            enemyPtr->isSkewer_ = true;
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
        // �L���X�g���ĕ���
        Player* playerPtr = static_cast<Player*>(other_);

        // ���h������Ă�Œ��Ȃ牟���߂��v���
        if (playerPtr->GetIsSkewer()) return;

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
        //isContactMowSupport_ = true;
    }

    // �ڐG�Ώۂ̖��̂� player_attack && support�Ɋ��ɐG��Ă������
    if (other_->GetId() == "player_mowAttack"/* && isContactMowSupport_*/)
    {
        // �ガ�����̔��~�ɐG��Ă����ԂŁA�v���C���[�̌����ƁA�v���C���[����G�ւ̌����̓��ϒl���A���̒l�̏ꍇ�A���ʂ̔��~���ɂ���Ǝv����B
        if (playerPtr_->GetMoveVec().Normalize().Dot((position_ - playerPtr_->GetPos()).Normalize()) >= 0.f)
        {
            // ptr���L���X�g���ĕ���
            PlayerMowAttack* paPtr = static_cast<PlayerMowAttack*>(other_);

            // �U���P�\���Ȃ�
            if (paPtr->GetFrameCountAttack()) //** ���󉺋L�̏��������ƁA�͈͓��ɒ��S�_�͂��Ȃ����ǁA���a�͐G��Ă���݂����ȏ�Ԃ��l������Ă��Ȃ��B�������킩��Ȃ��B
            {
                // ���W���A�����蔻��~�̒��S(=�v���C���[�̒��S���W�j+ �v���C���[�̐��� * (�v���C���[�̔��a + 5.f)�ɐݒ�
                position_ = paPtr->GetPos() + paPtr->GetVecMove() * 15.f;
                //position_ = { 300,300 };

                // �ガ����ꂽ�t���O�I��
                isMowDown_ = true;
                isMowDownTrigger_ = true;
                // ������΂����������L�^
                vec_mow_ = paPtr->GetVecMove();
            }
        }
    }
}

void Enemy::Docking()
{
    isDocking_ = false;
    vec_mow_ = { 0,0 };
}

void Enemy::Dead()
{
    if (isAlive_)
    {
        ParticleManager::GetInstance()->BossHitEnemyEffect(position_);
        Score::Add(500);
    }
    isAlive_ = false;
}
