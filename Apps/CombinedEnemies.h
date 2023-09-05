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


public:
	~CombinedEnemies();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="player_">player�|�C���^</param>
	/// <param name="direciton">�v���C���[���猩���G�̕���</param>
	/// <param name="enemies">�������G�̔z��</param>
	void Initialize(Player* player_, const Vector2& direciton, std::vector<std::unique_ptr<Enemy>>enemies);
	void Update();
	void Draw();

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
	//���������̍X�V
	void DirectionUpdate();

private:
	void EnemiesPosUpdate();
	void CalcCentorPos(const Vector2& targetPos, const Vector2& direciton);

public:
	const Vector2& GetCentorPos() { return centorPos_; }
	void SetCentorPos(const Vector2& centorPos) { centorPos_ = centorPos; }
	const Vector2& GetDireciton() { return direction_; }
	void SetDireciton(const Vector2& direciton) { direction_ = direciton; }
	float GetRadius() { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }

	void SetRadiusTmp() { radiusTmp_ = radius_; }
	void SetCentorPosTmp() { centorPosTmp_ = centorPos_; }
	void SetDirectionTmp() { directionTmp_ = direction_; }

	float GetRadiusTmp() { return radiusTmp_; }
	const Vector2& GetCentorPosTmp() { return centorPosTmp_; }
	const Vector2& GetDirectionTmp() { return directionTmp_; }

public:
	void ChangeState(const std::string& name);
};

