#pragma once
#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"
#include "Stage.h"


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
	//�X�e�[�W��
	Stage* stage_ = nullptr;
	//�U���ɓ��������G��ۊ�
	std::unique_ptr<CombinedEnemies>mowDownedEnemies_ = nullptr;

	//�G�̏��
	const uint32_t ENEMIES_MAX_ = 5;

private://�֐�
	EnemyManager() { ; }
	~EnemyManager() { ; }

public:
	EnemyManager& operator=(const EnemyManager& inst) = delete;
	EnemyManager(const EnemyManager& inst) = delete;

	static EnemyManager& GetInstance();

public:
	void Initialize(Player* player, Stage* stage);
	void Update();
	void Draw();

private:
	//�ガ����ꂽ�G���܂Ƃ߂ĕۑ����Ă���
	void SaveMowDownEnemies();
	//��������X�V����
	void CombinedUpdate();
	//���h���̓G���������鏈��
	void SkewerCombinedUpdate();

	//�����̍X�V
	void GenerateUpdate();
	//�ガ�����Ă�Œ��̓G�̉�̍X�V
	void MowDownTriggerEnemiesUpdate();

	//�ガ����ꂽ�u�Ԃ��I��点��
	void MowDownTriggerEnd();

private:
	//�G�̐����擾
	int32_t GetEnemiesCount();

private:
	void CoalesceneceCombEnemiesEachOther(std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator,
		std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF);

	/// <summary>
	/// ���̂��Ă�G��S�Ē��ׂč��̂����邩���߂�
	/// </summary>
	/// <param name="combEnemies"></param>
	/// <param name="settingFlag"></param>
	/// <param name="ifF"></param>
	void CheckCombinedEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies, bool& settingFlag,
		std::function <bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator) > ifF);

	/// <summary>
	/// �G��̈�̂𒲂ׂč��̂����邩���߂�
	/// </summary>
	/// <param name="enemy"></param>
	/// <param name="combEnemies"></param>
	/// <param name="settingFlag"></param>
	void CheckEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies,
		bool& settingFlag, std::function<bool(std::unique_ptr<Enemy>&)>ifF,
		std::function<bool(int32_t)>ifF2);
	/// <summary>
	/// �G���m���������鋤�ʏ���
	/// </summary>
	/// <param name="combEnemies">�������Ă�G�̔z��i�V���ɍ����ꍇ��nullptr�j</param>
	/// <param name="isCombined">�G��̈�̂�ǉ����Ă�����</param>
	/// <param name="isDockCombined">���łɂ��鍇�̂����G�ɒǉ����Ă�����</param>
	/// <param name="isPushBackArray">�z��ɒǉ����邩</param>
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combEnemies, bool isCombined, bool isDockCombined,
		std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)> addIfF, bool isPushBackArray = true);

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);

public:
	//�ːi����Ă�G�̉�̔��a
	float GetSkewerEnemiesRadius();
};


