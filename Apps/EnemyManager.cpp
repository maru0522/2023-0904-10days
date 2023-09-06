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
	bool isMowDownedComb = false;
	//くっついてる敵で合体したものをまとめる配列
	std::unique_ptr<CombinedEnemies> combEnemies = nullptr;
	uint32_t count = 0;

	//くっついてる敵の中でほかのくっついてる敵と合体したか
	if (combinedEnemiesArray_.size() >= 2)
	{

		for (std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr = combinedEnemiesArray_.begin();
			itr != combinedEnemiesArray_.end();
			itr++)
		{
			auto itrB = itr;

			itrB++;
			for (itrB; itrB != combinedEnemiesArray_.end(); itrB++)
			{
				//どっちもほかの敵と合体したら
				if (itr->get()->GetIsDockingAnyEnemy() && itrB->get()->GetIsDockingAnyEnemy())
				{
					itr->get()->AddCombinedEnemies(std::move(*itrB));
					combinedEnemiesArray_.erase(itrB);

					if (combinedEnemiesArray_.size() >= 2)
					{
						itr = combinedEnemiesArray_.begin();
					}
					else 
					{
						itr = combinedEnemiesArray_.begin();
						break;
					}
				}
			}
		}
	}

	//くっついてる敵の中で誰かほかの敵とくっついたら
	for (auto itr = combinedEnemiesArray_.begin(); itr != combinedEnemiesArray_.end(); itr++)
	{
		if (itr->get()->GetIsDockingAnyEnemy())
		{
			itr->get()->AllEnemiesDockingEnd();
			combEnemies = std::move(*itr);
			combinedEnemiesArray_.erase(itr);
			isMowDownedComb = true;
			break;
		}
	}

	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsDocking())
		{
			count++;
			//二体以上なら
			if (count >= 2 || (combEnemies && count >= 1))
			{
				isMowDowned = true;
				break;
			}
		}
	}


	if (isMowDowned)
	{
		//単体と単体が合体した場合は新たにインスタンス
		if (combEnemies == nullptr)
		{
			combEnemies = std::make_unique<CombinedEnemies>();
		}

		for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
		{
			if (itr->get()->GetIsDocking())
			{
				//合体したときの処理
				itr->get()->SetIsDocking(false);
				//今はtrue（仮
				if (true)
				{
					itr->get()->SetIsMowDown(false);
				}
				//くっついた敵の配列に追加する
				combEnemies->AddEnemy(std::move(*itr));
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

		//新しくインスタンスを作成した場合
		if (isMowDownedComb == false)
		{
			//初期化
			combEnemies->Initialize(player_, player_->GetDirectionVec());
		}
	}
	if (combEnemies)
	{
		//くっついた敵の塊を管理する配列に追加
		combinedEnemiesArray_.push_back(std::move(combEnemies));
	}
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

	//くっつく敵がいるかの更新処理
	MowDownUpdate();
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
