#include "CombinedEnemiesState.h"
#include "CombinedEnemies.h"


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

void ICombinedEnemiesState::TimerUpdate(const std::function<void(float)>& f, const std::function<void()>& endF)
{
	float t = timer_ / timerMax_;

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
	}

	timer_++;
}

//-------------------------------------------------------------------------------------
//�ガ����ꒆ
void CombinedEnemiesStateMoveMowDown::Initialize()
{
	nextStateName_ = "WAIT";
	timerMax_ = Enemy::kMowRatio_;

	direction_ = enemies_->GetMowDownVec();
	//�r���ŃX�e�[�g�ς�����Ƃ��p
	enemies_->SetRadius(enemies_->GetRadiusTmp());
	enemies_->SetCentorPosTmp();
}

void CombinedEnemiesStateMoveMowDown::Update()
{

	Vector2 centorPT = enemies_->GetCentorPosTmp();

	std::function<void(float)>f = [=](float t) {
		enemies_->SetCentorPos({ lerp(centorPT.x, centorPT.x + direction_.x * Player::kKnockbackDist_, t),
			lerp(centorPT.y, centorPT.y + direction_.y * Player::kKnockbackDist_, t) });
	};

	std::function<void()>endF = [=]() {
		enemies_->SetIsMowDown(false);
		enemies_->AllEnemiesEndMowDown();
	};

	TimerUpdate(f,endF);
}


//--------------------------------------------------------------------------------------
//�ҋ@��
void CombinedEnemiesStateMoveWait::Initialize()
{
	nextStateName_ = "SHRINK";
	enemies_->SetIsMowDown(false);
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
	timerMax_ = (float)10 / (float)enemies_->GetEnemiesNum();
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
	timerMax_ = (float)10 / (float)enemies_->GetEnemiesNum();
}

void CombinedEnemiesStateStretch::Update()
{
	Vector2 centorPT = enemies_->GetCentorPosTmp();
	Vector2 directionT = enemies_->GetDirectionTmp();
	float radiusT = enemies_->GetRadiusTmp();

	std::function<void(float)>f = [=](float t) {
		enemies_->SetRadius(lerp(0, enemies_->GetRadiusTmp(), min(t, 1.0f)));
		//���a�������Ă�����ɐi��
		enemies_->SetCentorPos({ lerp(centorPT.x, centorPT.x + directionT.x * radiusT * 2.0f, t),
			lerp(centorPT.y, centorPT.y + directionT.y * radiusT * 2.0f, t) });
	};

	TimerUpdate(f);
}
