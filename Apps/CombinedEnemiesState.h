#pragma once
#include<memory>
#include<string>
#include"Vector2.h"
#include<functional>


//�O���錾
class CombinedEnemies;

//���������G�̃X�e�[�g�e�N���X
class ICombinedEnemiesState
{
protected:
	float timer_ = 0;
	static float radiusTmp_;
	static Vector2 centorPosTmp_;
	static Vector2 directionTmp_;
	float timerMax_ = 30;
	//
	CombinedEnemies* enemies_ = nullptr;
	//
	std::string nextStateName_;

public:
	virtual ~ICombinedEnemiesState() { ; }

public:
	virtual void Initialize();
	virtual void Update() = 0;
	void TimerUpdate(std::function<void(float)> f = NULL);

	void SetCombinedEnemies(CombinedEnemies* enemies) { enemies_ = enemies; }

public:
	static std::unique_ptr<ICombinedEnemiesState>GetState(const std::string& name);
};


//�ҋ@��
class CombinedEnemiesStateMoveWait : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
};

//�k�ݒ�
class CombinedEnemiesStateShrink : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
};

//�k��ő҂��Ă�
class CombinedEnemiesStateWaitStretch : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
};

//�L�ђ�
class CombinedEnemiesStateStretch : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
};
