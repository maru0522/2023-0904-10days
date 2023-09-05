#pragma once
#include "Vector2.h"
#include <cstdint>
#include "Stage.h"
#include <string>
#include <functional>

class IEntity
{
public:
    // 定義
    enum class Shape
    {
        CIRCLE,
        SQUARE,
    };

    // 関数
    IEntity(Stage* stagePtr) : stagePtr_(stagePtr) {};
    virtual ~IEntity(void) = default;

    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;

    void OnCollision(void)
    {
        if (onCollision_) { onCollision_(); }
    }
protected:
    // 変数
    Stage* stagePtr_;

    Vector2 position_; // 中心点
    Vector2 rotation_; // 回転角(rad)
    Vector2 radius_;   // 半径(xy)※円形の場合、x値のみを参照する

    Shape shape_;
    std::string id_;
    IEntity* other_;
    std::function<void(void)> onCollision_;

public:
    // setter
    void SetPos(const Vector2& pos) { position_ = pos; }
    void SetRot(const Vector2& rot) { rotation_ = rot; }
    void SetRad(const Vector2& rad) { radius_ = rad; }
    void SetOther(IEntity* other) { other_ = other; }
    void SetOnCollision(const std::function<void(void)> callback_onCollision) { onCollision_ = callback_onCollision; }

    // getter
    const Vector2& GetPos(void) { return position_; }
    const Vector2& GetRot(void) { return rotation_; }
    const Vector2& GetRad(void) { return radius_; }
    const std::string& GetId(void) { return id_; }
    IEntity* GetOther(void) { return other_; }
    Shape GetShape(void) { return shape_; }
    const std::function<void(void)>& GetOnCollision(void) { return onCollision_; }
};

