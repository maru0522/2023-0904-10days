#pragma once
#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"
#include "Stage.h"


//敵管理クラス
class EnemyManager final
{
private://変数
	//敵の配列
	std::vector<std::unique_ptr<Enemy>>enemies_;
	//くっついた敵同士をまとめた配列
	std::vector<std::unique_ptr<CombinedEnemies>> combinedEnemiesArray_;
	//プレイヤーのポインタ
	Player* player_ = nullptr;
	//ステージの
	Stage* stage_ = nullptr;
	//攻撃に当たった敵を保管
	std::unique_ptr<CombinedEnemies>mowDownedEnemies_ = nullptr;

	//敵の上限
	const uint32_t ENEMIES_MAX_ = 5;

	//倒された敵の数用
	bool isDefeatedEnemies_ = false;
	uint32_t defeatedEnemiesNum_ = 0;

private://関数
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
	//薙ぎ払われた敵をまとめて保存しておく
	void SaveMowDownEnemies();
	//くっつける更新処理
	void CombinedUpdate();
	//串刺しの敵をくっつける処理
	void SkewerCombinedUpdate();

	//生成の更新
	void GenerateUpdate();
	//薙ぎ払われてる最中の敵の塊の更新
	void MowDownTriggerEnemiesUpdate();

	//薙ぎ払われた瞬間を終わらせる
	void MowDownTriggerEnd();

    // でかだんご
    void GenerateDekaUpdate(void);

private:
	//敵の数を取得
	int32_t GetEnemiesCount();

private:
	void CoalesceneceCombEnemiesEachOther(std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator,
		std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF);

	/// <summary>
	/// 合体してる敵を全て調べて合体させるか決める
	/// </summary>
	/// <param name="combEnemies"></param>
	/// <param name="settingFlag"></param>
	/// <param name="ifF"></param>
	void CheckCombinedEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies, bool& settingFlag,
		std::function <bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator) > ifF);

	/// <summary>
	/// 敵一体一体を調べて合体させるか決める
	/// </summary>
	/// <param name="enemy"></param>
	/// <param name="combEnemies"></param>
	/// <param name="settingFlag"></param>
	void CheckEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies,
		bool& settingFlag, std::function<bool(std::unique_ptr<Enemy>&)>ifF,
		std::function<bool(int32_t)>ifF2);
	/// <summary>
	/// 敵同士をつっくける共通処理
	/// </summary>
	/// <param name="combEnemies">くっついてる敵の配列（新たに作られる場合はnullptr）</param>
	/// <param name="isCombined">敵一体一体を追加していくか</param>
	/// <param name="isDockCombined">すでにある合体した敵に追加していくか</param>
	/// <param name="isPushBackArray">配列に追加するか</param>
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combEnemies, bool isCombined, bool isDockCombined,
		std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)> addIfF, bool isPushBackArray = true);

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
	//倒した敵の数(倒された敵がいたらtrue)
	bool GetDefeatedEnemiesNum(uint32_t& enemiesNum);

public:
	//突進されてる敵の塊の半径
	float GetSkewerEnemiesLength();
};


