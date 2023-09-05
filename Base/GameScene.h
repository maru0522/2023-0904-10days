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
    // 定数
    float kMaxGameTimer_{ 60.f };

    // 関数
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

private:
    // 変数
    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    Util::Timer timer_{};
};

