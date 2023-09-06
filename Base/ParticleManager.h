#pragma once
#include "Particle.h"
#include <list>

class ParticleManager final
{
private:
	ParticleManager() {};

private:
	// �p�[�e�B�N���̃C���X�^���X
	Emitter enemyDeadParticle;		//�G���񂾎�

public:
	//�{�X�ɓG�𓖂Ă���
	void BossHitEnemyEffect(const Vector2& pos);

public:
	static ParticleManager* GetInstance();

	void Init();
	void Update();
	void Draw();
	void Reset();
};