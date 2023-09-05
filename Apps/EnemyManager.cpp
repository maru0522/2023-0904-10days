#include "EnemyManager.h"
#include "Keyboard.h"


EnemyManager& EnemyManager::GetInstance()
{
	static EnemyManager sInst;
	return sInst;
}

//---------------------------------------------
void EnemyManager::Initialize(Player* player)
{
	player_ = player;

	enemies_.clear();
	combinedEnemiesArray_.clear();
}

void EnemyManager::MowDownUpdate()
{
	//�ガ����ꂽ�G�����邩
	bool isMowDowned = false;

	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDown())
		{
			true;
			break;
		}
	}

	//�C���X�^���X����
	std::unique_ptr<CombinedEnemies>combinedEnemies = std::make_unique<CombinedEnemies>();
	//�擾�p�̔z��
	std::vector<std::unique_ptr<Enemy>>enemiesL;

	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDown())
		{
			//���������G�̔z��ɒǉ�����
			enemiesL.push_back(std::move(enemy));
		}
	}
	//������
	combinedEnemies->Initialize(player_, { 0,-1.0f }, std::move(enemiesL));

	//���������G�̉���Ǘ�����z��ɒǉ�
	combinedEnemiesArray_.push_back(std::move(combinedEnemies));
}

void EnemyManager::Update()
{
	for (auto& enemy : enemies_)
	{
		enemy->Update();
	}

	for (auto& combinedEnemies : combinedEnemiesArray_)
	{
		combinedEnemies->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}

	for (auto& combinedEnemies : combinedEnemiesArray_)
	{
		combinedEnemies->Draw();
	}
}


//-------------------------------------------------------------
void EnemyManager::AddEnemy(std::unique_ptr<Enemy> enemy)
{
	enemies_.push_back(std::move(enemy));
}
