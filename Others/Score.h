#pragma once
#include <memory>

class Score
{
public:
	//初期化
	static void Init();
	//スコア追加
	static void Add(uint32_t score);
	//スコア描画
	static void Draw();

	//ハイスコアの更新(したか確認)
	static void HighScoreUpdate();

	//ハイスコア保存
	static void SaveScore();
	//ハイスコア読み込み
	static void LoadScore();

	static uint32_t nowScore_;		//現在のスコア
	static uint32_t highScore_;		//今までのハイスコア
};