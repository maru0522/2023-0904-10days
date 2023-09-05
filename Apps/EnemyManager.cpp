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
	//薙ぎ払われた敵がいるか
	bool isMowDowned = false;

	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDown())
		{
			true;
			break;
		}
	}

	//インスタンス生成
	std::unique_ptr<CombinedEnemies>combinedEnemies = std::make_unique<CombinedEnemies>();
	//取得用の配列
	std::vector<std::unique_ptr<Enemy>>enemiesL;

	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDown())
		{
			//くっついた敵の配列に追加する
			enemiesL.push_back(std::move(enemy));
		}
	}
	//初期化
	combinedEnemies->Initialize(player_, { 0,-1.0f }, std::move(enemiesL));

	//くっついた敵の塊を管理する配列に追加
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
