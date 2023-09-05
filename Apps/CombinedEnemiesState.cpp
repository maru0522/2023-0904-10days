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
//����
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
//�ҋ@��
void CombinedEnemiesStateMoveWait::Initialize()
{
	nextStateName_ = "SHRINK";
}

void CombinedEnemiesStateMoveWait::Update()
{
	//�����X�V
	enemies_->DirectionUpdate();

	TimerUpdate();
}


//--------------------------------------------------------------------------------------
//�k�ݒ�
void CombinedEnemiesStateShrink::Initialize()
{
	nextStateName_ = "WAIT_STRETCH";
	timerMax_ = 10;
	//��ɂ���p�����[�^��ۑ�
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
//�L�ёҋ@
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
//�L�ђ�
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
		//���a�������Ă�����ɐi��
		enemies_->SetCentorPos({ lerp(centorPT.x, centorPT.x + directionT.x * radiusT * 2.0f, t),
			lerp(centorPT.y, centorPT.y + directionT.y * radiusT * 2.0f, t) });
	};

	TimerUpdate(f);
}
