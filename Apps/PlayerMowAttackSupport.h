#pragma once
#include "IEntity.h"
#include "CollisionManger.h"
#include <DxLib.h>

class PlayerMowAttackSupport : // �n���߂��Ď����ł����΂��o��N���X
    public IEntity
{
public:
    // �萔
    const Vector2 kRadius_{ 40,40 }; // �U���͈� { mow�̓��{,1/2�{ }

    // �֐�
    PlayerMowAttackSupport(CollisionManger* colMPtr);
    ~PlayerMowAttackSupport(void) override;

    void Update(void) override {};
    void Draw(void) override;

private:
    // �ϐ�
    CollisionManger* colMPtr_;
};

