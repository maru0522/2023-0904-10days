#include "CombinedEnemiesState.h"
#include "CombinedEnemies.h"


float   ICombinedEnemiesState::radiusTmp_;
Vector2 ICombinedEnemiesState::centorPosTmp_;
Vector2 ICombinedEnemiesState::directionTmp_;


float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

std::unique_ptr<ICombinedEnemiesState> ICombinedEnemiesState::GetState(const std::string& name)
{
	std::unique_ptr<ICombinedEnemiesState>state = nullptr;

	if (name == "WAIT")
	{
		state = std::make_unique<CombinedEnemiesStateMoveWait>();
	}
	if (name == "SHRINK")
	{
		state = std::make_unique<CombinedEnemiesStateShrink>();
	}
	if (name == "WAIT_STRETCH")
	{
		state = std::make_unique<CombinedEnemiesStateWaitStretch>();
	}
	if (name == "STRETCH")
	{
		state = std::make_unique<CombinedEnemiesStateStretch>();
	}

	return state;
}

//-------------------------------------------------------------------------------------
//ã§í 
void ICombinedEnemiesState::Initialize()
{
	radiusTmp_ = enemies_->GetRadius();
	centorPosTmp_ = enemies_->GetCentorPos();
	directionTmp_ = enemies_->GetDireciton();
}

void ICombinedEnemiesState::TimerUpdate(std::function<void(float)> f)
{
	float t = timer_ / timerMax_;

	if (f)
	{
		f(t);
	}

	if (t >= 1.0f)
	{
		enemies_->ChangeState(nextStateName_);
	}

	timer_++;
}

//--------------------------------------------------------------------------------------
//ë“ã@íÜ
void CombinedEnemiesStateMoveWait::Initialize()
{
	nextStateName_ = "SHRINK";
}

void CombinedEnemiesStateMoveWait::Update()
{
	TimerUpdate();
}


//--------------------------------------------------------------------------------------
//èkÇ›íÜ
void CombinedEnemiesStateShrink::Initialize()
{
	nextStateName_ = "STRETCH_WAIT";
	//äÓÇ…Ç∑ÇÈÉpÉâÉÅÅ[É^Çï€ë∂
	ICombinedEnemiesState::Initialize();
}

void CombinedEnemiesStateShrink::Update()
{
	std::function<void(float)>f = [=](float t) {
		enemies_->SetRadius(lerp(radiusTmp_, 0, t));
	};

	TimerUpdate(f);
}



//--------------------------------------------------------------------------------------
//êLÇ—ë“ã@
void CombinedEnemiesStateWaitStretch::Initialize()
{
	nextStateName_ = "STRETCH";
}

void CombinedEnemiesStateWaitStretch::Update()
{
	std::function<void(float)>f = [=](float t) {
		enemies_->SetRadius(0);
	};

	TimerUpdate(f);
}


//--------------------------------------------------------------------------------------
//êLÇ—íÜ
void CombinedEnemiesStateStretch::Initialize()
{
	nextStateName_ = "WAIT";
}

void CombinedEnemiesStateStretch::Update()
{
	std::function<void(float)>f = [=](float t) {
		enemies_->SetRadius(lerp(0, radiusTmp_, t));
		//îºåaï™å¸Ç¢ÇƒÇÈï˚å¸Ç…êiÇﬁ
		enemies_->SetCentorPos({ lerp(centorPosTmp_.x, centorPosTmp_.x + directionTmp_.x * radiusTmp_, t),
			lerp(centorPosTmp_.y, centorPosTmp_.y + directionTmp_.y * radiusTmp_, t) });
	};

	TimerUpdate(f);
}
