#pragma once
#include <memory>

class Score
{
public:
	//������
	static void Init();
	//�X�R�A�ǉ�
	static void Add(uint32_t score);
	//�X�R�A�`��
	static void Draw();

	//�n�C�X�R�A�̍X�V(�������m�F)
	static void HighScoreUpdate();

	static uint32_t nowScore_;		//���݂̃X�R�A
	static uint32_t highScore_;		//���܂ł̃n�C�X�R�A
};