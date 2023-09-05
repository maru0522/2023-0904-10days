#include "Enemy.h"
#include "DxLib.h"
#include <cstdint>
#include "Util.h"
#include "MathUtil.h"
#include "PlayerAttack.h"

Enemy::Enemy(CollisionManger* colMPtr, Player* playerPtr, Stage* stagePtr) : IEntity(stagePtr), playerPtr_(playerPtr)
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

void Enemy::Update(void)
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

void Enemy::Draw(void)
{
    // �k�ݏ�ԂȂ�
    if (frameCount_move_ == 0)
    {
        // �G�̐F�͐ԐF��
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::RED, true, 1);
    }

    // �L�я�ԂȂ�
    if (frameCount_wait_ >= kMoveInterval_)
    {
        // �G�̐F�͗ΐF��
        DrawCircle((int32_t)position_.x, (int32_t)position_.y, (int32_t)radius_.x, Util::Color::GREEN, true, 1);
    }
}

void Enemy::OnCollision(void)
{
    // �ڐG�Ώۂ̖��̂� enemy
    if (other_->GetId() == "enemy")
    {
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

    // �ڐG�Ώۂ̖��̂� player_attack
    if (other_->GetId() == "player_mowAttack")
    {
        // ptr���L���X�g���ĕ���
        PlayerMowAttack* paPtr = static_cast<PlayerMowAttack*>(other_);

        // �U���P�\���Ȃ�
        if (paPtr->GetFrameCountAttack())
        {
            // ������΂��ꂽ��̍��W = ���W + (���K�����ꂽ������ԕ��� * ���x)�@��������ԕ��� = �v���C���[�̈ړ�����
            Vector2 blownAway_pos = position_ + paPtr->GetVecMove() * paPtr->GetKBlewDist();

            // �m�b�N�o�b�N��̍��W (+ ���a)���A�X�e�[�W�̓����Ȃ���W���f
            if (blownAway_pos.x - radius_.x > stagePtr_->GetLT().x && blownAway_pos.y - radius_.x > stagePtr_->GetLT().y && // ���݁A���a�͉~�Ƃ���x�����g���Ă��Ȃ��̂�
                blownAway_pos.x + radius_.x < stagePtr_->GetRB().x && blownAway_pos.y + radius_.x < stagePtr_->GetRB().y)   // y���g���Ă��Ȃ��͈̂Ӑ}�I
            {
                // ���f
                position_ = blownAway_pos;
            }
        }
    }
}
