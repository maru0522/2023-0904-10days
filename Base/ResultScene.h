#pragma once
#include "IScene.h"
#include <memory>

class ResultScene :
    public IScene
{
public:
    uint32_t png_result_ = 0;       //リザルト画像
    uint32_t result_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;   //シーン遷移音

    // 関数
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};
};

