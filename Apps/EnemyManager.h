#pragma once
#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"


//�G�Ǘ��N���X
class EnemyManager final
{
private://�ϐ�
	//�G�̔z��
	std::vector<std::unique_ptr<Enemy>>enemies_;
	//���������G���m���܂Ƃ߂��z��
	std::vector<std::unique_ptr<CombinedEnemies>> combinedEnemiesArray_;
	//�v���C���[�̃|�C���^
	Player* player_ = nullptr;

private://�֐�
	EnemyManager() { ; }
	~EnemyManager() { ; }

public:
	EnemyManager& operator=(const EnemyManager& inst) = delete;
	EnemyManager(const EnemyManager& inst) = delete;

	static EnemyManager& GetInstance();

public:
	void Initialize(Player* player);
	void Update();
	void Draw();

private:
	void MowDownUpdate();

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
};


