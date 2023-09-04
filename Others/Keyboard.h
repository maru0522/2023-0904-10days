#pragma once
#include <DxLib.h>
#include <cstdint>

class Keyboard final
{
public:
    static void Update(void) {
        // �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
        // �ŐV�̃L�[�{�[�h�����擾
        GetHitKeyStateAll(sKeys_);
    }

    // �����Ă�
    static bool IsDown(uint8_t key) { return sKeys_[key]; }
    // �������u��
    static bool IsTrigger(uint8_t key) { return !sKeysPre_[key] && sKeys_[key]; }
    // �������u��
    static bool IsReleased(uint8_t key) { return sKeysPre_[key] && !sKeys_[key]; }

    static char sKeys_[256];
    static char sKeysPre_[256];
};

using KEYS = Keyboard;

