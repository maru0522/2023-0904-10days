#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"
#include "MathUtil.h"



void CombinedEnemies::ChangeState(const std::string& name)
{
	state_.reset();
	state_ = ICombinedEnemiesState::GetState(name);
	state_->SetCombinedEnemies(this);
	state_->Initialize();
}


//-------------------------------------------------
void CombinedEnemies::Initialize(Player* player, const Vector2& direciton, std::vector<std::unique_ptr<Enemy>>enemies)
{
	enemies_.clear();
	player_ = player;

	//登録
	for (auto& enemy : enemies)
	{
		AddEnemy(std::move(enemy));
	}

	//
	distance_ = (player_->GetPos() - player_->GetPos() + direciton * radius_).Length();

	//中央位置計算
	CalcCentorPos(player_->GetPos(), direciton);

	//ステート
	ChangeState("WAIT");
}

void CombinedEnemies::CalcCentorPos(const Vector2& targetPos, const Vector2& direciton)
{
	centorPos_ = targetPos + direciton * distance_;
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
		enemy->SetPos(centorPos_ + direction_ * (centorIndex - (float)count));

		count++;
	}
}

void CombinedEnemies::Update()
{
	state_->Update();

	//playerまでの距離を計算
	direction_ = player_->GetPos() - centorPos_;

	//
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
	radius_ += enemy->GetRad().Length();
	//登録
	enemies_.push_back(std::move(enemy));
	//敵の数を加算
	enemiesNum_++;
}

