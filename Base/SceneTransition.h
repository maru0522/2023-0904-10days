#pragma once
#include <DxLib.h>
#include "Vector2.h"
#include <cstdint>

class SceneTransition
{
public:
    // 定数
    const int32_t kMaxFrameRolled_{ 60 }; // 何フレーム掛けて移動してくるか
    const int32_t kMaxFrameRolledTrans_{ 60 }; // 何フレーム掛けて透明になるか
    const int32_t kMaxFrameTBBelt_{ 20 }; // 何フレーム掛けるか

    // 関数
    SceneTransition(void) { Reset(); }
    ~SceneTransition(void) = default;

    void Reset(void);
    void Start(void);
    void End(void);
    void Update(void);
    void Draw(void);

private:
    // 変数
    bool isExe_;
    int32_t alpha_; // 画像のアルファ値

    Vector2 pos_rolledScroll_; // 巻物の巻かれてる部分の絵の座標
    Vector2 pos_unrollScroll_; // 巻物のまだ巻かれてない部分の絵の座標
    Vector2 pos_TBBeltBG_; // 巻物が遷移した先の上の帯部分の座標

    int32_t frameCount_rolled_;
    int32_t frameCount_transBG_;

    // resorce
    int32_t png_rolledScroll_ = LoadGraph("Resources/texture/makimono.png");
    int32_t png_unrollScroll_ = LoadGraph("Resources/texture/wasi_back.png");
    int32_t png_TBBeltBG_ = LoadGraph("Resources/texture/wasi_frame.png");
};
