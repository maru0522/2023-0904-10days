#include "Score.h"
#include "DxLib.h"
#include <fstream>
#include <string>
#include <sstream>

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
	DrawFormatString(700, 20, 0xffffff, "HIGHSCORE:%d", highScore_);
}

void Score::HighScoreUpdate()
{
	//ハイスコアの更新あったら塗り替える
	if (highScore_ < nowScore_)
	{
		highScore_ = nowScore_;
	}
}

void Score::SaveScore()
{
	//ファイル出力処理
	std::ofstream writing_file;

	std::string filename = "";
	filename = "./Resources/data/highScore.txt";

	writing_file.open(filename, std::ios::out);

	uint32_t h = highScore_;
	writing_file << h << std::endl;

	writing_file.close();
}

void Score::LoadScore()
{
	//ファイル入力処理
	std::ifstream reading_file;

	reading_file.open("./Resources/data/highScore.txt");
	////ファイルオープン失敗をチェック
	//if (reading_file.fail())
	//{
	//	assert(0);
	//}

	std::string line;

	while (getline(reading_file, line))
	{
		//文字列をintにして代入
		highScore_ = (uint32_t)atoi(line.c_str());
	}
}
