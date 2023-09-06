#pragma once
#include "IScene.h"
#include <memory>

class TitleScene :
    public IScene
{
private:
    //�^�C�g���摜�n���h��
    uint32_t titleImageHandle_ = 0;
    uint32_t title_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;  //�V�[���J�ډ�
    uint32_t decision_SE_ = 0;  //���艹

public:
    // �֐�
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};
};

