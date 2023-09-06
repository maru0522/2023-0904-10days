#pragma once
#include "IScene.h"
#include <memory>

class TitleScene :
    public IScene
{
private:
    //タイトル画像ハンドル
    uint32_t titleImageHandle_ = 0;
    uint32_t title_BGM_ = 0;
    uint32_t sceneChange_SE_ = 0;  //シーン遷移音
    uint32_t decision_SE_ = 0;  //決定音

public:
    // 関数
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};
};

