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
	void TimerUpdate(const std::function<void(float)>& f = NULL, const std::function<void()>& endF = NULL);

	void SetCombinedEnemies(CombinedEnemies* enemies) { enemies_ = enemies; }

public:
	static std::unique_ptr<ICombinedEnemiesState>GetState(const std::string& name);
};


//�ガ����ꒆ
class CombinedEnemiesStateMoveMowDown : public ICombinedEnemiesState
{
private:
	Vector2 direction_ = { 0,0 };

public:
	void Initialize()override;
	void Update()override;
};

//�ːi���ꒆ
class CombinedEnemiesStateSkewer : public ICombinedEnemiesState
{
private:
	Vector2 direction_ = { 0,0 };

public:
	void Initialize()override;
	void Update()override;
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
