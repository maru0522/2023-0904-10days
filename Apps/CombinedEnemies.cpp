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

void CombinedEnemies::AllEnemiesDockingEnd()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsDocking(false);
	}

	ChangeState("WAIT");
}

void CombinedEnemies::AllEnemiesEndMowDown()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsMowDown(false);
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
void CombinedEnemies::Initialize(Player* player, const Vector2& direciton)
{
	player_ = player;

	//
	distance_ = (player_->GetPos() - player_->GetPos() + direciton.Normalize() * radius_ * ((float)enemiesNum_ / 2.0f)).Length();

	//中央位置計算
	CalcCentorPos(player_->GetPos(), direciton.Normalize());

	//ステート
	ChangeState("WAIT");
}

void CombinedEnemies::CalcCentorPos(const Vector2& targetPos, const Vector2& direciton)
{
	//仮
	centorPos_ = enemies_[0]->GetPos();
}

void CombinedEnemies::AnyEnemyMowDownUpdate()
{
	if (isMowDown_)
	{
		return;
	}

	for (auto& enemy : enemies_)
	{
		//一体でも薙ぎ払われてたら
		if (enemy->GetIsMowDown())
		{
			mowDownVec_ = enemy->GetMowDownVec();
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
	}
	//敵全体の薙ぎ払いフラグ
	isMowDown_ = true;
	//ステート変更
	ChangeState("MOW_DOWN");
}

//--------------------------------------------------
void CombinedEnemies::EnemiesPosUpdate()
{
	//中央のインデックス
	float centorIndex = (float)enemiesNum_ / 2.0f - 0.5f;

	uint32_t count = 0;
	//対応した位置に配置
	for (auto& enemy : enemies_)
	{
		enemy->SetPos(centorPos_ + direction_ * (centorIndex - (float)count) * (radius_ / (float)enemiesNum_));

		count++;
	}
}

void CombinedEnemies::Update()
{
	//薙ぎ払いの更新
	AnyEnemyMowDownUpdate();

	state_->Update();
	//敵一体一体の座標更新
	EnemiesPosUpdate();
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
	//敵の長さを加算していく
	float addRadius = enemy->GetRad().Length() * 2.0f * (1.0f / (1.0f - Enemy::kPngScale_));
	radius_ = radiusTmp_ + addRadius;
	radiusTmp_ = radius_;
	//登録
	enemies_.push_back(std::move(enemy));
	//敵の数を加算
	enemiesNum_++;
}

void CombinedEnemies::AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combinedEneies)
{
	//中間地点を中心座標にする
	centorPos_ = centorPos_ + ((combinedEneies->centorPos_ - centorPos_) / 2.0f);

	for (auto& enemy : combinedEneies->enemies_)
	{
		AddEnemy(std::move(enemy));
	}

	SetIsMowDown(false);
	ChangeState("WAIT");
}


//--------------------------------------------------------------
void CombinedEnemies::DirectionUpdate()
{
	//playerまでの距離を計算
	direction_ = (player_->GetPos() - centorPos_).Normalize();
}

