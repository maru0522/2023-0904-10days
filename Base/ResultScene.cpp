#include "ResultScene.h"
#include <DxLib.h>
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"
#include "SceneManager.h"
#include "Score.h"
#include "UI.h"

void ResultScene::Initialize(void)
{
    //画像読み込み
    png_result_ = LoadGraph("Resources/texture/result.png");
    //BGM読み込み、再生
    result_BGM_ = LoadSoundMem("Resources/sound/result_BGM.mp3");
    PlaySoundMem(result_BGM_, DX_PLAYTYPE_LOOP);
    //SE読み込み
    sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");

    Score::SaveScore();
    UI::SetPos(UIType::Abutton, { 620.f,600.f });
    UI::SetSize(UIType::Abutton, 0.8f);
}

void ResultScene::Update(void)
{
    if (PadTriggerA())
    {
        PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
        //BGMストップ
        StopSoundMem(result_BGM_);
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
    }

#ifdef _DEBUG
    if (KEYS::IsTrigger(KEY_INPUT_R))
    {
        PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
        //BGMストップ
        StopSoundMem(result_BGM_);
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
    }
#endif // _DEBUG
}

void ResultScene::Draw(void)
{
    //リザルト画像
    DrawGraph(0, 0, png_result_, true);

#ifdef DEBUG
    DrawFormatString(0, 380, Util::Color::RED, "Scene: RESULT");
    DrawFormatString(0, 0, Util::Color::WHITE, "pad-A or key-R[DEBUG]でtitle");
#endif

    Score::Draw();
    UI::Draw(UIType::Abutton);
}
