#pragma once
#include <memory>
#include "IScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Util.h"
#include "Stage.h"

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
    // •Ï”
    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(stage_.get()) };
    std::unique_ptr<Enemy> enemy_{ std::make_unique<Enemy>(player_.get(), stage_.get()) };
    Util::Timer timer_{};
};

