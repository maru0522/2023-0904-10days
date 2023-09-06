#pragma once
#include "IScene.h"
#include <memory>

class TitleScene :
    public IScene
{
private:
    //�^�C�g���摜�n���h��
    uint32_t titleImageHandle_ = 0;

public:
    // �֐�
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};
};

