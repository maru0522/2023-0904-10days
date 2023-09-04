#pragma once
#include "IScene.h"
#include "SceneFactory.h"
#include <memory>

class SceneManager final
{
public:
    // �֐�
    SceneManager(void) : sceneFactory_(std::make_unique<SceneFactory>()) {}
    ~SceneManager(void);

    void RequestChangeScene(std::unique_ptr<IScene>& nextScene, int32_t waitFrame = 0);

    void Initialize(SceneFactory::Usage firstScene = SceneFactory::Usage::DEMO);
    void Update(void);
    void Draw(void);

private:
    // �ϐ�
    int32_t waitFrame_{};
    std::unique_ptr<IScene> currentScene_{ nullptr };
    std::unique_ptr<IScene> nextScene_{ nullptr };

    std::unique_ptr<SceneFactory> sceneFactory_{ nullptr };
};

// singleton�ł����đR��ׂ��N���X�����A�s�xstatic�ŌĂяo���̂����邢�̂ł��܂���
