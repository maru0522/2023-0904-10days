#pragma once
#include <DxLib.h>
#include "Vector2.h"
#include <cstdint>

class SceneTransition
{
public:
    // �萔
    const int32_t kMaxFrameRolled_{ 50 }; // ���t���[���|���邩
    const int32_t kMaxFrameTBBelt_{ 20 }; // ���t���[���|���邩

    // �֐�
    SceneTransition(void) { Reset(); }
    ~SceneTransition(void) = default;

    void Reset(void);
    void Start(void);
    void End(void);
    void Update(void);
    void Draw(void);

private:
    // �ϐ�
    bool isExe_;

    Vector2 pos_rolledScroll_; // �����̊�����Ă镔���̊G�̍��W
    Vector2 pos_unrollScroll_; // �����̂܂�������ĂȂ������̊G�̍��W
    Vector2 pos_TBBeltBG_; // �������J�ڂ�����̏�̑ѕ����̍��W

    int32_t frameCount_rolled_;

    // resorce
    int32_t png_rolledScroll_ = LoadGraph("Resources/texture/makimono.png");
    int32_t png_unrollScroll_ = LoadGraph("Resources/texture/wasi_back.png");
    int32_t png_TBBeltBG_ = LoadGraph("Resources/texture/wasi_frame.png");
};
