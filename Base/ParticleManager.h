#pragma once
#include "Particle.h"
#include <list>

class ParticleManager final
{
private:
	ParticleManager() {};

private:
	// パーティクルのインスタンス
	Emitter enemyDeadParticle;		//敵死んだ時

public:
	//ボスに敵を当てた時
	void BossHitEnemyEffect(const Vector2& pos);

public:
	static ParticleManager* GetInstance();

	void Init();
	void Update();
	void Draw();
	void Reset();
};