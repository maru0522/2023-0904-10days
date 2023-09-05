#pragma once
#include <memory>
#include "IScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Util.h"

class DemoScene final :
    public IScene
{
public:
    // ä÷êî
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

private:
    // ïœêî
    //std::unique_ptr<Player> player_{ std::make_unique<Player>() };
    //std::unique_ptr<Enemy> enemy_{ std::make_unique<Enemy>(player_.get()) };
};

