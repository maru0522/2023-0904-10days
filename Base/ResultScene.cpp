#include "ResultScene.h"
#include <DxLib.h>
#include "Util.h"
#include "Keyboard.h"
#include "Pad.h"
#include "SceneManager.h"

void ResultScene::Initialize(void)
{
    //�摜�ǂݍ���
    png_result_ = LoadGraph("Resources/texture/result.png");
    //BGM�ǂݍ��݁A�Đ�
    result_BGM_ = LoadSoundMem("Resources/sound/result_BGM.mp3");
    PlaySoundMem(result_BGM_, DX_PLAYTYPE_LOOP);
    //SE�ǂݍ���
    sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");
}

void ResultScene::Update(void)
{
    if (PadTriggerA())
    {
        PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
        //BGM�X�g�b�v
        StopSoundMem(result_BGM_);
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
    }

#ifdef _DEBUG
    if (KEYS::IsTrigger(KEY_INPUT_R))
    {
        PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
        //BGM�X�g�b�v
        StopSoundMem(result_BGM_);
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
    }
#endif // _DEBUG
}

void ResultScene::Draw(void)
{
    //���U���g�摜
    DrawGraph(0, 0, png_result_, true);

    DrawFormatString(0, 380, Util::Color::RED, "Scene: RESULT");
    DrawFormatString(0, 0, Util::Color::WHITE, "pad-A or key-R[DEBUG]��title");
}
