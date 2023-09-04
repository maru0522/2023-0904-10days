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
bool PadLongLorR() // Rトリガー (長押し)
{
	return	(pad.Z >= down2 && oldpad.Z >= down2) ||
		(pad.Z <= -down2 && oldpad.Z <= -down2);
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