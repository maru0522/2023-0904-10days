#include "TitleScene.h"
#include <DxLib.h>
#include "Util.h"

void TitleScene::Initialize(void)
{
}

void TitleScene::Update(void)
{
}

void TitleScene::Draw(void)
{
    DrawFormatString(100, 100, Util::Color::RED, "Scene: TITLE");
    DrawFormatString(100, 120, Util::Color::RED, "press pad-A or Space");
}
