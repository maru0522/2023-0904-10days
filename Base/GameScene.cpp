#include "GameScene.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "Keyboard.h"
#include "Pad.h"
#include "MathUtil.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Score.h"
#include "UI.h"

void GameScene::Initialize(void)
{
    //BGM読み込み、再生
    game_BGM_ = LoadSoundMem("Resources/sound/play_BGM.mp3");
    PlaySoundMem(game_BGM_, DX_PLAYTYPE_LOOP);
    //SE読み込み
    sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");

    // (0,0) ~ (1280,720) よりちょい内側
    stage_->Initialize({ 10,10 }, { 1270,710 });

    player_->SetPos({ 300,300 });
    player_->SetRot(0);
    player_->SetRad({ 10,0 });

    EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());

    //timer_.Start(kMaxGameTimer_);
    timer_.Start(1000000000);

    ParticleManager::GetInstance()->Init();

    Score::Init();
    GameSceneUpdate();

    UI::SetPos(UIType::Lstick, { 30.f,30.f });
    UI::SetSize(UIType::Lstick, 0.2f);
    UI::SetPos(UIType::Move, { 100.f,30.f });
    UI::SetSize(UIType::Move, 0.2f);

    UI::SetPos(UIType::Rbutton, { 30.f,80.f });
    UI::SetSize(UIType::Rbutton, 0.2f);
    UI::SetPos(UIType::Attack, { 120.f,80.f });
    UI::SetSize(UIType::Attack, 0.2f);

    UI::SetPos(UIType::Abutton, { 30.f,130.f });
    UI::SetSize(UIType::Abutton, 0.2f);
    UI::SetPos(UIType::Skewer, { 110.f,130.f });
    UI::SetSize(UIType::Skewer, 0.2f);
}

void GameScene::Update(void)
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
            player_->ResetSkewerInfo4Pause();
        }
    }

    if (isMenu_ == false)
    {
        switch (progress_)
        {
        case GameScene::Progress::PRE:
            // フレームカウントが規定値超えたら遷移
            if (frameCount_preGame_ > kMaxFrame_preGame_)
            {
                // 進行値をGAMEに
                progress_ = Progress::GAME;
            }

            break;
        case GameScene::Progress::GAME:
            break;
        case GameScene::Progress::POST:
            break;
        default:
            break;
        }

        if (progress_ == Progress::PRE)
        {
            frameCount_preGame_++;
        }
        else if (progress_ == Progress::GAME)
        {
            stage_->Update();

            player_->Update();
            EnemyManager::GetInstance().Update();

            if (KEYS::IsTrigger(KEY_INPUT_0))
            {
                timer_.SetEndTime(10.f);
            }

            if (timer_.GetIsEnd())
            {
                Score::HighScoreUpdate();

                PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
                //BGMストップ
                StopSoundMem(game_BGM_);
                SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::RESULT);
            }

            ParticleManager::GetInstance()->Update();
        }
        else if (progress_ == Progress::POST)
        {
            stage_->Update();

            player_->Update();
            EnemyManager::GetInstance().Update();

            if (KEYS::IsTrigger(KEY_INPUT_0))
            {
                timer_.SetEndTime(10.f);
            }

            if (timer_.GetIsEnd())
            {
                Score::HighScoreUpdate();

                PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
                //BGMストップ
                StopSoundMem(game_BGM_);
                SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::RESULT);
            }

            ParticleManager::GetInstance()->Update();
        }
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
                //BGMストップ
                StopSoundMem(game_BGM_);
                SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
            }
        }
        else if (destination_ == Destination::TITLE)
        {
            if (PadTriggerA())
            {
                PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
                //BGMストップ
                StopSoundMem(game_BGM_);
                SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::TITLE);
            }
        }
    }
}

void GameScene::GameSceneUpdate(void)
{
        stage_->Update();

        player_->Update();
        EnemyManager::GetInstance().Update();

        if (KEYS::IsTrigger(KEY_INPUT_0))
        {
            timer_.SetEndTime(10.f);
        }

        if (timer_.GetIsEnd())
        {
            Score::HighScoreUpdate();

            PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
            //BGMストップ
            StopSoundMem(game_BGM_);
            SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::RESULT);
        }

        ParticleManager::GetInstance()->Update();
}

void GameScene::Draw(void)
{
    stage_->Draw();

    player_->Draw();
    EnemyManager::GetInstance().Draw();

    ParticleManager::GetInstance()->Draw();

#ifdef DEBUG
    DrawFormatString(0, 380, Util::Color::RED, "Scene: GAME");
    DrawFormatString(0, 0, Util::Color::WHITE, "[DEBUG]key-0で終了時間を10秒に変更。既に経過してる場合はGameScene終了");
    DrawFormatString(0, 20, Util::Color::WHITE, "time: %f", timer_.GetElapsedTime());
#endif
    //DrawPad();

    Score::Draw();

    UI::Draw(UIType::Lstick);
    UI::Draw(UIType::Move);
    UI::Draw(UIType::Rbutton);
    UI::Draw(UIType::Attack);
    UI::Draw(UIType::Abutton);
    UI::Draw(UIType::Skewer);

    if (isMenu_)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawBox(0, 0, 1280, 720, Util::Color::BLACK, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawGraph(360, 120, png_retry_, 1);
        DrawGraph(360, 320, png_2title_, 1);
    }
}
