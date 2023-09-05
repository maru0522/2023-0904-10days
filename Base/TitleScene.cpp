#include "TitleScene.h"
#include <DxLib.h>
#include "Util.h"
#include "Pad.h"
#include "SceneManager.h"
#include "Keyboard.h"

void TitleScene::Initialize(void)
{
    CheckConnectPad();
}

void TitleScene::Update(void)
{
    // pad-A‚Å‰æ–ÊØ‘Ö
    if (PadTriggerA())
    {
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
    }

    // debag—pkey-R‚ÅØ‘Ö
#ifdef _DEBUG
    if (KEYS::IsTrigger(KEY_INPUT_R))
    {
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
    }
#endif // _DEBUG
}

void TitleScene::Draw(void)
{
    DrawFormatString(0, 380, Util::Color::RED, "Scene: TITLE");
    DrawFormatString(0, 0, Util::Color::RED, "press pad-A or Space");
    DrawFormatString(0, 20, Util::Color::WHITE, GetPadConnect() ? "pad connected" : "pad isnt connected");
}
