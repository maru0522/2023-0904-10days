#pragma once
#include "IScene.h"
#include <memory>

class ResultScene :
    public IScene
{
public:
    uint32_t png_result_ = 0;

    // ä÷êî
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};
};

