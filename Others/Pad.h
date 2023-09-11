#pragma once
#include "Vector2.h"

void CheckConnectPad(); // パッドが接続されているか再確認 (毎フレーム呼ばないように)
bool GetPadConnect(); // パッドが接続されているかを返す

bool PadDownA(); // Aボタン押す → true
bool PadTriggerA(); // Aボタン押す(瞬間) → true
bool PadLongA(); // Aボタン長押し → true

bool PadDownLorR(); // Rトリガー押す → true
bool PadTriggerLorR(); // Rトリガー押す(瞬間) → true
bool PadLongLorR(); // Rトリガー長押し → true

bool PadDownRB(void);
bool PadTriggerRB(void);
bool PadLongLorRB(void);

bool PadDownLB(void);
bool PadTriggerLB(void);
bool PadLongLorLB(void);

bool PadDownMenu(void);
bool PadTriggerMenu(void);

bool PadRight(); // スティック右倒す → true
bool PadLeft(); // スティック左倒す → true

Vector2 PadStick(); // スティックの値取得(-1.0f~1.0f)

void ControlUpdate(); // GetHitKeyStateAll(keys); の下に書いてね

void DrawPad(void);