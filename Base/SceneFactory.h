#pragma once
#include <memory>
#include <string>
#include "IScene.h"

// �V�KScene�N���X����邲�ƂɁA������CreateScene()�ɒǋL���Ă�������
#include "DemoScene.h"
#include "TitleScene.h"

class SceneFactory final
{
public:
    // ��`
    enum class Usage
    {
        DEMO,
        TITLE,
    };

    // �֐�
    std::unique_ptr<IScene> CreateScene(Usage scene);
};

