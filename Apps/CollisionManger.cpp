#include "CollisionManger.h"
#include <cmath>
#include "IEntity.h"

CollisionManger* CollisionManger::GetInstance(void)
{
    static CollisionManger colM;
    return &colM;
}

void CollisionManger::Update(void)
{
    auto it1 = flist_iEntityPtr_.begin();
    for (; it1 != flist_iEntityPtr_.end(); ++it1)
    {
        // it1 の次
        auto it2 = it1;
        it2++;

        for (; it2 != flist_iEntityPtr_.end(); ++it2)
        {
            bool isHit = ColEntities(*it1, *it2);
            if (isHit)
            {
                (*it1)->SetOther(*it2);
                (*it2)->SetOther(*it1);

                (*it1)->OnCollision();
                (*it2)->OnCollision();
            }
        }
    }
}

bool ColEntities(IEntity* e1, IEntity* e2)
{
    using Shape = IEntity::Shape;

    // e1 = 円, e2 = 円
    if (e1->GetShape() == Shape::CIRCLE && e2->GetShape() == Shape::CIRCLE)
    {
        return (e1->GetPos().x - e2->GetPos().x) * (e1->GetPos().x - e2->GetPos().x) +
               (e1->GetPos().y - e2->GetPos().y) * (e1->GetPos().y - e2->GetPos().y) <=
               (e1->GetRad().x + e2->GetRad().x) * (e1->GetRad().x + e2->GetRad().x);
    }

    // e1 = 円, e2 = 正方形
    if (e1->GetShape() == Shape::CIRCLE && e2->GetShape() == Shape::SQUARE)
    {
        // 最近接点
        Vector2 nearest{};
        nearest.x = (std::max)(e2->GetPos().x - e2->GetRad().x, (std::min)(e1->GetPos().x, e2->GetPos().x + e2->GetRad().x));
        nearest.y = (std::max)(e2->GetPos().y - e2->GetRad().y, (std::min)(e1->GetPos().y, e2->GetPos().y + e2->GetRad().y));

        return (e1->GetPos().x - nearest.x) * (e1->GetPos().x - nearest.x) +
               (e1->GetPos().y - nearest.y) * (e1->GetPos().y - nearest.y) <= e1->GetRad().x * e1->GetRad().x;
    }

    // e1 = 正方形, e2 = 円
    if (e1->GetShape() == Shape::SQUARE && e2->GetShape() == Shape::CIRCLE)
    {
        // 最近接点
        Vector2 nearest{};
        nearest.x = (std::max)(e1->GetPos().x - e1->GetRad().x, (std::min)(e2->GetPos().x, e1->GetPos().x + e1->GetRad().x));
        nearest.y = (std::max)(e1->GetPos().y - e1->GetRad().y, (std::min)(e2->GetPos().y, e1->GetPos().y + e1->GetRad().y));

        return (e2->GetPos().x - nearest.x) * (e2->GetPos().x - nearest.x) +
               (e2->GetPos().y - nearest.y) * (e2->GetPos().y - nearest.y) <= e2->GetRad().x * e2->GetRad().x;
    }

    // e1 = 正方形, e2 = 正方形
    // めんどくさいからパス

    return false;
}
