#pragma once
#include "IScene.h"
#include "SceneFactory.h"
#include <memory>
#include "SceneTransition.h"

class SceneManager final
{
public:
    // �萔
    const int32_t kSlowFrameRatio_{ 5 }; // ��Frame�Ɉ��X�V���������邩�i�X���[���[�V�����̊����j�̏����l

    // �֐�
    static SceneManager* GetInstance(void);

    void RequestChangeScene(SceneFactory::Usage nextScene, int32_t waitFrame = SceneTransition::kTotalFrame_);

    void Initialize(SceneFactory::Usage firstScene = SceneFactory::Usage::DEMO);
    void Update(void);
    void Draw(void);

    void StartSlowMotion(int32_t slowFrameRatio);
    void EndSlowMotion(void);

private:
    // �ϐ�
    int32_t slowFrameRatio_{ kSlowFrameRatio_ }; // ��Frame�Ɉ��X�V���������邩�i�X���[���[�V�����̊����j�̏����l

    int32_t waitFrame_{};
    int32_t frameCount_slowMotion_{};
    int32_t frameCount_debug_{};

    std::unique_ptr<IScene> currentScene_{ nullptr };
    std::unique_ptr<IScene> nextScene_{ nullptr };

    std::unique_ptr<SceneFactory> sceneFactory_{ nullptr };
    SceneTransition transition_;

    // singleton
    SceneManager(void) : sceneFactory_(std::make_unique<SceneFactory>()) {}
    ~SceneManager(void);
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

public:
    // getter
    int32_t GetFrameCountWhileSlow(void) { return frameCount_slowMotion_; }
};
