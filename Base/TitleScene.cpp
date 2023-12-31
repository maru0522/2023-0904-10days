#include "TitleScene.h"
#include <DxLib.h>
#include "Util.h"
#include "Pad.h"
#include "SceneManager.h"
#include "Keyboard.h"
#include "Score.h"
#include "UI.h"

void TitleScene::Initialize(void)
{
    CheckConnectPad();
    //画像読み込み
    titleImageHandle_ = LoadGraph("Resources/texture/title.png");
    //BGM読み込み、再生
    title_BGM_ = LoadSoundMem("Resources/sound/title_BGM.mp3");
    PlaySoundMem(title_BGM_, DX_PLAYTYPE_LOOP);
    //SE読み込み
    decision_SE_ = LoadSoundMem("Resources/sound/decision_SE.mp3");
    sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");

    Score::LoadScore();
    UI::SetPos(UIType::Abutton, { 620.f,600.f });
    UI::SetSize(UIType::Abutton, 0.8f);
}

void TitleScene::Update(void)
{
    // pad-Aで画面切替
    if (PadTriggerA())
    {
        PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
        //BGMストップ
        StopSoundMem(title_BGM_);
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TUTORIAL);
    }

    // debag用key-Rで切替
#ifdef _DEBUG
    if (KEYS::IsTrigger(KEY_INPUT_SPACE))
    {
        PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
        //BGMストップ
        StopSoundMem(title_BGM_);
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TUTORIAL);
    }
#endif // _DEBUG
    if (KEYS::IsTrigger(KEY_INPUT_R))
    {
        PlaySoundMem(decision_SE_, DX_PLAYTYPE_NORMAL);
        CheckConnectPad();
    }
}

void TitleScene::Draw(void)
{
    //タイトル
    DrawGraph(0, 0, titleImageHandle_,true);

#ifdef _DEBUG
    DrawFormatString(0, 380, Util::Color::RED, "Scene: TITLE");
    DrawFormatString(0, 0, Util::Color::RED, "press pad-A or key-SPACE");
    DrawFormatString(0, 20, Util::Color::RED, "key-Rでpad接続再確認");
    DrawFormatString(0, 40, Util::Color::WHITE, GetPadConnect() ? "pad connected" : "pad isnt connected");
#endif

    UI::Draw(UIType::Abutton);
}
