#pragma once
#include "Player.h"
#include "IEntity.h"
#include <DxLib.h>

class Enemy final : public IEntity
{
public:
	// �萔
	static const float kMoveSpeed_; // ���x
	static const int32_t kMoveTimes_{ 5 }; // ����ɕ����Ĉړ����邩(�u�Ԉړ��Ɍ�����̂�����邽�߁j
	static const int32_t kMoveInterval_{ 100 }; // �G�������܂ł̊Ԋu(�t���[���P��)
	//static const float kMowRatio_; // ������΂��ꂽ�Ƃ������ɑ΂��āA�ǂ̂��炢�̊����ňړ����Ă�����
	static const float kPushBackDist_; // �����߂�����
	static const float kPngScale_; // �摜�̊g�嗦
	static const uint32_t kMowFrame_ = 20; // ���t���[�������Đ�����΂���邩
	static const float KRadius_; // ���a

	// �֐�
	Enemy(CollisionManger* colMPtr, Player* playerPtr, Stage* stagePtr);
	~Enemy(void) override;

	void Update(void) override;
	//�ガ�����֌W�̃A�b�v�f�[�g
	void MowDownFlagUpdate();

	void Draw(void) override;

private:
    // �Ȃ񂩕�����Ȃ����ǁA�����߂������ʂ��Ă邯�ǁA�G���m�������߂�����܂���B
	void OnCollision(void);

	// �ϐ�
	Player* playerPtr_;
	CollisionManger* colMPtr_;

	int32_t frameCount_wait_; // �ҋ@�p�t���[���J�E���g
	int32_t frameCount_move_; // �ړ��p�t���[���J�E���g

	bool isAlive_{ true }; // �����Ă邩
	//bool isContactMowSupport_{}; // �ガ�����̌��m�T�|�[�g�N���X�̓����蔻��ɐG�ꂽ��

	// �ガ����ꂽ��
	bool isMowDown_ = false;
	bool isMowDownTrigger_ = false;
	Vector2 vec_mow_{}; // ������΂�������
	// ������΂���鋗�����������ƈړ��������v������ϐ�
	//float tortalMowDist{};
	// ������΂���鎞�̃C�[�W���O�p�^�C�}�[
	int32_t frameCount_mow_;
	// �ガ����ꂽ�Œ��ɍ���OK�t���O
	bool isDocking_{};
	// �����A�����h������Ă܂��t���O
	bool isSkewer_;

	Vector2 scale_ = { 1.0f,1.0f };

	// resource
	int32_t png_enemy_ = LoadGraph("Resources/texture/enemy.png");

public:
	// setter
	void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }
	void SetIsMowDownTrigger(bool isMowDownTrigger) { isMowDownTrigger_ = isMowDownTrigger; }
	void SetIsDocking(bool isDocking) { isDocking_ = isDocking; }
	void SetIsSkewer(bool isSkewer) { isSkewer_ = isSkewer; }
	void SetScale(const Vector2& scale) { scale_ = scale; }

	// getter
	bool GetIsMowDown() { return isMowDown_; }
	bool GetIsMowDownTrigger() { return isMowDownTrigger_; }
	bool GetIsAlive(void) { return isAlive_; }
	bool GetIsDocking() { return isDocking_; }
	const Vector2& GetMowDownVec() { return vec_mow_; }
	bool GetIsSkewer(void) { return isSkewer_; }
	const Vector2& GetScale() { return scale_; }

public:
	//���̂����Ƃ��ɊO���ŌĂяo��
	void Docking();
	//���S���ɌĂяo��
	void Dead();
};

