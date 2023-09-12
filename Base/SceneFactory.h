#pragma once
#include <memory>
#include <string>
#include "IScene.h"

// 新規Sceneクラスを作るごとに、こことCreateScene()に追記していくこと
#include "DemoScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "TutorialScene.h"

class SceneFactory final
{
public:
    // 定義
    enum class Usage
    {
        DEMO,
        TITLE,
        GAME,
        RESULT,
        TUTORIAL,
    };

    // 関数
    std::unique_ptr<IScene> CreateScene(Usage scene);
};

