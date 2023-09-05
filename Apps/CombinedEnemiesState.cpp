#include "CombinedEnemiesState.h"
#include "CombinedEnemies.h"


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

	return std::move(state);
}

//-------------------------------------------------------------------------------------
//共通
void ICombinedEnemiesState::Initialize()
{
	enemies_->SetCentorPosTmp();
	enemies_->SetDirectionTmp();
	enemies_->SetRadiusTmp();
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
	//向き更新
	enemies_->DirectionUpdate();

	TimerUpdate();
}


//--------------------------------------------------------------------------------------
//縮み中
void CombinedEnemiesStateShrink::Initialize()
{
	nextStateName_ = "WAIT_STRETCH";
	timerMax_ = 10;
	//基にするパラメータを保存
	ICombinedEnemiesState::Initialize();
}

void CombinedEnemiesStateShrink::Update()
{
	std::function<void(float)>f = [=](float t) {
		enemies_->SetRadius(lerp(enemies_->GetRadiusTmp(), 0, t));
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
	timerMax_ = 10;
}

void CombinedEnemiesStateStretch::Update()
{
	Vector2 centorPT = enemies_->GetCentorPosTmp();
	Vector2 directionT = enemies_->GetDirectionTmp();
	float radiusT = enemies_->GetRadiusTmp();

	std::function<void(float)>f = [=](float t) {
		enemies_->SetRadius(lerp(0, enemies_->GetRadiusTmp(), t));
		//半径分向いてる方向に進む
		enemies_->SetCentorPos({ lerp(centorPT.x, centorPT.x + directionT.x * radiusT * 2.0f, t),
			lerp(centorPT.y, centorPT.y + directionT.y * radiusT * 2.0f, t) });
	};

	TimerUpdate(f);
}
