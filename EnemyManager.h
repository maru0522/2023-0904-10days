#pragma once
#include "CombinedEnemies.h"


//敵管理クラス
class EnemyManager final
{
private://変数
	//敵の配列
	std::vector<std::unique_ptr<Enemy>>enemies_;
	//くっついた敵同士をまとめた配列
	std::vector<std::vector<std::unique_ptr<Enemy>>>combinedEnemies_;

private://関数

public:

public:

public:

};


