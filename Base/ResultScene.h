#pragma once
#include "IScene.h"

class ResultScene :
    public IScene
{
public:
    // ä÷êî
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};
};

