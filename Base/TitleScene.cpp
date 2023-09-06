#include "TitleScene.h"
#include <DxLib.h>
#include "Util.h"
#include "Pad.h"
#include "SceneManager.h"
#include "Keyboard.h"

void TitleScene::Initialize(void)
{
    CheckConnectPad();

    titleImageHandle_ = LoadGraph("Resources/title.png");
}

void TitleScene::Update(void)
{
    // pad-Aで画面切替
    if (PadTriggerA())
    {
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
    }

    // debag用key-Rで切替
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
    
    //タイトル
    DrawGraph(0, 0, titleImageHandle_,true);

    DrawFormatString(0, 380, Util::Color::RED, "Scene: TITLE");
    DrawFormatString(0, 0, Util::Color::RED, "press pad-A or key-SPACE");
    DrawFormatString(0, 20, Util::Color::RED, "key-Rでpad接続再確認");
    DrawFormatString(0, 40, Util::Color::WHITE, GetPadConnect() ? "pad connected" : "pad isnt connected");
}
