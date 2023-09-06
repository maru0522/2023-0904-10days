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
	uint32_t count = 0;

	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDown())
		{
			count++;
			//二体以上なら
			if (count >= 2)
			{
				isMowDowned = true;
			}
		}
	}

	if (isMowDowned)
	{
		//インスタンス生成
		std::unique_ptr<CombinedEnemies>combinedEnemies = std::make_unique<CombinedEnemies>();
		//取得用の配列
		std::vector<std::unique_ptr<Enemy>>enemiesL;

		for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
		{
			if (itr->get()->GetIsMowDown())
			{
				//追加したのでオフ
				itr->get()->SetIsMowDown(false);
				//くっついた敵の配列に追加する
				enemiesL.push_back(std::move(*itr));
				//元の配列から削除
				enemies_.erase(itr);

				//エラー回避
				if (enemies_.size())
				{
					itr = enemies_.begin();
				}
				else
				{
					break;
				}
			}
		}
		//初期化
		combinedEnemies->Initialize(player_, player_->GetDirectionVec(), std::move(enemiesL));

		//くっついた敵の塊を管理する配列に追加
		combinedEnemiesArray_.push_back(std::move(combinedEnemies));
	}
}

void EnemyManager::Update()
{
	//くっつく敵がいるかの更新処理
	MowDownUpdate();

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
