#include "Stage.h"
#include <DxLib.h>
#include "Util.h"
#include <cstdint>

void Stage::Initialize(const Vector2& lt, const Vector2& rb)
{
    leftTop_ = lt;
    RightBottom_ = rb;

    //�摜�ǂݍ���
    png_Stage_ = LoadGraph("Resources/texture/stage.png");
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
    //�X�e�[�W�摜�`��
    DrawRotaGraph(640, 360,1.1f,0, png_Stage_, true);
    // �s�������͈͕\��
    DrawBox((int32_t)leftTop_.x, (int32_t)leftTop_.y, (int32_t)RightBottom_.x, (int32_t)RightBottom_.y, Util::Color::GREEN, false);
}
