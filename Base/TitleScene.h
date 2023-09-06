#pragma once
#include "IScene.h"
#include <memory>

class TitleScene :
    public IScene
{
public:
    // ä÷êî
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};

private:
    uint32_t titleImageHandle_ = 0;
};

