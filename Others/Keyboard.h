#pragma once
#include <DxLib.h>
#include <cstdint>

class Keyboard final
{
public:
    static void Update(void) {
        // 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
        // 最新のキーボード情報を取得
        GetHitKeyStateAll(sKeys_);
    }

    // 押してる
    static bool IsDown(uint8_t key) { return sKeys_[key]; }
    // 押した瞬間
    static bool IsTrigger(uint8_t key) { return !sKeysPre_[key] && sKeys_[key]; }
    // 離した瞬間
    static bool IsReleased(uint8_t key) { return sKeysPre_[key] && !sKeys_[key]; }

    static char sKeys_[256];
    static char sKeysPre_[256];
};

using KEYS = Keyboard;

