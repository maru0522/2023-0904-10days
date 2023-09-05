#pragma once
#include"Enemy.h"
#include <memory>
#include <vector>


class ICombinedEnemiesState;

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
	//全体の半径
	float radius_ = 0.0f;
	//プレイヤーまでの距離
	float distance_ = 0;
	const int32_t MOVE_TIMES_ = Enemy::kMoveTimes_; // 何回に分けて移動するか(瞬間移動に見えるのを避けるため）
	const int32_t MOVE_INTERVAL_ = Enemy::kMoveInterval_; // 敵が動くまでの間隔(フレーム単位)
	//プレイヤーのポインター
	Player* player_ = nullptr;
	int32_t frameCountWait_; // 待機用フレームカウント
	int32_t frameCountMove_; // 移動用フレームカウント
	//ステート
	std::unique_ptr<ICombinedEnemiesState>state_ = nullptr;
	//
	float radiusTmp_;
	Vector2 centorPosTmp_;
	Vector2 directionTmp_;


public:
	~CombinedEnemies();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player_">playerポインタ</param>
	/// <param name="direciton">プレイヤーから見た敵の方向</param>
	/// <param name="enemies">くっつく敵の配列</param>
	void Initialize(Player* player_, const Vector2& direciton, std::vector<std::unique_ptr<Enemy>>enemies);
	void Update();
	void Draw();

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
	//向く方向の更新
	void DirectionUpdate();

private:
	void EnemiesPosUpdate();
	void CalcCentorPos(const Vector2& targetPos, const Vector2& direciton);

public:
	const Vector2& GetCentorPos() { return centorPos_; }
	void SetCentorPos(const Vector2& centorPos) { centorPos_ = centorPos; }
	const Vector2& GetDireciton() { return direction_; }
	void SetDireciton(const Vector2& direciton) { direction_ = direciton; }
	float GetRadius() { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }

	void SetRadiusTmp() { radiusTmp_ = radius_; }
	void SetCentorPosTmp() { centorPosTmp_ = centorPos_; }
	void SetDirectionTmp() { directionTmp_ = direction_; }

	float GetRadiusTmp() { return radiusTmp_; }
	const Vector2& GetCentorPosTmp() { return centorPosTmp_; }
	const Vector2& GetDirectionTmp() { return directionTmp_; }

public:
	void ChangeState(const std::string& name);
};

