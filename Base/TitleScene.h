#pragma once
#include "IScene.h"

class TitleScene :
    public IScene
{
public:
    // �֐�
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};
};

