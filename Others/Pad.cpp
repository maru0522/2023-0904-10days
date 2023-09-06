#include "Pad.h"
#include "DxLib.h"

static enum Push
{
	push = 128, // ���� (�ς�����_��)
	down = 600, // �����|���� (�ς��Ă�������)
	down2 = 400,
};

static DINPUT_JOYSTATE pad{};
static DINPUT_JOYSTATE oldpad{};

static bool IsPadConnect = false;

// �p�b�h���ڑ�����Ă��邩�Ċm�F (���t���[���Ă΂Ȃ��悤��)
void CheckConnectPad()
{
	ReSetupJoypad();
	if (GetJoypadNum()) IsPadConnect = true;
	else IsPadConnect = false;
}

// �p�b�h���ڑ�����Ă��邩��Ԃ�
bool GetPadConnect()
{
	return IsPadConnect;
}

bool PadDownA() // A�{�^��
{
	return pad.Buttons[0] == push;
}
bool PadDownLorR() // R�g���K�[
{
	return pad.Z >= down2 || pad.Z <= -down2;
}

bool PadTriggerA() // A�{�^��
{
	return pad.Buttons[0] == push && oldpad.Buttons[0] == 0;
}
bool PadTriggerLorR() // R�g���K�[
{
	return	(pad.Z >= down2 && oldpad.Z <= down2) ||
		(pad.Z <= -down2 && oldpad.Z >= -down2);
}

bool PadLongA() // A�{�^�� (������)
{
	return pad.Buttons[0] == push && oldpad.Buttons[0] == push;
}
bool PadLongLorR() // R�g���K�[ (������)
{
	return	(pad.Z >= down2 && oldpad.Z >= down2) ||
		(pad.Z <= -down2 && oldpad.Z <= -down2);
}

bool PadDownRB(void)
{
    return pad.Buttons[9] == push;
}

bool PadTriggerRB(void)
{
    return pad.Buttons[9] == push && oldpad.Buttons[9] == 0;
}

bool PadLongLorRB(void)
{
    return pad.Buttons[9] == push && oldpad.Buttons[9] == push;
}

bool PadDownLB(void)
{
    return pad.Buttons[8] == push;
}

bool PadTriggerLB(void)
{
    return pad.Buttons[8] == push && oldpad.Buttons[8] == 0;
}

bool PadLongLorLB(void)
{
    return pad.Buttons[8] == push && oldpad.Buttons[8] == push;
}

bool PadRight() // �X�e�B�b�N�E�|��
{
	return pad.X >= down && oldpad.X >= down;
}
bool PadLeft() // �X�e�B�b�N���|��
{
	return pad.X <= -down && oldpad.X <= -down;
}

Vector2 PadStick()
{
	float x = (float)pad.X / 1000.f;
	float y = (float)pad.Y / 1000.f;
	return Vector2(x,y);
}

void ControlUpdate()
{
	oldpad = pad;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &pad);
}