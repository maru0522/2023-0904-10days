#pragma once
#include"Enemy.h"
#include <memory>
#include <vector>

//くっついた敵たち
class CombinedEnemies
{
private:
	//一緒にくっついてる敵の配列
	std::vector<std::unique_ptr<Enemy>>enemies_;
	//合体してる敵の中心座標
	Vector2 centorPos_ = { 0,0 };
	//敵全体の向く方向
	Vector2 direction_ = { 0,0 };
	//敵の数
	uint32_t enemiesNum_ = 0;
	//動くスピードの基
	const float MOVE_SPEED_TMP_ = Enemy::kMoveSpeed_;
	//全体の長さ
	float length_ = 1.0f;
	//

public:

};

