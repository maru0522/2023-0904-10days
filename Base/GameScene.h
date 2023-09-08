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
    // �萔
    float kMaxGameTimer_{ 60.f };

    // �֐�
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

private:
    uint32_t game_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //�V�[���J�ډ�

    // �ϐ�
    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    Util::Timer timer_{};
};

