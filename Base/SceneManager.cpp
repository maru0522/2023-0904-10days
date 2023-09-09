#include "SceneManager.h"
#include <DxLib.h>
#include "Keyboard.h"

SceneManager::~SceneManager(void)
{
    if (currentScene_) {
        currentScene_->Finalize();
        currentScene_.reset();
    }
}

SceneManager* SceneManager::GetInstance(void)
{
    static SceneManager sceneM;
    return &sceneM;
}

void SceneManager::RequestChangeScene(SceneFactory::Usage nextScene, int32_t waitFrame)
{
    nextScene_ = std::move(sceneFactory_->CreateScene(nextScene));
    waitFrame_ = waitFrame;

    // �X���[���[�V�������̏ꍇ����
    if (frameCount_slowMotion_) EndSlowMotion();
    // �J�ڋN��
    transition_.Start();
    frameCount_debug_ = 0;
}

void SceneManager::Initialize(SceneFactory::Usage firstScene)
{
    // �ŏ��̃V�[���𐶐�
    currentScene_ = sceneFactory_->CreateScene(firstScene);
    currentScene_->Initialize();
}

void SceneManager::Update(void)
{
    frameCount_debug_++;
    // �X���[���[�V�����Ȃ�
    if (frameCount_slowMotion_ == 0)
    {
        CollisionManger::GetInstance()->Update();
        // ���V�[���̗\�񂪂���
        if (nextScene_) {
            // �ҋ@�t���[�����K��l�ȉ��̎��i����̏ꍇ�A��ʂ����܂�����A�V�[����J�ڂ���j
            if (waitFrame_ <= SceneTransition::kTotalFrame_ - SceneTransition::kMaxFrameRolled_)
            {
                // ���݃V�[����nullptr�ł͂Ȃ�
                if (currentScene_) {
                    currentScene_->Finalize();
                    currentScene_.reset();
                }

                // �V�[���ڍs
                currentScene_ = std::move(nextScene_); // �Ǘ������ڏ�
                nextScene_.reset();                    // ���V�[����nullptr�ɂ���
                currentScene_->Initialize();           // ���݃V�[����������
            }
        }
        // ���V�[���̗\�񂪂Ȃ� && �V�[���J�ڑҋ@�t���[�����Ȃ�
        else if (waitFrame_ == 0)
        {
            // ���݃V�[��Update()
            currentScene_->Update();
        }

        if (KEYS::IsTrigger(KEY_INPUT_P))
        {
            transition_.Start();
        }
        transition_.Update();

        // �ҋ@�t���[��������������
        waitFrame_--;
        // �ҋ@�t���[����0�ȉ��ɂȂ�Ȃ�
        waitFrame_ = (std::max)(waitFrame_, 0);
    }
    else // �X���[���[�V��������
    {
        // �K��l�Ŋ������Ƃ��̗]�肪0�̎��̂݁A�e��Update()���񂷁B
        if (frameCount_slowMotion_ % kSlowFrameRatio_ == 0)
        {
            frameCount_slowMotion_++;
            CollisionManger::GetInstance()->Update();
            // ���V�[���̗\�񂪂���
            if (nextScene_) {
                // �ҋ@�t���[���w�肪�Ȃ� && ���݃V�[����nullptr�ł͂Ȃ�
                if (waitFrame_ == 0 && currentScene_) {
                    currentScene_->Finalize();
                    currentScene_.reset();
                }

                // �V�[���ڍs
                currentScene_ = std::move(nextScene_); // �Ǘ������ڏ�
                nextScene_.reset();                    // ���V�[����nullptr�ɂ���
                currentScene_->Initialize();           // ���݃V�[����������
            }

            // ���݃V�[��Update()
            currentScene_->Update();


            // �ҋ@�t���[��������������
            waitFrame_--;
            // �ҋ@�t���[����0�ȉ��ɂȂ�Ȃ�
            waitFrame_ = (std::max)(waitFrame_, 0);
        }
        else
        {
            frameCount_slowMotion_++;
        }

        // frameCount_slowMotion_��2�ɕ����āA�K���C���N�������g�����悤�ɂ��Ă���̂́Aif���̌�ɂ��Ă���ꍇ�A�������[�v���Ă��܂��\�������邽�߁B
        // ����̓I�ɂ́Aif������Update()����EndSlowMotion()���Ă΂ꂽ��ɁAif����̃C���N�������g�����s����AframeCount_slowMotion_��
        // ���0�ɂȂ炸�������[�v�����ԁB
    }
}

void SceneManager::Draw(void)
{
    
    currentScene_->Draw();
    transition_.Draw();
    if (frameCount_slowMotion_)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawBox(0, 0, 1280, 720, Util::Color::BLACK, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
    }
    DrawFormatString(0, 100, Util::Color::WHITE, frameCount_slowMotion_ == 0 ? "no slow" : "slow");
    DrawFormatString(0, 120, Util::Color::WHITE, "slow: %d", frameCount_slowMotion_);
    DrawFormatString(0, 160, Util::Color::GREEN, "frameCount: %d", frameCount_debug_);
}

void SceneManager::StartSlowMotion(void)
{
    frameCount_slowMotion_++;
}

void SceneManager::EndSlowMotion(void)
{
    frameCount_slowMotion_ = 0;
}
