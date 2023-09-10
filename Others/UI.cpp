#include "UI.h"
#include "DxLib.h"

std::vector<uint32_t> UI::ui_;
std::vector<Vector2> UI::pos_;

UI::UI()
{
	//ì«Ç›çûÇ›Å`
	ui_[(uint32_t)UIType::Abutton] = LoadGraph("Resources/texture/Abutton");
	ui_[(uint32_t)UIType::Bbutton] = LoadGraph("Resources/texture/Bbutton");
	ui_[(uint32_t)UIType::Rbutton] = LoadGraph("Resources/texture/Rbutton");
	ui_[(uint32_t)UIType::Lstick] = LoadGraph("Resources/texture/Lstick");
}

void UI::Init()
{
}

void UI::Update()
{
}

void UI::Draw(UIType uiType)
{
	//éwíËÇ≥ÇÍÇΩUIÇï`âÊ
	DrawGraph(
		(uint32_t)pos_[(uint32_t)uiType].x,
		(uint32_t)pos_[(uint32_t)uiType].y,
		ui_[(uint32_t)uiType], true);
}

void UI::SetPos(UIType uiType, const Vector2& pos)
{
	pos_[(uint32_t)uiType] = pos;
}