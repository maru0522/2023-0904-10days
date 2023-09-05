#include "SceneManager.h"

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
}

void SceneManager::Initialize(SceneFactory::Usage firstScene)
{
    // �ŏ��̃V�[���𐶐�
    currentScene_ = sceneFactory_->CreateScene(firstScene);
    currentScene_->Initialize();
}

void SceneManager::Update(void)
{
    // �X���[���[�V�����Ȃ�
    if (frameCount_slowMotion_ == 0)
    {
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
    else // �X���[���[�V��������
    {
        // �K��l�Ŋ������Ƃ��̗]�肪0�̎��̂݁A�e��Update()���񂷁B
        if (frameCount_slowMotion_ % kSlowFrameRatio_ == 0)
        {
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
        frameCount_slowMotion_++;
    }
}

void SceneManager::Draw(void)
{
    currentScene_->Draw();
}

void SceneManager::StartSlowMotion(void)
{
    frameCount_slowMotion_++;
}

void SceneManager::EndSlowMotion(void)
{
    frameCount_slowMotion_ = 0;
}
