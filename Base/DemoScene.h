#pragma once
#include <memory>
#include "IScene.h"
#include "Player.h"

class DemoScene final :
    public IScene
{
public:
    // �֐�
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

private:
    // �ϐ�
    std::unique_ptr<Player> player_{ std::make_unique<Player>() };
};

