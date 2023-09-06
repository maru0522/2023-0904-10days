#pragma once
#include <memory>

namespace Easing
{
	//�C�[�W���O�Q
	float lerp(float start, float end, float timeRate);

	float InQuad(float timeRate);
	float OutQuad(float timeRate);
	float InQuad(float start, float end, float timeRate);
	float OutQuad(float start, float end, float timeRate);

	float InOutQuad(float timeRate);
	float InOutQuad(float start, float end, float timeRate);

	float OutBounce(float timeRate);

	float OutBounce(float start, float end, float timeRate);
	float InBounce(float start, float end, float timeRate);
	float InOutBounce(float start, float end, float timeRate);

	float InElastic(float timeRate);
	float OutElastic(float timeRate);

	float InElastic(float start, float end, float timeRate);
	float OutElastic(float start, float end, float timeRate);

	float InBack(float timeRate);
	float InBack(float start, float end, float timeRate);

	float OutBack(float timeRate);
	float OutBack(float start, float end, float timeRate);

	float InOutBack(float timeRate);
	float InOutBack(float start, float end, float timeRate);

	//�C�[�W���O�p�̃^�C�}�[(�b��)
	class EaseTimer
	{
	private:
		//�i�s���t���O
		bool run_ = false;
		//�i�s�I���t���O
		bool end_ = false;
		//��ޒ��t���O
		bool reverse_ = false;
		//��ޏI���t���O
		bool reverseend_ = false;

	public:
		//�R���X�g���N�^
		EaseTimer(float maxTime = 1.0f) {
			maxTime_ = maxTime;
		}

		float nowTime_ = 0.0f;		// ���݂̌o�ߎ���
		float maxTime_ = 1.0f;		// ���b�����Ĉړ����邩

		//�f���^�^�C���擾�p�̂��B�ϐ��������Ⴎ����
		static double Time;
		static float NowTime;
		static float DeltaTime;
		static double FPSCheckTime;
		static uint32_t FPS;
		static uint32_t FPSCounter;

		//�^�C�}�[���i��ł邩�t���O�擾
		bool GetRun()const { return run_; };
		//��x�ł��i�s���ꂽ��true�ɂȂ�t���O�擾
		bool GetStarted()const { return run_ || end_; };
		//�^�C�}�[���i�ݐ؂������t���O�擾
		bool GetEnd()const { return end_; };
		//�i�ݐ؂������t���O��ݒ�
		void SetEnd(bool end) { end_ = end; };

		//�^�C�}�[���߂��Ă邩�t���O�擾
		bool GetReverse()const { return reverse_; };
		//�^�C�}�[���߂�؂������t���O�擾
		bool GetReverseEnd()const { return reverseend_; };
		//�߂�؂������t���O��ݒ�
		void SetReverseEnd(bool reverseend) { reverseend_ = reverseend; };
		//��x�ł���ނ��ꂽ��true�ɂȂ�t���O�擾
		bool GetReverseStarted()const { return reverse_ || reverseend_; };

		//�^�C�}�[�͋N�����Ȃ�������������
		void Reset();
		//�Ăяo������^�C�}�[���i�s
		void Start();
		//�Ăяo������^�C�}�[�����
		void ReverseStart();
		//�o�ߎ��Ԃ��X�V
		void Update(const float elapseTimer = 1.0f);
		//�^�C�}�[�X�V+�������[�v
		void Roop(const float elapseTimer = 1.0f);
		void RoopReverse(const float elapseTimer = 1.0f);

		//���݂��ő�b���̂����ǂ̊�������(0.0f�`1.0f)�̒l�ŕԂ�
		float GetTimeRate();

		//�f���^�^�C���p������
		static void InitDeltaTime();
		//�f���^�^�C���p�X�V
		static void UpdateDeltaTime();
	};
};