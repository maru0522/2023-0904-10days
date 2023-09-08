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

void EnemyManager::CombinedUpdate()
{
	//薙ぎ払われた敵がいるか
	bool isMowDowned = false;
	bool isMowDownedComb = false;
	//くっついてる敵で合体したものをまとめる配列
	std::unique_ptr<CombinedEnemies> combEnemies = nullptr;
	uint32_t count = 0;

	//くっついてる敵の中でほかのくっついてる敵と合体したか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator, std::vector<std::unique_ptr<CombinedEnemies>>::iterator)>ifF0
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrA, std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrB)
	{
		return itrA->get()->GetIsDockingAnyEnemy() && itrB->get()->GetIsDockingAnyEnemy();
	};
	CoalesceneceCombEnemiesEachOther(ifF0);

	//くっついてる敵が、ほかの敵と合体したか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr)
	{
		return itr->get()->GetIsDockingAnyEnemy();
	};
	CheckCombinedEnemiesCondition(combEnemies, isMowDownedComb, ifF);

	//敵と合体したか
	std::function<bool(std::unique_ptr<Enemy>&)>ifF2 = [=](std::unique_ptr<Enemy>& enemy)
	{
		return enemy->GetIsDocking();
	};
	std::function<bool(int32_t)>ifF3 = [=](int32_t count)
	{
		return count >= 2;
	};
	CheckEnemiesCondition(combEnemies, isMowDowned, ifF2, ifF3);

	//合体してる敵に敵を一体一体追加
	std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)>addIfF =
		[=](std::vector<std::unique_ptr<Enemy>>::iterator itr)
	{
		return itr->get()->GetIsDocking();
	};
	AddCombinedEnemies(std::move(combEnemies), isMowDowned, isMowDownedComb, addIfF);
}

//突進の時に合体させる
void EnemyManager::SkewerCombinedUpdate()
{
	//串刺しの敵と合体したか
	bool isSkewerDocking = false;
	bool isDockSkewCombined = false;
	//くっついてる敵で合体したものをまとめる配列
	std::unique_ptr<CombinedEnemies> skewerCombEnemies = nullptr;
	uint32_t count = 0;

	//くっついてる敵の中でほかのくっついてる敵と合体したか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator, std::vector<std::unique_ptr<CombinedEnemies>>::iterator)>ifF
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrA, std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrB)
	{
		return itrA->get()->GetIsSkewer() && itrB->get()->GetIsSkewer();
	};
	CoalesceneceCombEnemiesEachOther(ifF);

	//くっついてる敵が、ほかの突進されてる敵に当たったか
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF2 = [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr)
	{
		return itr->get()->GetIsDockingAndSkewer();
	};
	CheckCombinedEnemiesCondition(skewerCombEnemies, isDockSkewCombined, ifF2);

	//串ざされてる敵と合体したか
	std::function<bool(std::unique_ptr<Enemy>&)>ifF3 = [=](std::unique_ptr<Enemy>& enemy)
	{
		return /*enemy->GetIsDocking() && */enemy->GetIsSkewer();
	};
	std::function<bool(int32_t)>ifF4 = [=](int32_t count)
	{
		return count >= 1;
	};
	CheckEnemiesCondition(skewerCombEnemies, isSkewerDocking, ifF3, ifF4);

	//くっついてる敵に追加していく処理
	std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)>addIfF =
		[=](std::vector<std::unique_ptr<Enemy>>::iterator itr)
	{
		return itr->get()->GetIsSkewer();
	};
	AddCombinedEnemies(std::move(skewerCombEnemies), isSkewerDocking, isDockSkewCombined, addIfF);
}


//------------------------------------------------------------------------------------------------------------------
//共通処理
void EnemyManager::CoalesceneceCombEnemiesEachOther(std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator,
	std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF)
{
	//二つ以上ないとがったいできないので
	if (combinedEnemiesArray_.size() >= 2)
	{
		for (std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr = combinedEnemiesArray_.begin();
			itr != combinedEnemiesArray_.end();
			itr++)
		{
			if (combinedEnemiesArray_.size() < 2)
			{
				break;
			}

			auto itrB = itr;
			itrB++;

			for (itrB; itrB != combinedEnemiesArray_.end(); itrB++)
			{
				//条件クリアすれば
				if (ifF(itr, itrB) && itr != itrB)
				{
					itr->get()->AddCombinedEnemies(std::move(*itrB));
					combinedEnemiesArray_.erase(itrB);

					if (combinedEnemiesArray_.size() >= 2)
					{
						itrB = combinedEnemiesArray_.begin();
					}
					else
					{
						itrB = combinedEnemiesArray_.begin();
						break;
					}
				}
			}
		}
	}
}

void EnemyManager::CheckCombinedEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies, bool& settingFlag,
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF)
{
	for (auto itr = combinedEnemiesArray_.begin(); itr != combinedEnemiesArray_.end(); itr++)
	{
		if (ifF(itr))
		{
			//itr->get()->AllEnemiesDockingEnd();
			combEnemies = std::move(*itr);
			combinedEnemiesArray_.erase(itr);
			settingFlag = true;
			break;
		}
	}
}

void EnemyManager::CheckEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies, bool& settingFlag,
	std::function<bool(std::unique_ptr<Enemy>&)> ifF,
	std::function<bool(int32_t)>ifF2)
{
	int32_t count = 0;

	for (auto& enemy : enemies_)
	{
		if (ifF(enemy))
		{
			count++;
			//条件満たしたら
			if (ifF2(count) || (combEnemies && count >= 1))
			{
				settingFlag = true;
				break;
			}
		}
	}
}

void EnemyManager::AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combEnemies, bool isCombined, bool isDockCombined
	, std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)> addIfF)
{
	if (isCombined)
	{
		//単体と単体が合体した場合は新たにインスタンス
		if (combEnemies == nullptr)
		{
			combEnemies = std::make_unique<CombinedEnemies>();
		}

		for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
		{
			if (addIfF(itr))
			{
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
				if (enemies_.size() > 0)
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
		if (isDockCombined == false)
		{
			//初期化
			combEnemies->Initialize(player_, player_->GetDirectionVec());
		}
		combEnemies->AllEnemiesDockingEnd();
	}
	if (combEnemies)
	{
		//くっついた敵の塊を管理する配列に追加
		combinedEnemiesArray_.push_back(std::move(combEnemies));
	}
}


//----------------------------------------------------------------------------------
void EnemyManager::Update()
{
	for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
	{
		itr->get()->Update();

		if (!itr->get()->GetIsAlive())
		{
			enemies_.erase(itr);

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

	for (auto itr = combinedEnemiesArray_.begin(); itr != combinedEnemiesArray_.end(); itr++)
	{
		itr->get()->Update();

		if (!itr->get()->GetIsAlive())
		{
			combinedEnemiesArray_.erase(itr);

			if (combinedEnemiesArray_.size())
			{
				itr = combinedEnemiesArray_.begin();
			}
			else
			{
				break;
			}
		}
	}

	//くっつく敵がいるかの更新処理
	CombinedUpdate();
	//突進されてる敵とくっついたか
	SkewerCombinedUpdate();
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
