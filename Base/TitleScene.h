#pragma once
#include "IScene.h"
#include <memory>

class TitleScene :
    public IScene
{
private:
    //タイトル画像ハンドル
    uint32_t titleImageHandle_ = 0;

public:
    // 関数
    void Initialize(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Finalize(void) override {};
};

