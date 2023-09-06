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
	bool isMowDownedComb = false;
	//�������Ă�G�ō��̂������̂��܂Ƃ߂�z��
	std::unique_ptr<CombinedEnemies> combEnemies = nullptr;
	uint32_t count = 0;

	//�������Ă�G�̒��łق��̂������Ă�G�ƍ��̂�����
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
				//�ǂ������ق��̓G�ƍ��̂�����
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

	//�������Ă�G�̒��ŒN���ق��̓G�Ƃ���������
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
			//��̈ȏ�Ȃ�
			if (count >= 2 || (combEnemies && count >= 1))
			{
				isMowDowned = true;
				break;
			}
		}
	}


	if (isMowDowned)
	{
		//�P�̂ƒP�̂����̂����ꍇ�͐V���ɃC���X�^���X
		if (combEnemies == nullptr)
		{
			combEnemies = std::make_unique<CombinedEnemies>();
		}

		for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
		{
			if (itr->get()->GetIsDocking())
			{
				//���̂����Ƃ��̏���
				itr->get()->SetIsDocking(false);
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

		//�V�����C���X�^���X���쐬�����ꍇ
		if (isMowDownedComb == false)
		{
			//������
			combEnemies->Initialize(player_, player_->GetDirectionVec());
		}
	}
	if (combEnemies)
	{
		//���������G�̉���Ǘ�����z��ɒǉ�
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

	//�������G�����邩�̍X�V����
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
