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
	//�ガ����ꂽ�G�����邩
	bool isMowDowned = false;
	bool isMowDownedComb = false;
	//�������Ă�G�ō��̂������̂��܂Ƃ߂�z��
	std::unique_ptr<CombinedEnemies> combEnemies = nullptr;
	uint32_t count = 0;

	//�������Ă�G�̒��łق��̂������Ă�G�ƍ��̂�����
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator, std::vector<std::unique_ptr<CombinedEnemies>>::iterator)>ifF0
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrA, std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrB)
	{
		return itrA->get()->GetIsDockingAnyEnemy() && itrB->get()->GetIsDockingAnyEnemy();
	};
	CoalesceneceCombEnemiesEachOther(ifF0);

	//�������Ă�G���A�ق��̓G�ƍ��̂�����
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr)
	{
		return itr->get()->GetIsDockingAnyEnemy();
	};
	CheckCombinedEnemiesCondition(combEnemies, isMowDownedComb, ifF);

	//�G�ƍ��̂�����
	std::function<bool(std::unique_ptr<Enemy>&)>ifF2 = [=](std::unique_ptr<Enemy>& enemy)
	{
		return enemy->GetIsDocking();
	};
	std::function<bool(int32_t)>ifF3 = [=](int32_t count)
	{
		return count >= 2;
	};
	CheckEnemiesCondition(combEnemies, isMowDowned, ifF2, ifF3);

	//���̂��Ă�G�ɓG����̈�̒ǉ�
	std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)>addIfF =
		[=](std::vector<std::unique_ptr<Enemy>>::iterator itr)
	{
		return itr->get()->GetIsDocking();
	};
	AddCombinedEnemies(std::move(combEnemies), isMowDowned, isMowDownedComb, addIfF);
}

//�ːi�̎��ɍ��̂�����
void EnemyManager::SkewerCombinedUpdate()
{
	//���h���̓G�ƍ��̂�����
	bool isSkewerDocking = false;
	bool isDockSkewCombined = false;
	//�������Ă�G�ō��̂������̂��܂Ƃ߂�z��
	std::unique_ptr<CombinedEnemies> skewerCombEnemies = nullptr;
	uint32_t count = 0;

	//�������Ă�G�̒��łق��̂������Ă�G�ƍ��̂�����
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator, std::vector<std::unique_ptr<CombinedEnemies>>::iterator)>ifF
		= [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrA, std::vector<std::unique_ptr<CombinedEnemies>>::iterator itrB)
	{
		return itrA->get()->GetIsSkewer() && itrB->get()->GetIsSkewer();
	};
	CoalesceneceCombEnemiesEachOther(ifF);

	//�������Ă�G���A�ق��̓ːi����Ă�G�ɓ���������
	std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF2 = [=](std::vector<std::unique_ptr<CombinedEnemies>>::iterator itr)
	{
		return itr->get()->GetIsDockingAndSkewer();
	};
	CheckCombinedEnemiesCondition(skewerCombEnemies, isDockSkewCombined, ifF2);

	//��������Ă�G�ƍ��̂�����
	std::function<bool(std::unique_ptr<Enemy>&)>ifF3 = [=](std::unique_ptr<Enemy>& enemy)
	{
		return /*enemy->GetIsDocking() && */enemy->GetIsSkewer();
	};
	std::function<bool(int32_t)>ifF4 = [=](int32_t count)
	{
		return count >= 1;
	};
	CheckEnemiesCondition(skewerCombEnemies, isSkewerDocking, ifF3, ifF4);

	//�������Ă�G�ɒǉ����Ă�������
	std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)>addIfF =
		[=](std::vector<std::unique_ptr<Enemy>>::iterator itr)
	{
		return itr->get()->GetIsSkewer();
	};
	AddCombinedEnemies(std::move(skewerCombEnemies), isSkewerDocking, isDockSkewCombined, addIfF);
}


//------------------------------------------------------------------------------------------------------------------
//���ʏ���
void EnemyManager::CoalesceneceCombEnemiesEachOther(std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator,
	std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF)
{
	//��ȏ�Ȃ��Ƃ��������ł��Ȃ��̂�
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
				//�����N���A�����
				if (ifF(itr, itrB))
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
			//��������������
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
		//�P�̂ƒP�̂����̂����ꍇ�͐V���ɃC���X�^���X
		if (combEnemies == nullptr)
		{
			combEnemies = std::make_unique<CombinedEnemies>();
		}

		for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
		{
			if (addIfF(itr))
			{
				//����true�i��
				if (true)
				{
					itr->get()->SetIsMowDown(false);
				}
				//���������G�̔z��ɒǉ�����
				combEnemies->AddEnemy(std::move(*itr));
				//���̔z�񂩂�폜
				enemies_.erase(itr);

				//�G���[���
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

		//�V�����C���X�^���X���쐬�����ꍇ
		if (isDockCombined == false)
		{
			//������
			combEnemies->Initialize(player_, player_->GetDirectionVec());
		}
		combEnemies->AllEnemiesDockingEnd();
	}
	if (combEnemies)
	{
		//���������G�̉���Ǘ�����z��ɒǉ�
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

	//�������G�����邩�̍X�V����
	CombinedUpdate();
	//�ːi����Ă�G�Ƃ���������
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
