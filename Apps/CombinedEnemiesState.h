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
	float timerMax_ = 30;
	//
	CombinedEnemies* enemies_ = nullptr;
	//
	std::string nextStateName_;
protected:
	//ぶるんのパラメータ
	//最小スケール
	float minScale_ = 1.0f;
	//最大スケール
	float maxScale_ = 1.0f;
	//どのくらいのスピードでやるか
	float shakeRate_ = 1.0f;

public:
	virtual ~ICombinedEnemiesState() { ; }

public:
	virtual void Initialize();
	virtual void Update() = 0;
	void TimerUpdate(const std::function<void(float)>& f = NULL, const std::function<void()>& endF = NULL);
	void ShakeUpdate(float minS, float maxS, float rate, float t);
	float GetTimeRate();

	void SetCombinedEnemies(CombinedEnemies* enemies) { enemies_ = enemies; }

public:
	static std::unique_ptr<ICombinedEnemiesState>GetState(const std::string& name);
};


//薙ぎ払われ中
class CombinedEnemiesStateMoveMowDown : public ICombinedEnemiesState
{
private:
	Vector2 direction_ = { 0,0 };

public:
	void Initialize()override;
	void Update()override;
};

//突進され中
class CombinedEnemiesStateSkewer : public ICombinedEnemiesState
{
private:
	Vector2 direction_ = { 0,0 };

public:
	void Initialize()override;
	void Update()override;
};

//合体後
class CombinedEnemiesStateAfterCombined : public ICombinedEnemiesState
{
private:

public:
	void Initialize()override;
	void Update()override;
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
