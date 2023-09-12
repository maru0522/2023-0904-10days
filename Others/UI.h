#pragma once
#include <memory>
#include <vector>
#include <Vector2.h>

enum class UIType
{
	Abutton,	//Aボタン
	Bbutton,	//Bボタン
	Rbutton,	//Rボタン
	Lstick,		//Lスティック
	Move,		//移動
	Attack,		//なぎ払い
	Skewer,		//串刺し

    Tutorial1,
    Tutorial2,
    Tutorial3,
    Tutorial4,
    Tutorial5,
    Tutorial6,
    Tutorial7,

	Max			//最大数(for文とか回す用)
};

class UI
{
private:
	static std::vector<uint32_t> ui_;	//UIテクスチャハンドル群
	static std::vector<Vector2> pos_;	//UI座標群
	static std::vector<float> size_;	//UIサイズ群

public:
	UI();
	static void AllLoad();
	static void Init();
	static void Update();

	//指定されたUIを描画
	static void Draw(UIType uiType);

	//指定されたUIの描画座標設定
	static void SetPos(UIType uiType,const Vector2& pos);
	//指定されたUIの描画サイズ設定
	static void SetSize(UIType uiType, float size);
};