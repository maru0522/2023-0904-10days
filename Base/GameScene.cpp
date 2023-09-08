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
	//BGM�ǂݍ��݁A�Đ�
	game_BGM_ = LoadSoundMem("Resources/sound/play_BGM.mp3");
	PlaySoundMem(game_BGM_, DX_PLAYTYPE_LOOP);
	//SE�ǂݍ���
	sceneChange_SE_ = LoadSoundMem("Resources/sound/sceneChange_SE.mp3");

	// (0,0) ~ (1280,720) ��肿�傢����
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
		//BGM�X�g�b�v
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
	DrawFormatString(0, 0, Util::Color::WHITE, "[DEBUG]key-0�ŏI�����Ԃ�10�b�ɕύX�B���Ɍo�߂��Ă�ꍇ��GameScene�I��");
	DrawFormatString(0, 20, Util::Color::WHITE, "time: %f", timer_.GetElapsedTime());
	//DrawPad();

	Score::Draw();
}
