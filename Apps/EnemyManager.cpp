#include "EnemyManager.h"
#include "Keyboard.h"
#include "MathUtil.h"
#include <memory>


EnemyManager& EnemyManager::GetInstance()
{
	static EnemyManager sInst;
	return sInst;
}

//---------------------------------------------
void EnemyManager::Initialize(Player* player, Stage* stage)
{
	player_ = player;
	stage_ = stage;

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

void EnemyManager::GenerateUpdate()
{
	if (GetEnemiesCount() < ENEMIES_MAX_)
	{

		float x = Math::Function::Random<float>(100, 1100);
		float y = Math::Function::Random<float>(100, 600);
		std::unique_ptr<Enemy>enemy = std::make_unique<Enemy>(CollisionManger::GetInstance(), player_, stage_);
		enemy->SetPos({ x,y });
		enemy->SetRot(0);
		enemy->SetRad({ 10,0 });

		EnemyManager::GetInstance().AddEnemy(std::move(enemy));
	}
}


int32_t EnemyManager::GetEnemiesCount()
{
	int32_t count = 0;

	count += enemies_.size();

	count += combinedEnemiesArray_.size();

	return count;
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
			if (combinedEnemiesArray_.size() < 2)
			{
				break;
			}

			auto itrB = itr;
			itrB++;

			for (itrB; itrB != combinedEnemiesArray_.end(); itrB++)
			{
				//�����N���A�����
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
	//��������
	GenerateUpdate();
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
