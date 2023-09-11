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
    enum Destination
    {
        RETRY,
        TITLE,
    };

    // 定数
    const int32_t kMaxFrameSkipTutorial_{ 210 }; // pad-B何フレーム長押しでチュートリアルをスキップするか

    // 関数
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

    // 変数
    bool isMenu_; // メニュー用
    int32_t destination_; // メニュー内の次シーンの遷移先
    int32_t frameCount_skipTutorial_; // チュートリアルをスキップする用のフレームカウンタ

    uint32_t game_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //シーン遷移音
    int32_t png_retry_ = LoadGraph("Resources/texture/retry.png");
    int32_t png_2title_ = LoadGraph("Resources/texture/toTitle.png");
};

