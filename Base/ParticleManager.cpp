#include "ParticleManager.h"

void ParticleManager::BossHitEnemyEffect(const Vector2& pos)
{
	enemyDeadParticle.SetPos(pos.x, pos.y);
	Vector2 s = { 1.f, 1.f };
	enemyDeadParticle.SetScale(s);
	enemyDeadParticle.Add(40, 0.5f, 15.0f, 30.0f, { -5.0f,-5.0f }, { 5.0f,5.0f },{0,0},0,0,0xff3232);
}

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Init()
{
	enemyDeadParticle.Init();
}

void ParticleManager::Update()
{
	enemyDeadParticle.Update();
}

void ParticleManager::Draw()
{
	enemyDeadParticle.DrawCircleParticle();
}

void ParticleManager::Reset()
{
}
