#include "ResultScene.h"
#include <DxLib.h>
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"
#include "SceneManager.h"

void ResultScene::Initialize(void)
{
    png_result_ = LoadGraph("Resources/result.png");
}

void ResultScene::Update(void)
{
    if (PadTriggerA())
    {
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
    }

#ifdef _DEBUG
    if (KEYS::IsTrigger(KEY_INPUT_R))
    {
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
    }
#endif // _DEBUG
}

void ResultScene::Draw(void)
{
    //ƒŠƒUƒ‹ƒg‰æ‘œ
    DrawGraph(0, 0, png_result_, true);

    DrawFormatString(0, 380, Util::Color::RED, "Scene: RESULT");
    DrawFormatString(0, 0, Util::Color::WHITE, "pad-A or key-R[DEBUG]‚Åtitle");
}
