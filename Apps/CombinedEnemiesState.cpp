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
//共通
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
//待機中
void CombinedEnemiesStateMoveWait::Initialize()
{
	nextStateName_ = "SHRINK";
}

void CombinedEnemiesStateMoveWait::Update()
{
	TimerUpdate();
}


//--------------------------------------------------------------------------------------
//縮み中
void CombinedEnemiesStateShrink::Initialize()
{
	nextStateName_ = "STRETCH_WAIT";
	//基にするパラメータを保存
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
//伸び待機
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
//伸び中
void CombinedEnemiesStateStretch::Initialize()
{
	nextStateName_ = "WAIT";
}

void CombinedEnemiesStateStretch::Update()
{
	std::function<void(float)>f = [=](float t) {
		enemies_->SetRadius(lerp(0, radiusTmp_, t));
		//半径分向いてる方向に進む
		enemies_->SetCentorPos({ lerp(centorPosTmp_.x, centorPosTmp_.x + directionTmp_.x * radiusTmp_, t),
			lerp(centorPosTmp_.y, centorPosTmp_.y + directionTmp_.y * radiusTmp_, t) });
	};

	TimerUpdate(f);
}
