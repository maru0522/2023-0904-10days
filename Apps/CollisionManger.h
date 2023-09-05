#pragma once
#include "IEntity.h"
#include <forward_list>

// 簡易的な衝突マネージャ
class CollisionManger final
{
public:
    // 関数
    static CollisionManger* GetInstance(void);

    void Register(IEntity* iEntityPtr) { flist_iEntityPtr_.push_front(iEntityPtr); }
    void UnRegister(IEntity* iEntityPtr) { flist_iEntityPtr_.remove(iEntityPtr); }
    void Update(void);

private:
    // 変数
    std::forward_list<IEntity*> flist_iEntityPtr_;

    // singleton
    CollisionManger(void) = default;
    ~CollisionManger(void) = default;
    CollisionManger(const CollisionManger&) = delete;
    CollisionManger& operator=(const CollisionManger&) = delete;
};

bool ColEntities(IEntity* e1, IEntity* e2);