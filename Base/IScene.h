#pragma once
class IScene
{
public:
    // ä÷êî
    virtual ~IScene(void) = default;

    virtual void Initialize(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    virtual void Finalize(void) = 0;
};

