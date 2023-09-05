#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"
#include "MathUtil.h"



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
	distance_ = (player_->GetPos() - player_->GetPos() + direciton.Normalize() * radius_ * ((float)enemiesNum_ / 2.0f)).Length();

	//’†‰›ˆÊ’uŒvŽZ
	CalcCentorPos(player_->GetPos(), direciton.Normalize());

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
	//‘Î‰ž‚µ‚½ˆÊ’u‚É”z’u
	for (auto& enemy : enemies_)
	{
		enemy->SetPos(centorPos_ + direction_ * (centorIndex - (float)count) * (radius_ / (float)enemiesNum_));

		count++;
	}
}

void CombinedEnemies::Update()
{
	state_->Update();
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
	//“G‚Ì’·‚³‚ð‰ÁŽZ‚µ‚Ä‚¢‚­
	radius_ += enemy->GetRad().Length() * 2.0f;
	//“o˜^
	enemies_.push_back(std::move(enemy));
	//“G‚Ì”‚ð‰ÁŽZ
	enemiesNum_++;
}

void CombinedEnemies::DirectionUpdate()
{
	//player‚Ü‚Å‚Ì‹——£‚ðŒvŽZ
	direction_ = (player_->GetPos() - centorPos_).Normalize();
}

