#pragma once
#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"


//“GŠÇ—ƒNƒ‰ƒX
class EnemyManager final
{
private://•Ï”
	//“G‚Ì”z—ñ
	std::vector<std::unique_ptr<Enemy>>enemies_;
	//‚­‚Á‚Â‚¢‚½“G“¯m‚ğ‚Ü‚Æ‚ß‚½”z—ñ
	std::vector<std::unique_ptr<CombinedEnemies>> combinedEnemiesArray_;
	//ƒvƒŒƒCƒ„[‚Ìƒ|ƒCƒ“ƒ^
	Player* player_ = nullptr;

private://ŠÖ”
	EnemyManager() { ; }
	~EnemyManager() { ; }

public:
	EnemyManager& operator=(const EnemyManager& inst) = delete;
	EnemyManager(const EnemyManager& inst) = delete;

	static EnemyManager& GetInstance();

public:
	void Initialize(Player* player);
	void Update();
	void Draw();

private:
	//‚­‚Á‚Â‚¯‚éXVˆ—
	void CombinedUpdate();
	//‹øh‚µ‚Ì“G‚ğ‚­‚Á‚Â‚¯‚éˆ—
	void SkewerCombinedUpdate();

private:
	void CoalesceneceCombEnemiesEachOther(std::function<bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator,
		std::vector<std::unique_ptr<CombinedEnemies>>::iterator)> ifF);

	/// <summary>
	/// ‡‘Ì‚µ‚Ä‚é“G‚ğ‘S‚Ä’²‚×‚Ä‡‘Ì‚³‚¹‚é‚©Œˆ‚ß‚é
	/// </summary>
	/// <param name="combEnemies"></param>
	/// <param name="settingFlag"></param>
	/// <param name="ifF"></param>
	void CheckCombinedEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies, bool& settingFlag,
		std::function <bool(std::vector<std::unique_ptr<CombinedEnemies>>::iterator) > ifF);

	/// <summary>
	/// “Gˆê‘Ìˆê‘Ì‚ğ’²‚×‚Ä‡‘Ì‚³‚¹‚é‚©Œˆ‚ß‚é
	/// </summary>
	/// <param name="enemy"></param>
	/// <param name="combEnemies"></param>
	/// <param name="settingFlag"></param>
	void CheckEnemiesCondition(std::unique_ptr<CombinedEnemies>& combEnemies,
		bool& settingFlag, std::function<bool(std::unique_ptr<Enemy>&)>ifF,
		std::function<bool(int32_t)>ifF2);
	/// <summary>
	/// “G“¯m‚ğ‚Â‚Á‚­‚¯‚é‹¤’Êˆ—
	/// </summary>
	/// <param name="combEnemies">‚­‚Á‚Â‚¢‚Ä‚é“G‚Ì”z—ñiV‚½‚Éì‚ç‚ê‚éê‡‚Ínullptrj</param>
	/// <param name="isCombined">“Gˆê‘Ìˆê‘Ì‚ğ’Ç‰Á‚µ‚Ä‚¢‚­‚©</param>
	/// <param name="isDockCombined">‚·‚Å‚É‚ ‚é‡‘Ì‚µ‚½“G‚É’Ç‰Á‚µ‚Ä‚¢‚­‚©</param>
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combEnemies, bool isCombined, bool isDockCombined,
		std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)> addIfF);

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);
};


