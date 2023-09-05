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
    // pad-AÇ≈âÊñ êÿë÷
    if (PadTriggerA())
    {
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
    }

    // debagópkey-RÇ≈êÿë÷
#ifdef _DEBUG
    if (KEYS::IsTrigger(KEY_INPUT_SPACE))
    {
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
    }
    if (KEYS::IsTrigger(KEY_INPUT_R))
    {
        CheckConnectPad();
    }
#endif // _DEBUG
}

void TitleScene::Draw(void)
{
    DrawFormatString(0, 380, Util::Color::RED, "Scene: TITLE");
    DrawFormatString(0, 0, Util::Color::RED, "press pad-A or key-SPACE");
    DrawFormatString(0, 20, Util::Color::RED, "key-RÇ≈padê⁄ë±çƒämîF");
    DrawFormatString(0, 40, Util::Color::WHITE, GetPadConnect() ? "pad connected" : "pad isnt connected");
}
