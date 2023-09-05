#pragma once
#include "IEntity.h"

class Player final : public IEntity
{
public:
    // �萔
    float kMoveSpeed_{3.f};

    // �֐�
    Player(Stage* stagePtr) : IEntity(stagePtr) {}
    ~Player(void) override = default;

    void Update(void);
    void Draw(void);

private:
    // �ϐ�
};

