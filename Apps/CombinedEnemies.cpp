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

	//�����ʒu�v�Z
	CalcCentorPos(player_->GetPos(), direciton.Normalize());

	//�X�e�[�g
	ChangeState("WAIT");
}

void CombinedEnemies::CalcCentorPos(const Vector2& targetPos, const Vector2& direciton)
{
	//��
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
		//��̂ł��ガ�����Ă���
		if (enemy->GetIsMowDown())
		{
			mowDownVec_ = enemy->GetMowDownVec();
			//�ガ��������
			MowDown();
			break;
		}
	}
}

void CombinedEnemies::MowDown()
{
	for (auto& enemy : enemies_)
	{
		//�G�P�̂̓ガ�����t���O
		enemy->SetIsMowDown(true);
	}
	//�G�S�̂̓ガ�����t���O
	isMowDown_ = true;
	//�X�e�[�g�ύX
	ChangeState("MOW_DOWN");
}

//--------------------------------------------------
void CombinedEnemies::EnemiesPosUpdate()
{
	//�����̃C���f�b�N�X
	float centorIndex = (float)enemiesNum_ / 2.0f - 0.5f;

	uint32_t count = 0;
	//�Ή������ʒu�ɔz�u
	for (auto& enemy : enemies_)
	{
		enemy->SetPos(centorPos_ + direction_ * (centorIndex - (float)count) * (radius_ / (float)enemiesNum_));

		count++;
	}
}

void CombinedEnemies::Update()
{
	//�ガ�����̍X�V
	AnyEnemyMowDownUpdate();

	state_->Update();
	//�G��̈�̂̍��W�X�V
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
	//�G�̒��������Z���Ă���
	float addRadius = enemy->GetRad().Length() * 2.0f * (1.0f / (1.0f - Enemy::kPngScale_));
	radius_ = radiusTmp_ + addRadius;
	radiusTmp_ = radius_;
	//�o�^
	enemies_.push_back(std::move(enemy));
	//�G�̐������Z
	enemiesNum_++;
}

void CombinedEnemies::AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combinedEneies)
{
	//���Ԓn�_�𒆐S���W�ɂ���
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
	//player�܂ł̋������v�Z
	direction_ = (player_->GetPos() - centorPos_).Normalize();
}

