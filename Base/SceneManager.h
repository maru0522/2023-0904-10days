#pragma once
#include "IScene.h"
#include "SceneFactory.h"
#include <memory>

class SceneManager final
{
public:
    // 定数
    const int32_t kSlowFrameRatio_{ 5 }; // 何Frameに一回更新処理を入れるか（スローモーションの割合）

    // 関数
    static SceneManager* GetInstance(void);

    void RequestChangeScene(SceneFactory::Usage nextScene, int32_t waitFrame = 0);

    void Initialize(SceneFactory::Usage firstScene = SceneFactory::Usage::DEMO);
    void Update(void);
    void Draw(void);

    void StartSlowMotion(void);
    void EndSlowMotion(void);

private:
    // 変数
    int32_t waitFrame_{};
    int32_t frameCount_slowMotion_{};

    std::unique_ptr<IScene> currentScene_{ nullptr };
    std::unique_ptr<IScene> nextScene_{ nullptr };

    std::unique_ptr<SceneFactory> sceneFactory_{ nullptr };

    // singleton
    SceneManager(void) : sceneFactory_(std::make_unique<SceneFactory>()) {}
    ~SceneManager(void);
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    // getter
    int32_t GetFrameCountWhileSlow(void) { return frameCount_slowMotion_; }
};
