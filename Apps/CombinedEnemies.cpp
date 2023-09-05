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

	//“o˜^
	for (auto& enemy : enemies)
	{
		AddEnemy(std::move(enemy));
	}

	//
	distance_ = (player_->GetPos() - player_->GetPos() + direciton * radius_).Length();

	//’†‰›ˆÊ’uŒvZ
	CalcCentorPos(player_->GetPos(), direciton);

	//ƒXƒe[ƒg
	ChangeState("WAIT");
}

void CombinedEnemies::CalcCentorPos(const Vector2& targetPos, const Vector2& direciton)
{
	centorPos_ = targetPos + direciton * distance_;
}

//--------------------------------------------------
void CombinedEnemies::EnemiesPosUpdate()
{
	//’†‰›‚ÌƒCƒ“ƒfƒbƒNƒX
	float centorIndex = (float)enemiesNum_ / 2.0f - 0.5f;

	uint32_t count = 0;
	//‘Î‰‚µ‚½ˆÊ’u‚É”z’u
	for (auto& enemy : enemies_)
	{
		enemy->SetPos(centorPos_ + direction_ * (centorIndex - (float)count));

		count++;
	}
}

void CombinedEnemies::Update()
{
	state_->Update();

	//player‚Ü‚Å‚Ì‹——£‚ğŒvZ
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
	//“G‚Ì’·‚³‚ğ‰ÁZ‚µ‚Ä‚¢‚­
	radius_ += enemy->GetRad().Length();
	//“o˜^
	enemies_.push_back(std::move(enemy));
	//“G‚Ì”‚ğ‰ÁZ
	enemiesNum_++;
}

