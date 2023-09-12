#pragma once
#include "IScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Util.h"
#include "Stage.h"
#include "CollisionManger.h"

class TutorialScene :
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

    enum Destination
    {
        RETRY,
        TITLE,
    };

    // 定数
    const int32_t kMaxFrameSkipTutorial_{ 210 }; // pad-B何フレーム長押しでチュートリアルをスキップするか
    const int32_t kMaxFrame4StartTrans_{ 50 }; // 透明化しはじめるまで何フレームかかるか
    const int32_t kMaxFrameTransColor_{ 90 }; // 透明になるまで何フレームかかるか
    const int32_t kIntervalNextSign_{ 50 }; // 透明化してから次の看板表示まで何フレーム待つか

    // 関数
    void Initialize(void) override;
    void Update(void) override;
    void TutorialSceneUpdate(void);
    void Draw(void) override;
    void Finalize(void) override {};

    // 変数
    bool isMenu_; // メニュー用
    int32_t destination_; // メニュー内の次シーンの遷移先
    int32_t frameCount_skipTutorial_; // チュートリアルをスキップする用のフレームカウンタ

    uint32_t progressNum_{ 1 }; // チュートリアル進行度
    int32_t frameCount_sign_; // 看板を透明にするときのカウンタ
    int32_t alpha_sign_{ 255 }; // 看板の透明度

    std::unique_ptr<Stage> stage_{ std::make_unique<Stage>() };

    std::unique_ptr<Player> player_{ std::make_unique<Player>(CollisionManger::GetInstance(), stage_.get()) };
    Util::Timer timer_{};

    uint32_t game_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //シーン遷移音
    int32_t png_retry_ = LoadGraph("Resources/texture/retry.png");
    int32_t png_2title_ = LoadGraph("Resources/texture/toTitle.png");
};

