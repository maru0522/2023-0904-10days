#include "DemoScene.h"
#include <DxLib.h>
#include "Util.h"

using namespace Util;

void DemoScene::Initialize(void)
{
    //player_->SetPos({ 0,0 });
    //player_->SetRot({ 0,0 });
    //player_->SetRad({ 10,0 });

    //enemy_->SetPos({ 150,150 });
    //enemy_->SetRot({ 0,0 });
    //enemy_->SetRad({ 10,0 });
}

void DemoScene::Update(void)
{
    //player_->Update();
    //enemy_->Update();
}

void DemoScene::Draw(void)
{
    //player_->Draw();
    //enemy_->Draw();

#ifdef _DEBUG
    DrawFormatString(0, 380, Util::Color::RED, "Scene: DEMO");
#endif // DEBUG
}
