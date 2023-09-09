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

bool CombinedEnemies::GetIsMowDownTriggerAnyEnemy()
{
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDownTrigger())
		{
			return true;
		}
	}

	return false;
}

bool CombinedEnemies::GetIsMowDownTriggerEnd()
{
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsMowDownTrigger())
		{
			return false;
		}
	}

	return true;
}

bool CombinedEnemies::GetIsDockingAndSkewer()
{
	if (GetIsDockingAnyEnemy() && isSkewer_)
	{
		return true;
	}

	return false;
}

void CombinedEnemies::AllEnemiesDockingEnd()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsDocking(false);
	}
}

void CombinedEnemies::AllEnemiesEndMowDown()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsMowDown(false);
		enemy->SetIsMowDownTrigger(false);
	}
}

void CombinedEnemies::MowDownEnd()
{
	isMowDown_ = false;
	AllEnemiesEndMowDown();
}

void CombinedEnemies::MowDownTriggerEnd()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetIsMowDownTrigger(false);
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
void CombinedEnemies::Initialize(Player* player, const Vector2& direction)
{
	player_ = player;

	//
	distance_ = (player_->GetPos() - player_->GetPos() + direction.Normalize() * length * ((float)enemiesNum_ / 2.0f)).Length();

	//�����ʒu�v�Z
	CalcCentorPos(player_->GetPos(), direction.Normalize());

	//�X�e�[�g
	ChangeState("AFTER_COMBINED");
}

void CombinedEnemies::CalcCentorPos(const Vector2& targetPos, const Vector2& direction, float length)
{
	//��
	centorPos_ = targetPos + direction * length;
}

void CombinedEnemies::AnyEnemyMowDownUpdate()
{
	if (isMowDown_ || isSkewer_)
	{
		return;
	}

	for (auto& enemy : enemies_)
	{
		//��̂ł��ガ�����Ă���
		if (enemy->GetIsMowDown())
		{
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
		mowDownVec_ = enemy->GetMowDownVec();
	}
	//�G�S�̂̓ガ�����t���O
	isMowDown_ = true;
	//�X�e�[�g�ύX
	ChangeState("MOW_DOWN");
}

void CombinedEnemies::AnyEnemySkewerUpdate()
{
	bool isSkewer = false;

	//�ǂꂩ��ł��ːi����Ă邩
	for (auto& enemy : enemies_)
	{
		if (enemy->GetIsSkewer())
		{
			isSkewer = true;
			break;
		}
	}

	if (!isSkewer) { return; }

	//�ːi����鏈��
	Skewer();
}

void CombinedEnemies::Skewer()
{
	//�ǂꂩ��ł��ːi����Ă���S���t���O���Ă�
	for (auto& enemy : enemies_)
	{
		enemy->SetIsSkewer(true);
	}

	isSkewer_ = true;

	ChangeState("SKEWER");
}

void CombinedEnemies::SkewerUpdate()
{
	DirectionUpdate();
	//���̉��̕������[�̓G�ɂȂ�悤��
	float enemyRadius = length / (float)enemiesNum_ / 2.0f;
	centorPos_ = player_->GetPos4SwordBottom() + (player_->GetMoveVec() * (length / 2.0f + enemyRadius));

	if (!player_->GetIsSkewer() && isSkewer_)
	{
		Dead();
	}
}


void CombinedEnemies::EnemiesMowDownTriggerUpdate()
{
	for (auto& enemy : enemies_)
	{
		enemy->MowDownFlagUpdate();
	}
}

//-------------------------------------------------
void CombinedEnemies::Dead()
{
	for (auto& enemy : enemies_)
	{
		enemy->Dead();
	}

	isAlive_ = false;
}

//--------------------------------------------------
void CombinedEnemies::EnemiesPosUpdate()
{
	//�G����̂̏ꍇ�͓G�̍��W���g���i�����߂��Ȃǂ�����̂Łj
	if (enemiesNum_ == 1 && !GetIsMowDownTriggerAnyEnemy())
	{
		centorPos_ = enemies_[0]->GetPos();
		return;
	}

	//�����̃C���f�b�N�X
	float centorIndex = (float)enemiesNum_ / 2.0f - 0.5f;

	uint32_t count = 0;
	//�Ή������ʒu�ɔz�u
	for (auto& enemy : enemies_)
	{
		enemy->SetPos(centorPos_ + direction_ * (centorIndex - (float)count) * (length / (float)enemiesNum_));

		count++;
	}
}

void CombinedEnemies::EnemiesScaleReset()
{
	for (auto& enemy : enemies_)
	{
		enemy->SetScale({ 1.0f,1.0f });
	}
}

void CombinedEnemies::EnemiesRotUpdate()
{
	float rot = atan2f(direction_.y, direction_.x);

	for (auto& enemy : enemies_)
	{
		enemy->SetRot(rot + Math::kPI / 2.0f);
	}
}

void CombinedEnemies::SetEnemiesScale(const Vector2& scale)
{
	for (auto& enemy : enemies_)
	{
		enemy->SetScale(scale);
	}
}

void CombinedEnemies::SetScaleSinRot(float minS, float maxS, float rate, int32_t timer)
{
	int32_t count = 0;
	for (auto& enemy : enemies_)
	{
		float rot = atan2f(direction_.y, direction_.x);

		Vector2 scale = { -sinf(rot) * sinf((float)timer * rate + (float)count),
			cosf(rot) * sinf((float)timer * rate + (float)count) };

		scale += {Math::Function::Random<float>((double)minS, (double)maxS),
			Math::Function::Random<float>((double)minS, (double)maxS)};

		scale.x = min(max(scale.x, minS), maxS);
		scale.y = min(max(scale.y, minS), maxS);

		enemy->SetScale(scale);
		count++;
	}
}

void CombinedEnemies::SetScale(const Vector2& scale)
{
	for (auto& enemy : enemies_)
	{
		Vector2 scaleL = scale;

		scaleL += {Math::Function::Random<float>((double)-scale.x * 0.1f, (double)scale.x * 0.1f),
			Math::Function::Random<float>((double)-scale.y * 0.1f, (double)scale.y * 0.1f)};

		enemy->SetScale(scaleL);
	}
}

//------------------------------------------------------------------------------
void CombinedEnemies::Update()
{
	//�ガ�����̍X�V
	//AnyEnemyMowDownUpdate();
	//�ːi�̍X�V
	AnyEnemySkewerUpdate();

	//�G��̈�̂̍��W�X�V
	EnemiesPosUpdate();

	state_->Update();
	//�p�x�X�V
	EnemiesRotUpdate();

	//�ガ�����t���O�X�V
	EnemiesMowDownTriggerUpdate();

	//�G����̂̏ꍇ�͉����߂��Ȃǂ����邽��
	if (enemiesNum_ == 1)
	{
		enemies_[0]->SetPos(centorPos_);
	}
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
	enemy->SetIsDocking(false);
	enemy->SetIsMowDownTrigger(false);
	//�G�̒��������Z���Ă���
	float addRadius = Enemy::KRadius_ * 2.0f;
	length = radiusTmp_ + addRadius;
	radiusTmp_ = length;
	//�o�^
	enemies_.push_back(std::move(enemy));
	//�G�̐������Z
	enemiesNum_++;
	//���̃t���O�I�t
	AllEnemiesDockingEnd();

	//�ǂ��炩���ガ�����Ă���
	/*else*/ if (GetIsMowDown())
	{
		MowDownEnd();
		ChangeState("AFTER_COMBINED");
	}
}

void CombinedEnemies::AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combinedEneies)
{
	//���Ԓn�_�𒆐S���W�ɂ���
	centorPos_ = centorPos_ + ((combinedEneies->centorPos_ - centorPos_) / 2.0f);

	for (auto& enemy : combinedEneies->enemies_)
	{
		AddEnemy(std::move(enemy));
	}

	//�ǂ��炩���ガ�����Ă���
	if (combinedEneies->GetIsMowDown() || GetIsMowDown())
	{
		MowDownEnd();
	}

	ChangeState("AFTER_COMBINED");
}


//--------------------------------------------------------------
void CombinedEnemies::DirectionUpdate()
{
	//player�܂ł̋������v�Z
	direction_ = (player_->GetPos() - centorPos_).Normalize();
}

