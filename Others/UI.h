#pragma once
#include <memory>
#include <vector>
#include <Vector2.h>

enum class UIType
{
	Abutton,	//A�{�^��
	Bbutton,	//B�{�^��
	Rbutton,	//R�{�^��
	Lstick,		//L�X�e�B�b�N

	Max			//�ő吔(for���Ƃ��񂷗p)
};

class UI
{
private:
	static std::vector<uint32_t> ui_;	//UI�e�N�X�`���n���h���Q
	static std::vector<Vector2> pos_;	//UI���W�Q

public:
	UI();
	static void Init();
	static void Update();

	//�w�肳�ꂽUI��`��
	static void Draw(UIType uiType);

	//�w�肳�ꂽUI�̕`����W�ݒ�
	static void SetPos(UIType uiType,const Vector2& pos);
};