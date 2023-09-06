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
	uint32_t count = 0;

	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDown())
		{
			count++;
			//��̈ȏ�Ȃ�
			if (count >= 2)
			{
				isMowDowned = true;
			}
		}
	}

	if (isMowDowned)
	{
		//�C���X�^���X����
		std::unique_ptr<CombinedEnemies>combinedEnemies = std::make_unique<CombinedEnemies>();
		//�擾�p�̔z��
		std::vector<std::unique_ptr<Enemy>>enemiesL;

		for (auto itr = enemies_.begin(); itr != enemies_.end(); itr++)
		{
			if (itr->get()->GetIsMowDown())
			{
				//�ǉ������̂ŃI�t
				itr->get()->SetIsMowDown(false);
				//���������G�̔z��ɒǉ�����
				enemiesL.push_back(std::move(*itr));
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
		//������
		combinedEnemies->Initialize(player_, player_->GetDirectionVec(), std::move(enemiesL));

		//���������G�̉���Ǘ�����z��ɒǉ�
		combinedEnemiesArray_.push_back(std::move(combinedEnemies));
	}
}

void EnemyManager::Update()
{
	//�������G�����邩�̍X�V����
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
