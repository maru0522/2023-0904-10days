#pragma once
#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"


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

private://関数
	EnemyManager() { ; }
	~EnemyManager() { ; }

public:
	EnemyManager& operator=(const EnemyManager& inst) = delete;
	EnemyManager(const EnemyManager& inst) = delete;

	static EnemyManager& GetInstance();

public:
	void Initialize(Player* player);
	void Update();
	void Draw();

private:
	void MowDownUpdate();

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
};


