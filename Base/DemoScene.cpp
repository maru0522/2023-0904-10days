#include "DemoScene.h"
#include <DxLib.h>
#include "Util.h"

void DemoScene::Initialize(void)
{
    player_->SetPos({ 0,0 });
    player_->SetRot({ 0,0 });
    player_->SetRad({ 10,0 });
}

void DemoScene::Update(void)
{
    player_->Update();
}

void DemoScene::Draw(void)
{
    player_->Draw();
    DrawFormatString(100, 100, Util::Color::RED, "Scene: DEMO");
}
