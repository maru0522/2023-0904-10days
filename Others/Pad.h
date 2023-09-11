#pragma once
#include "Vector2.h"

void CheckConnectPad(); // �p�b�h���ڑ�����Ă��邩�Ċm�F (���t���[���Ă΂Ȃ��悤��)
bool GetPadConnect(); // �p�b�h���ڑ�����Ă��邩��Ԃ�

bool PadDownA(); // A�{�^������ �� true
bool PadTriggerA(); // A�{�^������(�u��) �� true
bool PadLongA(); // A�{�^�������� �� true

bool PadDownLorR(); // R�g���K�[���� �� true
bool PadTriggerLorR(); // R�g���K�[����(�u��) �� true
bool PadLongLorR(); // R�g���K�[������ �� true

bool PadDownRB(void);
bool PadTriggerRB(void);
bool PadLongLorRB(void);

bool PadDownLB(void);
bool PadTriggerLB(void);
bool PadLongLorLB(void);

bool PadDownMenu(void);
bool PadTriggerMenu(void);

bool PadRight(); // �X�e�B�b�N�E�|�� �� true
bool PadLeft(); // �X�e�B�b�N���|�� �� true

Vector2 PadStick(); // �X�e�B�b�N�̒l�擾(-1.0f~1.0f)

void ControlUpdate(); // GetHitKeyStateAll(keys); �̉��ɏ����Ă�

void DrawPad(void);