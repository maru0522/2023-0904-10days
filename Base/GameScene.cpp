#include "GameScene.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "Keyboard.h"
#include "Pad.h"
#include "MathUtil.h"

void GameScene::Initialize(void)
{
    // (0,0) ~ (1280,720) よりちょい内側
    stage_->Initialize({ 10,10 }, { 1270,710 });

    player_->SetPos({ 300,300 });
    player_->SetRot({ 0,0 });
    player_->SetRad({ 10,0 });

    float x = Math::Function::Random<float>(100, 1100);
    float y = Math::Function::Random<float>(100, 600);
    enemy1_->SetPos({ x,y });
    enemy1_->SetRot({ 0,0 });
    enemy1_->SetRad({ 10,0 });

    x = Math::Function::Random<float>(100, 1100);
    y = Math::Function::Random<float>(100, 600);
    enemy2_->SetPos({ x,y });
    enemy2_->SetRot({ 0,0 });
    enemy2_->SetRad({ 10,0 });

    x = Math::Function::Random<float>(100, 1100);
    y = Math::Function::Random<float>(100, 600);
    enemy3_->SetPos({ x,y });
    enemy3_->SetRot({ 0,0 });
    enemy3_->SetRad({ 10,0 });

    x = Math::Function::Random<float>(100, 1100);
    y = Math::Function::Random<float>(100, 600);
    enemy4_->SetPos({ x,y });
    enemy4_->SetRot({ 0,0 });
    enemy4_->SetRad({ 10,0 });

    x = Math::Function::Random<float>(100, 1100);
    y = Math::Function::Random<float>(100, 600);
    enemy5_->SetPos({ x,y });
    enemy5_->SetRot({ 0,0 });
    enemy5_->SetRad({ 10,0 });

    timer_.Start(kMaxGameTimer_);
}

void GameScene::Update(void)
{
    stage_->Update();

    player_->Update();
    enemy1_->Update();
    enemy2_->Update();
    enemy3_->Update();
    enemy4_->Update();
    enemy5_->Update();

    if (KEYS::IsTrigger(KEY_INPUT_0))
    {
        timer_.SetEndTime(10.f);
    }

    if (timer_.GetIsEnd())
    {
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::RESULT);
    }
}

void GameScene::Draw(void)
{
    stage_->Draw();

    player_->Draw();
    enemy1_->Draw();
    enemy2_->Draw();
    enemy3_->Draw();
    enemy4_->Draw();
    enemy5_->Draw();

    DrawFormatString(0, 380, Util::Color::RED, "Scene: GAME");
    DrawFormatString(0, 0, Util::Color::WHITE, "[DEBUG]key-0で終了時間を10秒に変更。既に経過してる場合はGameScene終了");
    DrawFormatString(0, 20, Util::Color::WHITE, "time: %f", timer_.GetElapsedTime());
}
