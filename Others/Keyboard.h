#pragma once
#include <DxLib.h>

class Keyboard
{
public:
    static void Update(void) {
        // �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
        // �ŐV�̃L�[�{�[�h�����擾
        GetHitKeyStateAll(keys_);
    }

    static char keys_[256];
    static char prekeys_[256];
};

