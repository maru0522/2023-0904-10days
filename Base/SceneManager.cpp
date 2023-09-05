#include "SceneManager.h"

SceneManager::~SceneManager(void)
{
    if (currentScene_) {
        currentScene_->Finalize();
        currentScene_.reset();
    }
}

SceneManager* SceneManager::GetInstance(void)
{
    static SceneManager sceneM;
    return &sceneM;
}

void SceneManager::RequestChangeScene(SceneFactory::Usage nextScene, int32_t waitFrame)
{
    nextScene_ = std::move(sceneFactory_->CreateScene(nextScene));
    waitFrame_ = waitFrame;
}

void SceneManager::Initialize(SceneFactory::Usage firstScene)
{
    // 最初のシーンを生成
    currentScene_ = sceneFactory_->CreateScene(firstScene);
    currentScene_->Initialize();
}

void SceneManager::Update(void)
{
    // スローモーションなし
    if (frameCount_slowMotion_ == 0)
    {
        // 次シーンの予約がある
        if (nextScene_) {
            // 待機フレーム指定がない && 現在シーンがnullptrではない
            if (waitFrame_ == 0 && currentScene_) {
                currentScene_->Finalize();
                currentScene_.reset();
            }

            // シーン移行
            currentScene_ = std::move(nextScene_); // 管理権限移譲
            nextScene_.reset();                    // 次シーンをnullptrにする
            currentScene_->Initialize();           // 現在シーンを初期化
        }

        // 現在シーンUpdate()
        currentScene_->Update();


        // 待機フレームを減少させる
        waitFrame_--;
        // 待機フレームは0以下にならない
        waitFrame_ = (std::max)(waitFrame_, 0);
    }
    else // スローモーションあり
    {
        // 規定値で割ったときの余りが0の時のみ、各種Update()を回す。
        if (frameCount_slowMotion_ % kSlowFrameRatio_ == 0)
        {
            // 次シーンの予約がある
            if (nextScene_) {
                // 待機フレーム指定がない && 現在シーンがnullptrではない
                if (waitFrame_ == 0 && currentScene_) {
                    currentScene_->Finalize();
                    currentScene_.reset();
                }

                // シーン移行
                currentScene_ = std::move(nextScene_); // 管理権限移譲
                nextScene_.reset();                    // 次シーンをnullptrにする
                currentScene_->Initialize();           // 現在シーンを初期化
            }

            // 現在シーンUpdate()
            currentScene_->Update();


            // 待機フレームを減少させる
            waitFrame_--;
            // 待機フレームは0以下にならない
            waitFrame_ = (std::max)(waitFrame_, 0);
        }
        frameCount_slowMotion_++;
    }
}

void SceneManager::Draw(void)
{
    currentScene_->Draw();
}

void SceneManager::StartSlowMotion(void)
{
    frameCount_slowMotion_++;
}

void SceneManager::EndSlowMotion(void)
{
    frameCount_slowMotion_ = 0;
}
