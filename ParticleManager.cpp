#include "ParticleManager.h"

void ParticleManager::BossHitEnemyEffect(const Vector2& pos)
{
	enemyDeadParticle.SetPos(pos.x,pos.y);
	Vector2 s = { 1.f, 1.f };
	enemyDeadParticle.SetScale(s);
	enemyDeadParticle.Add(15, 60, 0.5f, 3.0f, {-0.5f,-0.5f}, { 0.5f,0.5f });
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
	enemyDeadParticle.DrawBoxParticle();
}

void ParticleManager::Reset()
{
}
