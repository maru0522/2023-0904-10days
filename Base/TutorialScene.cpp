#include "TutorialScene.h"
#include "Pad.h"
#include "SceneManager.h"
#include "EnemyManager.h"
#include "Keyboard.h"
#include "Score.h"
#include "ParticleManager.h"
#include "UI.h"

void TutorialScene::Initialize(void)
{
    //BGM読み込み、再生
    game_BGM_ = LoadSoundMem("Resources/sound/play_BGM.mp3");
    PlaySoundMem(game_BGM_, DX_PLAYTYPE_LOOP);
    //SE読み込み
    sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");

    // (0,0) ~ (1280,720) よりちょい内側
    stage_->Initialize({ 10,10 }, { 1270,710 });

    player_->SetPos({ 640,360 });
    player_->SetRot(0);
    player_->SetRad({ 10,0 });
    player_->SetVecMove({ 0,-1 });

    EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());

    //timer_.Start(kMaxGameTimer_);
    timer_.Start(1000000000);

    ParticleManager::GetInstance()->Init();

    Score::Init();
    TutorialSceneUpdate();

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

    UI::SetPos(UIType::Tutorial1, { 1000,500 });
    UI::SetSize(UIType::Tutorial1, 0.6f);
    UI::SetPos(UIType::Tutorial2, { 1000,300 });
    UI::SetSize(UIType::Tutorial2, 0.6f);
    UI::SetPos(UIType::Tutorial3, { 1000,500 });
    UI::SetSize(UIType::Tutorial3, 0.6f);
    UI::SetPos(UIType::Tutorial4, { 1000,500 });
    UI::SetSize(UIType::Tutorial4, 0.6f);
    UI::SetPos(UIType::Tutorial5, { 1000,500 });
    UI::SetSize(UIType::Tutorial5, 0.6f);
    UI::SetPos(UIType::Tutorial6, { 1000,500 });
    UI::SetSize(UIType::Tutorial6, 0.6f);
    UI::SetPos(UIType::Tutorial7, { 1000,500 });
    UI::SetSize(UIType::Tutorial7, 0.6f);
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

    if (PadDownB()) // pad-B長押しでチュートリアルスキップ用のカウンタ加算
    {
        frameCount_skipTutorial_++;
        if (frameCount_skipTutorial_ > kMaxFrameSkipTutorial_)
        {
            PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
            //BGMストップ
            StopSoundMem(game_BGM_);
            SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
        }
    }
    else // 押さんくなったら初期化
    {
        // 初期化
        frameCount_skipTutorial_ = 0;
    }

    if (isMenu_ == false)
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

        if (progressNum_ == 1)
        {
            // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // 現在フレームと最大値の割合算出
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // 割合から、α値を段々減算
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // いったん仮でRB押しで次へ進行
            if (PadTriggerRB())
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;
            }

            // フレームカウンタが起動されてたら加算し続ける
            if (frameCount_sign_)
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;

                // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
                if (frameCount_sign_ > kMaxFrame4StartTrans_ +  kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // カウンタ初期化して
                    frameCount_sign_ = 0;
                    // α値初期化して
                    alpha_sign_ = 255;
                    // 表示する看板を次のものへ
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 2)
        {
            // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // 現在フレームと最大値の割合算出
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // 割合から、α値を段々減算
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // いったん仮でA押し && スティック入力してたら
            if (PadDownA() && PadStick().IsNonZero()) // ここスローモーションかかるせいで時間かかりすぎ、エンジンならこうはならないと思うけど
            {
                // ここの加算は最初の一回のみにしたい
                if (frameCount_sign_ == 0)
                {
                    // 看板用フレームカウンタ加算
                    frameCount_sign_++;
                }
            }

            // フレームカウンタが起動されてたら加算し続ける
            if (frameCount_sign_)
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;

                // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // カウンタ初期化して
                    frameCount_sign_ = 0;
                    // α値初期化して
                    alpha_sign_ = 255;
                    // 表示する看板を次のものへ
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 3)
        {
            // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // 現在フレームと最大値の割合算出
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // 割合から、α値を段々減算
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // いったん仮でpad-AをReleaseで
            if (PadReleaseA()) // ここの挙動ちょっと怪しい
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;
            }

            // フレームカウンタが起動されてたら加算し続ける
            if (frameCount_sign_)
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;

                // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // カウンタ初期化して
                    frameCount_sign_ = 0;
                    // α値初期化して
                    alpha_sign_ = 255;
                    // 表示する看板を次のものへ
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 4) // "8体くっつけてデカ団子に！"
        {
            // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // 現在フレームと最大値の割合算出
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // 割合から、α値を段々減算
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // いったん仮でRB押しで次へ進行
            if (PadTriggerRB())
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;
            }

            // フレームカウンタが起動されてたら加算し続ける
            if (frameCount_sign_)
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;

                // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // カウンタ初期化して
                    frameCount_sign_ = 0;
                    // α値初期化して
                    alpha_sign_ = 255;
                    // 表示する看板を次のものへ
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 5) // "倒すと高得点"
        {
            // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // 現在フレームと最大値の割合算出
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // 割合から、α値を段々減算
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // いったん仮でRB押しで次へ進行
            if (PadTriggerRB())
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;
            }

            // フレームカウンタが起動されてたら加算し続ける
            if (frameCount_sign_)
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;

                // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // カウンタ初期化して
                    frameCount_sign_ = 0;
                    // α値初期化して
                    alpha_sign_ = 255;
                    // 表示する看板を次のものへ
                    progressNum_++;
                }
            }
        }
        else if (progressNum_ == 6) // "それではご武運を"
        {
            // 透明化しはじめるまでのフレームを待つため、kMaxFrame4StartTrans_の値を超えるまでは、rateの計算が出来ないようにする
            float frame = (float)(std::max)(frameCount_sign_ - kMaxFrame4StartTrans_, 0);

            // 現在フレームと最大値の割合算出
            float rate = (std::min)(frame / kMaxFrameTransColor_, 1.f);
            // 割合から、α値を段々減算
            alpha_sign_ = (int32_t)((1 - rate) * 255);

            // いったん仮でRB押しで次へ進行
            if (PadTriggerRB())
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;
            }

            // フレームカウンタが起動されてたら加算し続ける
            if (frameCount_sign_)
            {
                // 看板用フレームカウンタ加算
                frameCount_sign_++;

                // フレームカウンタの値が、(透明化し始めるまで + 透明になるまで + 次の間隔)を上回ったら
                if (frameCount_sign_ > kMaxFrame4StartTrans_ + kMaxFrameTransColor_ + kIntervalNextSign_)
                {
                    // カウンタ初期化して
                    frameCount_sign_ = 0;
                    // α値初期化して
                    alpha_sign_ = 255;
                    // 表示する看板を次のものへ
                    progressNum_++;

                    PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
                    //BGMストップ
                    StopSoundMem(game_BGM_);
                    SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::GAME);
                }
            }
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

void TutorialScene::TutorialSceneUpdate(void)
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

void TutorialScene::Draw(void)
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
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    //DrawBox(0, 0, 1280, 720, Util::Color::BLACK, true);
    //SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    if (progressNum_ == 1)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_sign_);
        UI::Draw(UIType::Tutorial1);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
    }
    else if(progressNum_ == 2)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_sign_);
        UI::Draw(UIType::Tutorial2);
        UI::Draw(UIType::Tutorial3);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
    }
    else if (progressNum_ == 3)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_sign_);
        UI::Draw(UIType::Tutorial4);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
    }
    else if (progressNum_ == 4)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_sign_);
        UI::Draw(UIType::Tutorial5);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
    }
    else if (progressNum_ == 5)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_sign_);
        UI::Draw(UIType::Tutorial6);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
    }
    else if (progressNum_ == 6)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_sign_);
        UI::Draw(UIType::Tutorial7);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
    }

    if (isMenu_)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawBox(0, 0, 1280, 720, Util::Color::BLACK, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        if (destination_ == Destination::RETRY)
        {
            SetDrawBright(180, 0, 0);
            DrawGraph(360, 120, png_retry_, 1);
            SetDrawBright(255, 255, 255);
        }
        else
        {
            DrawGraph(360, 120, png_retry_, 1);
        }

        if (destination_ == Destination::TITLE)
        {
            SetDrawBright(180, 0, 0);
            DrawGraph(360, 320, png_2title_, 1);
            SetDrawBright(255, 255, 255);
        }
        else
        {
            DrawGraph(360, 320, png_2title_, 1);
        }
    }
}
