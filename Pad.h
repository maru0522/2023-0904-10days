#pragma once

void CheckConnectPad(); // �p�b�h���ڑ�����Ă��邩�Ċm�F (���t���[���Ă΂Ȃ��悤��)
bool GetPadConnect(); // �p�b�h���ڑ�����Ă��邩��Ԃ�

bool PadDownA(); // A�{�^������ �� true
bool PadDownLorR(); // R�g���K�[���� �� true

bool PadTriggerA(); // A�{�^������(�u��) �� true
bool PadTriggerLorR(); // R�g���K�[����(�u��) �� true

bool PadLongA(); // A�{�^�������� �� true
bool PadLongLorR(); // R�g���K�[������ �� true

bool PadRight(); // �X�e�B�b�N�E�|�� �� true
bool PadLeft(); // �X�e�B�b�N���|�� �� true

int PadHorizontal(); // �E �� 1, �� �� -1, ����ȊO �� 0

void ControlUpdate(); // GetHitKeyStateAll(keys); �̉��ɏ����Ă�