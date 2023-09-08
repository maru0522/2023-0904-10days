#pragma once
#include"Enemy.h"
#include <memory>
#include <vector>


class ICombinedEnemiesState;

//���������G����
class CombinedEnemies
{
private:
	//�ꏏ�ɂ������Ă�G�̔z��
	std::vector<std::unique_ptr<Enemy>>enemies_;
	//���̂��Ă�G�̒��S���W
	Vector2 centorPos_ = { 0,0 };
	//�G�S�̂̌�������
	Vector2 direction_ = { 0,0 };
	//�G�̐�
	uint32_t enemiesNum_ = 0;
	//�����X�s�[�h�̊�
	const float MOVE_SPEED_TMP_ = Enemy::kMoveSpeed_;
	//�S�̂̔��a
	float radius_ = 0.0f;
	//�v���C���[�܂ł̋���
	float distance_ = 0;
	const int32_t MOVE_TIMES_ = Enemy::kMoveTimes_; // ����ɕ����Ĉړ����邩(�u�Ԉړ��Ɍ�����̂�����邽�߁j
	const int32_t MOVE_INTERVAL_ = Enemy::kMoveInterval_; // �G�������܂ł̊Ԋu(�t���[���P��)
	//�v���C���[�̃|�C���^�[
	Player* player_ = nullptr;
	int32_t frameCountWait_; // �ҋ@�p�t���[���J�E���g
	int32_t frameCountMove_; // �ړ��p�t���[���J�E���g
	//�X�e�[�g
	std::unique_ptr<ICombinedEnemiesState>state_ = nullptr;
	//
	float radiusTmp_;
	Vector2 centorPosTmp_;
	Vector2 directionTmp_;
	//�ガ��������
	Vector2 mowDownVec_ = {0,0};
	//�ガ�����Ă邩
	bool isMowDown_ = false;
	//���h���ɂ���Ă��邩
	bool isSkewer_ = false;
	//�����Ă��邩
	bool isAlive_ = true;


public:
	~CombinedEnemies();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="player_">player�|�C���^</param>
	/// <param name="direciton">�v���C���[���猩���G�̕���</param>
	/// <param name="enemies">�������G�̔z��</param>
	void Initialize(Player* player_, const Vector2& direciton);
	void Update();
	void Draw();

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies>combinedEneies);
	//���������̍X�V
	void DirectionUpdate();
	//�G��̈�̂̈ʒu�X�V
	void EnemiesPosUpdate();
	//�X�P�[�������ɖ߂�
	void EnemiesScaleReset();

private:
	//�p�x�̍X�V
	void EnemiesRotUpdate();
	//�X�P�[�����Z�b�g
	void SetEnemiesScale(const Vector2& scale);
	//
	void CalcCentorPos(const Vector2& targetPos, const Vector2& direciton);
	//�������Ă�G�̂ǂꂩ��ł��ガ����ꂽ��
	void AnyEnemyMowDownUpdate();
	//�ガ����
	void MowDown();
	//�������Ă�G�̂ǂꂩ��ł��ːi���ꂽ��
	void AnyEnemySkewerUpdate();
	//�ːi����鏈��
	void Skewer();
	//�ガ����ꂽ�u�ԃt���O�̍X�V
	void EnemiesMowDownTriggerUpdate();

public:
	const Vector2& GetCentorPos() { return centorPos_; }
	void SetCentorPos(const Vector2& centorPos) { centorPos_ = centorPos; }
	const Vector2& GetDireciton() { return direction_; }
	void SetDireciton(const Vector2& direciton) { direction_ = direciton; }
	float GetRadius() { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }

	void SetRadiusTmp() { 
		radiusTmp_ = radius_; 
	}
	void SetCentorPosTmp() { centorPosTmp_ = centorPos_; }
	void SetDirectionTmp() { directionTmp_ = direction_; }
	void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }
	void SetIsSkewer(bool isSkewer) { isSkewer_ = isSkewer; }

	float GetRadiusTmp() { return radiusTmp_; }
	const Vector2& GetCentorPosTmp() { return centorPosTmp_; }
	const Vector2& GetDirectionTmp() { return directionTmp_; }

	int32_t GetEnemiesNum() { return enemiesNum_; }

	const Vector2& GetMowDownVec() { return mowDownVec_; }
	bool GetIsMowDown() { return isMowDown_; }
	bool GetIsSkewer() { return isSkewer_; }
	bool GetIsAlive() { return isAlive_; }

	const Vector2& GetTargetPos() { return player_->GetPos(); }

	//�z��̎Q��
	std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }

public:
	//�G�S�̂̒��łق��̓G�ƍ��̂������̂����邩
	bool GetIsDockingAnyEnemy();
	//�ガ�������ꂽ�u�Ԃ����邩
	bool GetIsMowDownTriggerAnyEnemy();
	//�G�S�̂̒��łق��̋��h���ɂ���Ă�G�ƍ��̂������̂����邩
	bool GetIsDockingAndSkewer();
	//�ق��̓G�ƍ��̂����Ƃ���isDocking��false
	void AllEnemiesDockingEnd();
	//�ړ����I���A�S���̓G�̓ガ�����t���O���I�t
	void AllEnemiesEndMowDown();
	//�ガ�����I���
	void MowDownEnd();
	//�ガ����ꂽ�u�ԏI���
	void MowDownTriggerEnd();
	//�ːi����Ă�Ƃ��̍X�V
	void SkewerUpdate();
	//���S����
	void Dead();
	//�X�P�[���h�炷
	void SetScaleSinRot(float minS, float maxS, float rate, int32_t timer);

public:
	void ChangeState(const std::string& name);
};

