#pragma once
#include "CombinedEnemies.h"
#include "CombinedEnemiesState.h"
#include "Stage.h"


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
	//ƒXƒe[ƒW‚Ì
	Stage* stage_ = nullptr;
	//UŒ‚‚É“–‚½‚Á‚½“G‚ğ•ÛŠÇ
	std::unique_ptr<CombinedEnemies>mowDownedEnemies_ = nullptr;

	//“G‚ÌãŒÀ
	const uint32_t ENEMIES_MAX_ = 5;

private://ŠÖ”
	EnemyManager() { ; }
	~EnemyManager() { ; }

public:
	EnemyManager& operator=(const EnemyManager& inst) = delete;
	EnemyManager(const EnemyManager& inst) = delete;

	static EnemyManager& GetInstance();

public:
	void Initialize(Player* player, Stage* stage);
	void Update();
	void Draw();

private:
	//“ã‚¬•¥‚í‚ê‚½“G‚ğ‚Ü‚Æ‚ß‚Ä•Û‘¶‚µ‚Ä‚¨‚­
	void SaveMowDownEnemies();
	//‚­‚Á‚Â‚¯‚éXVˆ—
	void CombinedUpdate();
	//‹øh‚µ‚Ì“G‚ğ‚­‚Á‚Â‚¯‚éˆ—
	void SkewerCombinedUpdate();

	//¶¬‚ÌXV
	void GenerateUpdate();
	//“ã‚¬•¥‚í‚ê‚Ä‚éÅ’†‚Ì“G‚Ì‰ò‚ÌXV
	void MowDownTriggerEnemiesUpdate();

	//“ã‚¬•¥‚í‚ê‚½uŠÔ‚ğI‚í‚ç‚¹‚é
	void MowDownTriggerEnd();

private:
	//“G‚Ì”‚ğæ“¾
	int32_t GetEnemiesCount();

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
	/// <param name="isPushBackArray">”z—ñ‚É’Ç‰Á‚·‚é‚©</param>
	void AddCombinedEnemies(std::unique_ptr<CombinedEnemies> combEnemies, bool isCombined, bool isDockCombined,
		std::function<bool(std::vector<std::unique_ptr<Enemy>>::iterator)> addIfF, bool isPushBackArray = true);

public:
	void AddEnemy(std::unique_ptr<Enemy>enemy);

public:
	//“Ëi‚³‚ê‚Ä‚é“G‚Ì‰ò‚Ì”¼Œa
	float GetSkewerEnemiesRadius();
};


