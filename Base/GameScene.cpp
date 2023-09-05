#include "GameScene.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "Keyboard.h"
#include "Pad.h"
#include "MathUtil.h"
#include "EnemyManager.h"

void GameScene::Initialize(void)
{
    // (0,0) ~ (1280,720) よりちょい内側
    stage_->Initialize({ 10,10 }, { 1270,710 });

    player_->SetPos({ 300,300 });
    player_->SetRot({ 0,0 });
    player_->SetRad({ 10,0 });

    float x = Math::Function::Random<float>(100, 1100);
    float y = Math::Function::Random<float>(100, 600);
    std::unique_ptr<Enemy>enemy1 = std::make_unique<Enemy>(CollisionManger::GetInstance(), player_.get(), stage_.get());
    std::unique_ptr<Enemy>enemy2 = std::make_unique<Enemy>(CollisionManger::GetInstance(), player_.get(), stage_.get());
    std::unique_ptr<Enemy>enemy3 = std::make_unique<Enemy>(CollisionManger::GetInstance(), player_.get(), stage_.get());
    std::unique_ptr<Enemy>enemy4 = std::make_unique<Enemy>(CollisionManger::GetInstance(), player_.get(), stage_.get());
    std::unique_ptr<Enemy>enemy5 = std::make_unique<Enemy>(CollisionManger::GetInstance(), player_.get(), stage_.get());
    enemy1->SetPos({ x,y });
    enemy1->SetRot({ 0,0 });
    enemy1->SetRad({ 10,0 });

    x = Math::Function::Random<float>(100, 1100);
    y = Math::Function::Random<float>(100, 600);
    enemy2->SetPos({ x,y });
    enemy2->SetRot({ 0,0 });
    enemy2->SetRad({ 10,0 });

    x = Math::Function::Random<float>(100, 1100);
    y = Math::Function::Random<float>(100, 600);
    enemy3->SetPos({ x,y });
    enemy3->SetRot({ 0,0 });
    enemy3->SetRad({ 10,0 });

    x = Math::Function::Random<float>(100, 1100);
    y = Math::Function::Random<float>(100, 600);
    enemy4->SetPos({ x,y });
    enemy4->SetRot({ 0,0 });
    enemy4->SetRad({ 10,0 });

    x = Math::Function::Random<float>(100, 1100);
    y = Math::Function::Random<float>(100, 600);
    enemy5->SetPos({ x,y });
    enemy5->SetRot({ 0,0 });
    enemy5->SetRad({ 10,0 });

    EnemyManager::GetInstance().Initialize(player_.get());

    EnemyManager::GetInstance().AddEnemy(std::move(enemy1));
    EnemyManager::GetInstance().AddEnemy(std::move(enemy2));
    EnemyManager::GetInstance().AddEnemy(std::move(enemy3));
    EnemyManager::GetInstance().AddEnemy(std::move(enemy4));
    EnemyManager::GetInstance().AddEnemy(std::move(enemy5));

    timer_.Start(kMaxGameTimer_);
}

void GameScene::Update(void)
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
        SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::RESULT);
    }
}

void GameScene::Draw(void)
{
    stage_->Draw();

    player_->Draw();
    EnemyManager::GetInstance().Draw();

    DrawFormatString(0, 380, Util::Color::RED, "Scene: GAME");
    DrawFormatString(0, 0, Util::Color::WHITE, "[DEBUG]key-0で終了時間を10秒に変更。既に経過してる場合はGameScene終了");
    DrawFormatString(0, 20, Util::Color::WHITE, "time: %f", timer_.GetElapsedTime());
}
