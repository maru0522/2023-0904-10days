#include "UI.h"
#include "DxLib.h"

std::vector<uint32_t> UI::ui_((uint32_t)UIType::Max);
std::vector<Vector2> UI::pos_((uint32_t)UIType::Max);
std::vector<float> UI::size_((uint32_t)UIType::Max);

UI::UI()
{
}

void UI::AllLoad()
{
	//�ǂݍ��݁`
	ui_[(uint32_t)UIType::Abutton] = LoadGraph("Resources/texture/Abutton.png");
	ui_[(uint32_t)UIType::Bbutton] = LoadGraph("Resources/texture/Bbutton.png");
	ui_[(uint32_t)UIType::Rbutton] = LoadGraph("Resources/texture/Rbutton.png");
	ui_[(uint32_t)UIType::Lstick] = LoadGraph("Resources/texture/Lstick.png");
	ui_[(uint32_t)UIType::Move] = LoadGraph("Resources/texture/Move.png");
	ui_[(uint32_t)UIType::Attack] = LoadGraph("Resources/texture/Attack.png");
	ui_[(uint32_t)UIType::Skewer] = LoadGraph("Resources/texture/Skewer.png");

	//���W�ƃT�C�Y������
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		pos_[i] = {0.f,0.f};
		size_[i] = 0;
	}
}

void UI::Init()
{
}

void UI::Update()
{
}

void UI::Draw(UIType uiType)
{
	//�w�肳�ꂽUI��`��
	DrawRotaGraph(
		(uint32_t)pos_[(uint32_t)uiType].x,
		(uint32_t)pos_[(uint32_t)uiType].y,
		(double)size_[(uint32_t)uiType],
		0.0,
		ui_[(uint32_t)uiType], true);
}

void UI::SetPos(UIType uiType, const Vector2& pos)
{
	pos_[(uint32_t)uiType] = pos;
}

void UI::SetSize(UIType uiType, float size)
{
	size_[(uint32_t)uiType] = size;
}