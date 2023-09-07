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
	//薙ぎ払い方向
	Vector2 mowDownVec_ = {0,0};
	//薙ぎ払われてるか
	bool isMowDown_ = false;
	//串刺しにされているか
	bool isSkewer_ = false;
	//生きているか
	bool isAlive_ = true;


public:
	~CombinedEnemies();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player_">playerポインタ</param>
	/// <param name="direciton">プレイヤーから見た敵の方向</param>
	/// <param name="enemies">くっつく敵の配列</param>
	void Initialize(Player* player_, const Vector2& direciton);
	void Update();
	void Draw();

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies>combinedEneies);
	//向く方向の更新
	void DirectionUpdate();

private:
	void EnemiesPosUpdate();
	void CalcCentorPos(const Vector2& targetPos, const Vector2& direciton);
	//くっついてる敵のどれか一つでも薙ぎ払われたら
	void AnyEnemyMowDownUpdate();
	//薙ぎ払う
	void MowDown();
	//くっついてる敵のどれか一つでも突進されたら
	void AnyEnemySkewerUpdate();
	//突進される処理
	void Skewer();

public:
	const Vector2& GetCentorPos() { return centorPos_; }
	void SetCentorPos(const Vector2& centorPos) { centorPos_ = centorPos; }
	const Vector2& GetDireciton() { return direction_; }
	void SetDireciton(const Vector2& direciton) { direction_ = direciton; }
	float GetRadius() { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }

	void SetRadiusTmp() { 
		radiusTmp_ = radius_; 
	}
	void SetCentorPosTmp() { centorPosTmp_ = centorPos_; }
	void SetDirectionTmp() { directionTmp_ = direction_; }
	void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }
	void SetIsSkewer(bool isSkewer) { isSkewer_ = isSkewer; }

	float GetRadiusTmp() { return radiusTmp_; }
	const Vector2& GetCentorPosTmp() { return centorPosTmp_; }
	const Vector2& GetDirectionTmp() { return directionTmp_; }

	int32_t GetEnemiesNum() { return enemiesNum_; }

	const Vector2& GetMowDownVec() { return mowDownVec_; }
	bool GetIsMowDown() { return isMowDown_; }
	bool GetIsSkewer() { return isSkewer_; }
	bool GetIsAlive() { return isAlive_; }

	const Vector2& GetTargetPos() { player_->GetPos(); }

	//配列の参照
	std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }

public:
	//敵全体の中でほかの敵と合体したものがいるか
	bool GetIsDockingAnyEnemy();
	//敵全体の中でほかの串刺しにされてる敵と合体したものがいるか
	bool GetIsDockingAndSkewer();
	//ほかの敵と合体したときにisDockingをfalse
	void AllEnemiesDockingEnd();
	//移動し終わり、全部の敵の薙ぎ払いフラグをオフ
	void AllEnemiesEndMowDown();
	//薙ぎ払い終わり
	void MowDownEnd();
	//突進されてるときの更新
	void SkewerUpdate();
	//死亡処理
	void Dead();

public:
	void ChangeState(const std::string& name);
};

