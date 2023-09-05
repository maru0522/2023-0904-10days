#pragma once
#include "IEntity.h"

class Player final : public IEntity
{
public:
    // 定数
    float kMoveSpeed_{3.f};

    // 関数
    Player(Stage* stagePtr) : IEntity(stagePtr) {}
    ~Player(void) override = default;

    void Update(void);
    void Draw(void);

private:
    // 変数
};

