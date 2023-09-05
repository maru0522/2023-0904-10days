#pragma once
#include "IEntity.h"

class Player final : public IEntity
{
public:
    // ’è”
    float kMoveSpeed_{3.f};

    // ŠÖ”
    Player(Stage* stagePtr) : IEntity(stagePtr) {}
    ~Player(void) override = default;

    void Update(void);
    void Draw(void);

private:
    // •Ï”
};

