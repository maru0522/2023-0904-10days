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
	//全体の長さ
	float length = 0.0f;
	//プレイヤーまでの距離
	float distance_ = 0;
	const int32_t MOVE_TIMES_ = Enemy::kMoveTimes_; // 何回に分けて移動するか(瞬間移動に見えるのを避けるため）
	const int32_t MOVE_INTERVAL_ = Enemy::kMoveInterval_; // 敵が動くまでの間隔(フレーム単位)
	//プレイヤーのポインター
	Player* player_ = nullptr;
	//ステージのポインタ
	Stage* stage_ = nullptr;
	int32_t frameCountWait_; // 待機用フレームカウント
	int32_t frameCountMove_; // 移動用フレームカウント
	//ステート
	std::unique_ptr<ICombinedEnemiesState>state_ = nullptr;
	//
	float radiusTmp_;
	Vector2 centorPosTmp_;
	Vector2 directionTmp_;
	//薙ぎ払い方向
	Vector2 mowDownVec_ = { 0,0 };
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
	/// <param name="direction">プレイヤーから見た敵の方向</param>
	/// <param name="enemies">くっつく敵の配列</param>
	void Initialize(Player* player, Stage* stage, const Vector2& direction);
	void Update();
	void Draw();

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies>combinedEneies);
	//向く方向の更新
	void DirectionUpdate();
	//敵一体一体の位置更新
	void EnemiesPosUpdate();
	//スケールを元に戻す
	void EnemiesScaleReset();
	//薙ぎ払う
	void MowDown();
	//
	void CalcCentorPos(const Vector2& targetPos, const Vector2& direction, float length = 1.0f);

private:
	//敵をステージ外に出さないように
	bool SetInStagePos(const Vector2& pos, Vector2& pushBackVec);
	//
	bool SetInStageEnemiesPos();

private:
	//角度の更新
	void EnemiesRotUpdate();
	//スケールをセット
	void SetEnemiesScale(const Vector2& scale);
	//くっついてる敵のどれか一つでも薙ぎ払われたら
	void AnyEnemyMowDownUpdate();
	//くっついてる敵のどれか一つでも突進されたら
	void AnyEnemySkewerUpdate();
	//突進される処理
	void Skewer();
	//薙ぎ払われた瞬間フラグの更新
	void EnemiesMowDownTriggerUpdate();

public:
	const Vector2& GetCentorPos() { return centorPos_; }
	void SetCentorPos(const Vector2& centorPos) { centorPos_ = centorPos; }
	const Vector2& GetDirection() { return direction_; }
	void SetDirection(const Vector2& direction) { direction_ = direction.Normalize(); }
	float GetLength() { return length; }
	void SetLength(float radius) {
		length = radius;
	}

	void SetLengthTmp() {
		radiusTmp_ = length;
	}
	void SetCentorPosTmp() { centorPosTmp_ = centorPos_; }
	void SetDirectionTmp() { directionTmp_ = direction_; }
	void SetIsMowDown(bool isMowDown) { isMowDown_ = isMowDown; }
	void SetIsSkewer(bool isSkewer) { isSkewer_ = isSkewer; }
	void SetIsSkewer(const Vector2& mowDownVec) { mowDownVec_ = mowDownVec; }

	float GetLengthTmp() { return radiusTmp_; }
	const Vector2& GetCentorPosTmp() { return centorPosTmp_; }
	const Vector2& GetDirectionTmp() { return directionTmp_; }

	int32_t GetEnemiesNum() { return enemiesNum_; }

	const Vector2& GetMowDownVec() { return mowDownVec_; }
	bool GetIsMowDown() { return isMowDown_; }
	bool GetIsSkewer() { return isSkewer_; }
	bool GetIsAlive() { return isAlive_; }

	const Vector2& GetTargetPos() { return player_->GetPos(); }

	//配列の参照
	std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }

public:
	//敵全体の中でほかの敵と合体したものがいるか
	bool GetIsDockingAnyEnemy();
	//薙ぎ払いされた瞬間がいるか
	bool GetIsMowDownTriggerAnyEnemy();
	//全ての敵が薙ぎ払われた瞬間ではないかどうか
	bool GetIsMowDownTriggerEnd();
	//敵全体の中でほかの串刺しにされてる敵と合体したものがいるか
	bool GetIsDockingAndSkewer();
	//ほかの敵と合体したときにisDockingをfalse
	void AllEnemiesDockingEnd();
	//移動し終わり、全部の敵の薙ぎ払いフラグをオフ
	void AllEnemiesEndMowDown();
	//薙ぎ払い終わり
	void MowDownEnd();
	//薙ぎ払われた瞬間終わり
	void MowDownTriggerEnd();
	//突進されてるときの更新
	void SkewerUpdate();
	//死亡処理
	void Dead();
	//スケール揺らす
	void SetScaleSinRot(float minS, float maxS, float rate, int32_t timer);
	//スケールセット
	void SetScale(const Vector2& scale);

public:
	void ChangeState(const std::string& name);
};

