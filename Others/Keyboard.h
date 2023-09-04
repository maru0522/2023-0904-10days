#pragma once
#include <DxLib.h>

class Keyboard
{
public:
    static void Update(void) {
        // 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
        // 最新のキーボード情報を取得
        GetHitKeyStateAll(keys_);
    }

    static char keys_[256];
    static char prekeys_[256];
};

