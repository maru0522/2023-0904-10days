#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"
#include "MathUtil.h"



bool CombinedEnemies::GetIsDockingAnyEnemy()
{
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsDocking())
		{
			return true;
		}
	}

	return false;
}

bool CombinedEnemies::GetIsMowDownTriggerAnyEnemy()
{
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDownTrigger())
		{
			return true;
		}
	}

	return false;
}

bool CombinedEnemies::GetIsMowDownTriggerEnd()
{
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDownTrigger())
		{
			return false;
		}
	}

	return true;
}

bool CombinedEnemies::GetIsDockingAndSkewer()
{
	if (GetIsDockingAnyEnemy() && isSkewer_)
	{
		return true;
	}

	return false;
}

void CombinedEnemies::AllEnemiesDockingEnd()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsDocking(false);
	}
}

void CombinedEnemies::AllEnemiesEndMowDown()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsMowDown(false);
		enemy->SetIsMowDownTrigger(false);
	}
}

void CombinedEnemies::MowDownEnd()
{
	isMowDown_ = false;
	AllEnemiesEndMowDown();
}

void CombinedEnemies::MowDownTriggerEnd()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsMowDownTrigger(false);
	}
}

void CombinedEnemies::ChangeState(const std::string& name)
{
	std::string nameL = name;

	state_.reset();
	state_ = ICombinedEnemiesState::GetState(nameL);
	state_->SetCombinedEnemies(this);
	state_->Initialize();
}


CombinedEnemies::~CombinedEnemies()
{
	state_.reset();
}

//-------------------------------------------------
void CombinedEnemies::Initialize(Player* player, const Vector2& direction)
{
	player_ = player;

	//
	distance_ = (player_->GetPos() - player_->GetPos() + direction.Normalize() * length * ((float)enemiesNum_ / 2.0f)).Length();

	//中央位置計算
	CalcCentorPos(player_->GetPos(), direction.Normalize());

	//ステート
	ChangeState("AFTER_COMBINED");
}

void CombinedEnemies::CalcCentorPos(const Vector2& targetPos, const Vector2& direction, float length)
{
	//仮
	centorPos_ = targetPos + direction * length;
}

void CombinedEnemies::AnyEnemyMowDownUpdate()
{
	if (isMowDown_ || isSkewer_)
	{
		return;
	}

	for (auto& enemy : enemies_)
	{
		//一体でも薙ぎ払われてたら
		if (enemy->GetIsMowDown())
		{
			//薙ぎ払う処理
			MowDown();
			break;
		}
	}
}

void CombinedEnemies::MowDown()
{
	for (auto& enemy : enemies_)
	{
		//敵単体の薙ぎ払いフラグ
		enemy->SetIsMowDown(true);
		mowDownVec_ = enemy->GetMowDownVec();
	}
	//敵全体の薙ぎ払いフラグ
	isMowDown_ = true;
	//ステート変更
	ChangeState("MOW_DOWN");
}

void CombinedEnemies::AnyEnemySkewerUpdate()
{
	bool isSkewer = false;

	//どれか一つでも突進されてるか
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsSkewer())
		{
			isSkewer = true;
			break;
		}
	}

	if (!isSkewer) { return; }

	//突進される処理
	Skewer();
}

void CombinedEnemies::Skewer()
{
	//どれか一つでも突進されてたら全部フラグ立てる
	for (auto& enemy : enemies_)
	{
		enemy->SetIsSkewer(true);
	}

	isSkewer_ = true;

	ChangeState("SKEWER");
}

void CombinedEnemies::SkewerUpdate()
{
	DirectionUpdate();
	//串の下の部分が端の敵になるように
	float enemyRadius = length / (float)enemiesNum_ / 2.0f;
	centorPos_ = player_->GetPos4SwordBottom() + (player_->GetMoveVec() * (length / 2.0f + enemyRadius));

	if (!player_->GetIsSkewer() && isSkewer_)
	{
		Dead();
	}
}


void CombinedEnemies::EnemiesMowDownTriggerUpdate()
{
	for (auto& enemy : enemies_)
	{
		enemy->MowDownFlagUpdate();
	}
}

//-------------------------------------------------
void CombinedEnemies::Dead()
{
	for (auto& enemy : enemies_)
	{
		enemy->Dead();
	}

	isAlive_ = false;
}

//--------------------------------------------------
void CombinedEnemies::EnemiesPosUpdate()
{
	//敵が一体の場合は敵の座標を使う（押し戻しなどがあるので）
	if (enemiesNum_ == 1 && !GetIsMowDownTriggerAnyEnemy())
	{
		centorPos_ = enemies_[0]->GetPos();
		return;
	}

	//中央のインデックス
	float centorIndex = (float)enemiesNum_ / 2.0f - 0.5f;

	uint32_t count = 0;
	//対応した位置に配置
	for (auto& enemy : enemies_)
	{
		enemy->SetPos(centorPos_ + direction_ * (centorIndex - (float)count) * (length / (float)enemiesNum_));

		count++;
	}
}

void CombinedEnemies::EnemiesScaleReset()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetScale({ 1.0f,1.0f });
	}
}

void CombinedEnemies::EnemiesRotUpdate()
{
	float rot = atan2f(direction_.y, direction_.x);

	for (auto& enemy : enemies_)
	{
		enemy->SetRot(rot + Math::kPI / 2.0f);
	}
}

void CombinedEnemies::SetEnemiesScale(const Vector2& scale)
{
	for (auto& enemy : enemies_)
	{
		enemy->SetScale(scale);
	}
}

void CombinedEnemies::SetScaleSinRot(float minS, float maxS, float rate, int32_t timer)
{
	int32_t count = 0;
	for (auto& enemy : enemies_)
	{
		float rot = atan2f(direction_.y, direction_.x);

		Vector2 scale = { -sinf(rot) * sinf((float)timer * rate + (float)count),
			cosf(rot) * sinf((float)timer * rate + (float)count) };

		scale += {Math::Function::Random<float>((double)minS, (double)maxS),
			Math::Function::Random<float>((double)minS, (double)maxS)};

		scale.x = min(max(scale.x, minS), maxS);
		scale.y = min(max(scale.y, minS), maxS);

		enemy->SetScale(scale);
		count++;
	}
}

void CombinedEnemies::SetScale(const Vector2& scale)
{
	for (auto& enemy : enemies_)
	{
		Vector2 scaleL = scale;

		scaleL += {Math::Function::Random<float>((double)-scale.x * 0.1f, (double)scale.x * 0.1f),
			Math::Function::Random<float>((double)-scale.y * 0.1f, (double)scale.y * 0.1f)};

		enemy->SetScale(scaleL);
	}
}

//------------------------------------------------------------------------------
void CombinedEnemies::Update()
{
	//薙ぎ払いの更新
	//AnyEnemyMowDownUpdate();
	//突進の更新
	AnyEnemySkewerUpdate();

	//敵一体一体の座標更新
	EnemiesPosUpdate();

	state_->Update();
	//角度更新
	EnemiesRotUpdate();

	//薙ぎ払いフラグ更新
	EnemiesMowDownTriggerUpdate();

	//敵が一体の場合は押し戻しなどをするため
	if (enemiesNum_ == 1)
	{
		enemies_[0]->SetPos(centorPos_);
	}
}

void CombinedEnemies::Draw()
{
	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}
}

//------------------------------------------------------------
void CombinedEnemies::AddEnemy(std::unique_ptr<Enemy> enemy)
{
	enemy->SetIsDocking(false);
	enemy->SetIsMowDownTrigger(false);
	//敵の長さを加算していく
	float addRadius = Enemy::KRadius_ * 2.0f;
	length = radiusTmp_ + addRadius;
	radiusTmp_ = length;
	//登録
	enemies_.push_back(std::move(enemy));
	//敵の数を加算
	enemiesNum_++;
	//合体フラグオフ
	AllEnemiesDockingEnd();

	//どちらかが薙ぎ払われてたら
	/*else*/ if (GetIsMowDown())
	{
		MowDownEnd();
		ChangeState("AFTER_COMBINED");
	}
}

void CombinedEnemies::AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combinedEneies)
{
	//中間地点を中心座標にする
	centorPos_ = centorPos_ + ((combinedEneies->centorPos_ - centorPos_) / 2.0f);

	for (auto& enemy : combinedEneies->enemies_)
	{
		AddEnemy(std::move(enemy));
	}

	//どちらかが薙ぎ払われてたら
	if (combinedEneies->GetIsMowDown() || GetIsMowDown())
	{
		MowDownEnd();
	}

	ChangeState("AFTER_COMBINED");
}


//--------------------------------------------------------------
void CombinedEnemies::DirectionUpdate()
{
	//playerまでの距離を計算
	direction_ = (player_->GetPos() - centorPos_).Normalize();
}

