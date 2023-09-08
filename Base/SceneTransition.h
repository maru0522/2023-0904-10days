#pragma once
#include <DxLib.h>
#include "Vector2.h"
#include <cstdint>

class SceneTransition
{
public:
    // �萔
    static const int32_t kMaxFrameRolled_{ 50 }; // ���t���[���|���Ĉړ����Ă��邩
    static const int32_t kMaxFrameRolledTrans_{ 10 }; // ���t���[���|���ē����ɂȂ邩
    static const int32_t kMaxFrameTBBelt_{ 10 }; // ���t���[���|���邩
    static const int32_t kTotalFrame_{kMaxFrameRolled_ + kMaxFrameRolledTrans_ + kMaxFrameTBBelt_}; // ��L�̍��v�l

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
    int32_t alpha_; // �摜�̃A���t�@�l

    Vector2 pos_rolledScroll_; // �����̊�����Ă镔���̊G�̍��W
    Vector2 pos_unrollScroll_; // �����̂܂�������ĂȂ������̊G�̍��W
    Vector2 pos_TBBeltBG_; // �������J�ڂ�����̏�̑ѕ����̍��W

    int32_t frameCount_rolled_; // �����ׂ̃J�E���^
    int32_t frameCount_transBG_; // �^�񒆂𓧉߂���J�E���^
    int32_t frameCount_moveTBBG_; // �㉺�̑т��J����ׂ̃J�E���^

    // resorce
    int32_t png_rolledScroll_ = LoadGraph("Resources/texture/makimono.png");
    int32_t png_unrollScroll_ = LoadGraph("Resources/texture/wasi_back2.png");
    int32_t png_topBeltBG_ = LoadGraph("Resources/texture/wasi_frame_up2.png");
    int32_t png_bottomBeltBG_ = LoadGraph("Resources/texture/wasi_frame_bottom2.png");
};
