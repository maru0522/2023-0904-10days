#pragma once
#include"Enemy.h"
#include <memory>
#include <vector>

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
	//�S�̂̒���
	float length_ = 1.0f;
	//

public:

};

