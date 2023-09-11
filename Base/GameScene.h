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
    // 定義
    enum class Progress
    {
        PRE,
        GAME,
        POST,
    };

    // 定数
    float kMaxGameTimer_{ 60.f };
    const int32_t kMaxFrame_preGame_{ 180 }; // ゲーム前イベントが何フレームか

    // 関数
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

private:
    uint32_t game_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //シーン遷移音

    // 変数
    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    Util::Timer timer_{};

    Progress progress_;
    int32_t frameCount_preGame_; // ゲーム前イベント用のフレームカウンタ。
};

