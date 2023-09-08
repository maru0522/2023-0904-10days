#include "GameScene.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "Keyboard.h"
#include "Pad.h"
#include "MathUtil.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Score.h"

void GameScene::Initialize(void)
{
	//BGM読み込み、再生
	game_BGM_ = LoadSoundMem("Resources/sound/play_BGM.mp3");
	PlaySoundMem(game_BGM_, DX_PLAYTYPE_LOOP);
	//SE読み込み
	sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");

	// (0,0) ~ (1280,720) よりちょい内側
	stage_->Initialize({ 10,10 }, { 1270,710 });

	player_->SetPos({ 300,300 });
	player_->SetRot(0);
	player_->SetRad({ 10,0 });

	EnemyManager::GetInstance().Initialize(player_.get(), stage_.get());

	//timer_.Start(kMaxGameTimer_);
	timer_.Start(1000000000);

	ParticleManager::GetInstance()->Init();

	Score::Init();
    Update();
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
		Score::HighScoreUpdate();

		PlaySoundMem(sceneChange_SE_, DX_PLAYTYPE_NORMAL);
		//BGMストップ
		StopSoundMem(game_BGM_);
		SceneManager::GetInstance()->RequestChangeScene(SceneFactory::Usage::RESULT);
	}

	ParticleManager::GetInstance()->Update();
}

void GameScene::Draw(void)
{
	stage_->Draw();

	player_->Draw();
	EnemyManager::GetInstance().Draw();

	ParticleManager::GetInstance()->Draw();

	DrawFormatString(0, 380, Util::Color::RED, "Scene: GAME");
	DrawFormatString(0, 0, Util::Color::WHITE, "[DEBUG]key-0で終了時間を10秒に変更。既に経過してる場合はGameScene終了");
	DrawFormatString(0, 20, Util::Color::WHITE, "time: %f", timer_.GetElapsedTime());
	//DrawPad();

	Score::Draw();
}
