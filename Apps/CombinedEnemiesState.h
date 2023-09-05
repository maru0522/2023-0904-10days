#pragma once
#include<memory>
#include<string>
#include"Vector2.h"
#include<functional>


//前方宣言
class CombinedEnemies;

//くっついた敵のステート親クラス
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


//待機中
class CombinedEnemiesStateMoveWait : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
};

//縮み中
class CombinedEnemiesStateShrink : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
};

//縮んで待ってる
class CombinedEnemiesStateWaitStretch : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
};

//伸び中
class CombinedEnemiesStateStretch : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
};
