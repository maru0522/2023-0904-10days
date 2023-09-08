#include "CombinedEnemiesState.h"
#include "CombinedEnemies.h"
#include "Util.h"
#include "MathUtil.h"


float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

std::unique_ptr<ICombinedEnemiesState> ICombinedEnemiesState::GetState(const std::string& name)
{
	std::unique_ptr<ICombinedEnemiesState>state = nullptr;

	if (name == "MOW_DOWN")
	{
		state = std::make_unique<CombinedEnemiesStateMoveMowDown>();
	}
	if (name == "SKEWER")
	{
		state = std::make_unique<CombinedEnemiesStateSkewer>();
	}
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

void ICombinedEnemiesState::TimerUpdate(const std::function<void(float)>& f, const std::function<void()>& endF)
{
	float t = min(timer_ / timerMax_, 1.0f);

	if (f)
	{
		f(t);
	}

	if (t >= 1.0f)
	{
		if (endF)
		{
			endF();
		}

		enemies_->ChangeState(nextStateName_);
		return;
	}

	timer_++;
}

//-------------------------------------------------------------------------------------
//薙ぎ払われ中
void CombinedEnemiesStateMoveMowDown::Initialize()
{
	nextStateName_ = "WAIT";
	timerMax_ = Enemy::kMowFrame_;

	direction_ = enemies_->GetMowDownVec();
	//途中でステート変わったとき用
	enemies_->SetRadius(enemies_->GetRadiusTmp());
	enemies_->SetCentorPosTmp();
}

void CombinedEnemiesStateMoveMowDown::Update()
{
	Vector2 centorPT = enemies_->GetCentorPosTmp();

	std::function<void(float)>f = [=](float t) {
		enemies_->SetCentorPos({ lerp(centorPT.x, centorPT.x + direction_.x * Player::kMowDist_ * timerMax_,Math::Ease::EaseOutCirc(t,0,1.0f)),
			lerp(centorPT.y, centorPT.y + direction_.y * Player::kMowDist_ * timerMax_, Math::Ease::EaseOutCirc(t,0,1.0f)) });
	};

	std::function<void()>endF = [=]() {
		enemies_->MowDownEnd();
	};

	TimerUpdate(f, endF);
}


//--------------------------------------------------------------------------------------
void CombinedEnemiesStateSkewer::Initialize()
{
	//途中でステート変わったとき用
	enemies_->SetRadius(enemies_->GetRadiusTmp());
	enemies_->SetCentorPosTmp();
	enemies_->SetIsSkewer(true);
}

void CombinedEnemiesStateSkewer::Update()
{
	enemies_->SkewerUpdate();
}


//--------------------------------------------------------------------------------------
//待機中
void CombinedEnemiesStateMoveWait::Initialize()
{
	nextStateName_ = "SHRINK";
	enemies_->SetIsMowDown(false);
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
		enemies_->SetRadius(lerp(enemies_->GetRadiusTmp(), 0, Math::Ease::EaseInCubic(t, 0, 1.0f)));
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
		enemies_->SetRadius(lerp(0, enemies_->GetRadiusTmp(), Math::Ease::EaseInCubic(t, 0, 1.0f)));
		//半径分向いてる方向に進む
		Vector2 nextPos = { lerp(centorPT.x, centorPT.x + directionT.x * radiusT * 2.0f, t),
			lerp(centorPT.y, centorPT.y + directionT.y * radiusT * 2.0f, t) };

		//すり抜けたら
		Vector2 directionT2 = (enemies_->GetTargetPos() - enemies_->GetCentorPos()).Normalize();
		if ((enemies_->GetTargetPos() - nextPos).Normalize().Dot(directionT2) <= 0.0f)
		{
			nextPos = enemies_->GetTargetPos() + (-directionT2 * enemies_->GetRadiusTmp() / 2.0f);
		}

		enemies_->SetCentorPos(nextPos);
	};

	TimerUpdate(f);
}
