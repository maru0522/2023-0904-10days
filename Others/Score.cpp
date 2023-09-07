#include "Score.h"
#include "DxLib.h"

uint32_t Score::nowScore_ = 0;
uint32_t Score::highScore_ = 0;

void Score::Init()
{
	nowScore_ = 0;
	//ハイスコアの読み込み//
}

void Score::Add(uint32_t score)
{
	nowScore_ += score;
}

void Score::Draw()
{
	DrawFormatString(700, 0, 0xffffff, "SCORE:%d", nowScore_);
}

void Score::HighScoreUpdate()
{
	//ハイスコアの更新あったら塗り替える
	if (highScore_ < nowScore_)
	{
		highScore_ = nowScore_;
	}
}