#include "Pad.h"
#include "DxLib.h"

static enum Push
{
	push = 128, // 押下 (変えちゃダメ)
	down = 600, // 押し倒す量 (変えてもいいよ)
	down2 = 400,
};

static DINPUT_JOYSTATE pad{};
static DINPUT_JOYSTATE oldpad{};

static bool IsPadConnect = false;

// パッドが接続されているか再確認 (毎フレーム呼ばないように)
void CheckConnectPad()
{
	ReSetupJoypad();
	if (GetJoypadNum()) IsPadConnect = true;
	else IsPadConnect = false;
}

// パッドが接続されているかを返す
bool GetPadConnect()
{
	return IsPadConnect;
}

bool PadDownA() // Aボタン
{
	return pad.Buttons[0] == push;
}
bool PadDownLorR() // Rトリガー
{
	return pad.Z >= down2 || pad.Z <= -down2;
}

bool PadTriggerA() // Aボタン
{
	return pad.Buttons[0] == push && oldpad.Buttons[0] == 0;
}
bool PadTriggerLorR() // Rトリガー
{
	return	(pad.Z >= down2 && oldpad.Z <= down2) ||
		(pad.Z <= -down2 && oldpad.Z >= -down2);
}

bool PadLongA() // Aボタン (長押し)
{
	return pad.Buttons[0] == push && oldpad.Buttons[0] == push;
}
bool PadReleaseA()
{
    return pad.Buttons[0] == 0 && oldpad.Buttons[0] == push;
}
bool PadLongLorR() // Rトリガー (長押し)
{
	return	(pad.Z >= down2 && oldpad.Z >= down2) ||
		(pad.Z <= -down2 && oldpad.Z <= -down2);
}

bool PadDownB()
{
    return pad.Buttons[1] == push;
}

bool PadTriggerB()
{
    return pad.Buttons[1] == push && oldpad.Buttons[1] == 0;
}

bool PadLongB()
{
    return pad.Buttons[1] == push && oldpad.Buttons[1] == push;
}

bool PadDownRB(void)
{
    return pad.Buttons[5] == push;
}

bool PadTriggerRB(void)
{
    return pad.Buttons[5] == push && oldpad.Buttons[5] == 0;
}

bool PadLongLorRB(void)
{
    return pad.Buttons[5] == push && oldpad.Buttons[5] == push;
}

bool PadDownLB(void)
{
    return pad.Buttons[4] == push;
}

bool PadTriggerLB(void)
{
    return pad.Buttons[4] == push && oldpad.Buttons[4] == 0;
}

bool PadLongLorLB(void)
{
    return pad.Buttons[4] == push && oldpad.Buttons[4] == push;
}

bool PadDownMenu(void)
{
    return pad.Buttons[7] == push;
}

bool PadTriggerMenu(void)
{
    return pad.Buttons[7] == push && oldpad.Buttons[7] == 0;
}

bool PadRight() // スティック右倒す
{
	return pad.X >= down && oldpad.X >= down;
}
bool PadLeft() // スティック左倒す
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

void DrawPad(void)
{
    DrawFormatString(300, 100, 0xffffff, "button[0]:%d", pad.Buttons[0]);
    DrawFormatString(300, 120, 0xffffff, "button[1]:%d", pad.Buttons[1]);
    DrawFormatString(300, 140, 0xffffff, "button[2]:%d", pad.Buttons[2]);
    DrawFormatString(300, 160, 0xffffff, "button[3]:%d", pad.Buttons[3]);
    DrawFormatString(300, 180, 0xffffff, "button[4]:%d", pad.Buttons[4]);
    DrawFormatString(300, 200, 0xffffff, "button[5]:%d", pad.Buttons[5]);
    DrawFormatString(300, 220, 0xffffff, "button[6]:%d", pad.Buttons[6]);
    DrawFormatString(300, 240, 0xffffff, "button[7]:%d", pad.Buttons[7]);
    DrawFormatString(300, 260, 0xffffff, "button[8]:%d", pad.Buttons[8]);
    DrawFormatString(300, 280, 0xffffff, "button[9]:%d", pad.Buttons[9]);
    DrawFormatString(300, 300, 0xffffff, "button[10]:%d", pad.Buttons[10]);
    DrawFormatString(300, 320, 0xffffff, "button[11]:%d", pad.Buttons[11]);
    DrawFormatString(300, 340, 0xffffff, "button[12]:%d", pad.Buttons[12]);
    DrawFormatString(300, 360, 0xffffff, "button[13]:%d", pad.Buttons[13]);
    DrawFormatString(300, 380, 0xffffff, "button[14]:%d", pad.Buttons[14]);
    DrawFormatString(300, 400, 0xffffff, "button[15]:%d", pad.Buttons[15]);
    DrawFormatString(300, 420, 0xffffff, "button[16]:%d", pad.Buttons[16]);
    DrawFormatString(300, 440, 0xffffff, "button[17]:%d", pad.Buttons[17]);
    DrawFormatString(300, 460, 0xffffff, "button[18]:%d", pad.Buttons[18]);
    DrawFormatString(300, 480, 0xffffff, "button[19]:%d", pad.Buttons[19]);
    DrawFormatString(300, 500, 0xffffff, "button[20]:%d", pad.Buttons[20]);
    DrawFormatString(300, 520, 0xffffff, "button[21]:%d", pad.Buttons[21]);
    DrawFormatString(300, 540, 0xffffff, "button[22]:%d", pad.Buttons[22]);
    DrawFormatString(300, 560, 0xffffff, "button[23]:%d", pad.Buttons[23]);
    DrawFormatString(300, 580, 0xffffff, "button[24]:%d", pad.Buttons[24]);
    DrawFormatString(300, 600, 0xffffff, "button[25]:%d", pad.Buttons[25]);
    DrawFormatString(300, 620, 0xffffff, "button[26]:%d", pad.Buttons[26]);
    DrawFormatString(300, 640, 0xffffff, "button[27]:%d", pad.Buttons[27]);
    DrawFormatString(300, 660, 0xffffff, "button[28]:%d", pad.Buttons[28]);
    DrawFormatString(300, 680, 0xffffff, "button[29]:%d", pad.Buttons[29]);
    DrawFormatString(300, 700, 0xffffff, "button[30]:%d", pad.Buttons[30]);
    DrawFormatString(300, 720, 0xffffff, "button[31]:%d", pad.Buttons[31]);
}
