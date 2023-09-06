#include "TitleScene.h"
#include <DxLib.h>
#include "Util.h"
#include "Pad.h"
#include "SceneManager.h"
#include "Keyboard.h"

void TitleScene::Initialize(void)
{
    CheckConnectPad();
    //�摜�ǂݍ���
    titleImageHandle_ = LoadGraph("Resources/texture/title.png");
    //BGM�ǂݍ��݁A�Đ�
    title_BGM_ = LoadSoundMem("Resources/sound/title_BGM.mp3");
    PlaySoundMem(title_BGM_, DX_PLAYTYPE_LOOP);
    //SE�ǂݍ���
    decision_SE_ = LoadSoundMem("Resources/sound/decision_SE.mp3");
    sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");
}

void TitleScene::Update(void)
{
    // pad-A�ŉ�ʐؑ�
    if (PadTriggerA())
    {
        PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
        //BGM�X�g�b�v
        StopSoundMem(title_BGM_);
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
    }

    // debag�pkey-R�Őؑ�
#ifdef _DEBUG
    if (KEYS::IsTrigger(KEY_INPUT_SPACE))
    {
        PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
        //BGM�X�g�b�v
        StopSoundMem(title_BGM_);
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
    }
    if (KEYS::IsTrigger(KEY_INPUT_R))
    {
        PlaySoundMem(decision_SE_, DX_PLAYTYPE_NORMAL);
        CheckConnectPad();
    }
#endif // _DEBUG
}

void TitleScene::Draw(void)
{
    //�^�C�g��
    DrawGraph(0, 0, titleImageHandle_,true);

    DrawFormatString(0, 380, Util::Color::RED, "Scene: TITLE");
    DrawFormatString(0, 0, Util::Color::RED, "press pad-A or key-SPACE");
    DrawFormatString(0, 20, Util::Color::RED, "key-R��pad�ڑ��Ċm�F");
    DrawFormatString(0, 40, Util::Color::WHITE, GetPadConnect() ? "pad connected" : "pad isnt connected");
}
