#include "TutorialScene.h"
#include "Pad.h"
#include "SceneManager.h"

void TutorialScene::Initialize(void)
{
    //BGM�ǂݍ��݁A�Đ�
    game_BGM_ = LoadSoundMem("Resources/sound/play_BGM.mp3");
    PlaySoundMem(game_BGM_, DX_PLAYTYPE_LOOP);
    //SE�ǂݍ���
    sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");

}

void TutorialScene::Update(void)
{
    if (PadTriggerMenu())
    {
        if (isMenu_)
        {
            isMenu_ = false;
        }
        else
        {
            isMenu_ = true;
            //player_->ResetSkewerInfo4Pause();
        }
    }

    if (PadDownB()) // pad-B�������Ń`���[�g���A���X�L�b�v�p�̃J�E���^���Z
    {
        frameCount_skipTutorial_++;
        if (frameCount_skipTutorial_ > kMaxFrameSkipTutorial_)
        {
            PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
            //BGM�X�g�b�v
            StopSoundMem(game_BGM_);
            SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
        }
    }
    else // �����񂭂Ȃ����珉����
    {
        // ������
        frameCount_skipTutorial_ = 0;
    }

    if (isMenu_ == false)
    {
        
    }
    else
    {
        if (PadStick().y >= 0.3f)
        {
            destination_++;
            destination_ = (std::min)(destination_, 1);
        }
        else if (PadStick().y <= -0.3f)
        {
            destination_--;
            destination_ = (std::max)(destination_, 0);
        }

        if (destination_ == Destination::RETRY)
        {
            if (PadTriggerA())
            {
                PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
                //BGM�X�g�b�v
                StopSoundMem(game_BGM_);
                SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
            }
        }
        else if (destination_ == Destination::TITLE)
        {
            if (PadTriggerA())
            {
                PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
                //BGM�X�g�b�v
                StopSoundMem(game_BGM_);
                SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
            }
        }
    }
}

void TutorialScene::Draw(void)
{
}
