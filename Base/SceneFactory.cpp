#include "SceneFactory.h"

std::unique_ptr<IScene> SceneFactory::CreateScene(Usage scene)
{
    // �V�[����DEMO�Ȃ�
    if (scene == Usage::DEMO) { return std::make_unique<DemoScene>(); }
    // �V�[����TITLE�Ȃ�
    if (scene == Usage::TITLE) { return std::make_unique<TitleScene>(); }
    // �V�[����GAME�Ȃ�
    if (scene == Usage::GAME) { return std::make_unique<GameScene>(); }
    // �V�[����result�Ȃ�
    if (scene == Usage::RESULT) { return std::make_unique<ResultScene>(); }
    // �V�[����tutorial�Ȃ�
    if (scene == Usage::TUTORIAL) { return std::make_unique<TutorialScene>(); }

    // �����w�肪�Ȃ��ꍇDEMO�V�[����Ԃ�
    return std::make_unique<DemoScene>();
}
