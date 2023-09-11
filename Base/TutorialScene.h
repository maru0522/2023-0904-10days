#pragma once
#include "IScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Util.h"
#include "Stage.h"
#include "CollisionManger.h"

class TutorialScene :
    public IScene
{
public:
    // ��`
    enum Destination
    {
        RETRY,
        TITLE,
    };

    // �萔
    const int32_t kMaxFrameSkipTutorial_{ 210 }; // pad-B���t���[���������Ń`���[�g���A�����X�L�b�v���邩

    // �֐�
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

    // �ϐ�
    bool isMenu_; // ���j���[�p
    int32_t destination_; // ���j���[���̎��V�[���̑J�ڐ�
    int32_t frameCount_skipTutorial_; // �`���[�g���A�����X�L�b�v����p�̃t���[���J�E���^

    uint32_t game_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //�V�[���J�ډ�
    int32_t png_retry_ = LoadGraph("Resources/texture/retry.png");
    int32_t png_2title_ = LoadGraph("Resources/texture/toTitle.png");
};

