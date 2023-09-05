#include "GameScene.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "Keyboard.h"
#include "Pad.h"

void GameScene::Initialize(void)
{
    player_->SetPos({ 300,300 });
    player_->SetRot({ 0,0 });
    player_->SetRad({ 10,0 });

    enemy_->SetPos({ 150,150 });
    enemy_->SetRot({ 0,0 });
    enemy_->SetRad({ 10,0 });

    timer_.Start(kMaxGameTimer_);
}

void GameScene::Update(void)
{
    player_->Update();
    enemy_->Update();

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
    player_->Draw();
    enemy_->Draw();

    DrawFormatString(0, 380, Util::Color::RED, "Scene: GAME");
    DrawFormatString(0, 0, Util::Color::WHITE, "[DEBUG]key-0で終了時間を10秒に変更。既に経過してる場合はGameScene終了");
    DrawFormatString(0, 20, Util::Color::WHITE, "time: %f", timer_.GetElapsedTime());
}
