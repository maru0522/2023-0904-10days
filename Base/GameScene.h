#pragma once
#include <memory>
#include "IScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Util.h"
#include "Stage.h"
#include "CollisionManger.h"

class GameScene :
    public IScene
{
public:
    // ’è”
    float kMaxGameTimer_{ 60.f };

    // ŠÖ”
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

private:
    uint32_t game_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //ƒV[ƒ“‘JˆÚ‰¹

    // •Ï”
    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    Util::Timer timer_{};
};

