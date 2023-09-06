#include "Particle.h"
#include "MathUtil.h"
#include "Easing.h"
#include "DxLib.h"

Emitter::Emitter()
{
	addInterval_ = 0;
	maxScale_ = 0;
	minScale_ = 0;
	scaling_ = 0;
}

void Emitter::Init()
{
}

void Emitter::Update(bool isGravity)
{
	//�������s�����p�[�e�B�N����S�폜
	for (size_t i = 0; i < particles_.size(); i++)
	{
		if (particles_[i].frame >= particles_[i].num_frame)
		{
			particles_.erase(particles_.begin() + i);
			i = (size_t)-1;
		}
	}

	//�S�p�[�e�B�N���X�V
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i].timer.Update();

		//�o�߃t���[�����J�E���g
		particles_[i].frame++;

		//�X�P�[���̐��`���
		particles_[i].scale.x = Easing::lerp(particles_[i].startScale.x, particles_[i].endScale.x, particles_[i].timer.GetTimeRate());
		particles_[i].scale.y = Easing::lerp(particles_[i].startScale.y, particles_[i].endScale.y, particles_[i].timer.GetTimeRate());

		//�����x�𑬓x�ɉ��Z
		particles_[i].velo += particles_[i].accel;

		//�d�͉��Z
		if (isGravity)
		{
			particles_[i].velo.y += particles_[i].gravity;
		}

		//���x�ɂ��ړ�
		particles_[i].pos += particles_[i].velo;
	}
}

void Emitter::DrawBoxParticle()
{
	for (auto& p : particles_)
	{
		DrawBox(
			(uint32_t)(p.pos.x - p.scale.x), (uint32_t)(p.pos.y - p.scale.y),
			(uint32_t)(p.pos.x + p.scale.x), (uint32_t)(p.pos.y + p.scale.y),
			0xffffff, true);
	}
}

void Emitter::DrawGraph()
{
	for (auto& p : particles_)
	{
		//��ł�낤;
	}
}

void Emitter::Add(uint32_t addNum, float life, float minScale, float maxScale, Vector2 minVelo, Vector2 maxVelo, Vector2 accel, float minRot, float maxRot, uint32_t color)
{
	for (uint32_t i = 0; i < addNum; i++)
	{
		//�w�肵���ő吔�����Ă��琶�����Ȃ�
		if (particles_.size() >= maxCount_)
		{
			return;
		}

		//���X�g�ɗv�f��ǉ�
		particles_.emplace_back();
		//�ǉ������v�f�̎Q��
		Particle& p = particles_.back();
		//�G�~�b�^�[�̒����烉���_���ō��W������
		float x = Math::Function::Random<float>(-scale_.x, scale_.x);
		float y = Math::Function::Random<float>(-scale_.y, scale_.y);
		Vector2 randomPos(x, y);
		//�����͈̔͂���傫�������_���Ō���
		float scale_ = Math::Function::Random<float>(minScale, maxScale);
		Vector2 randomScale(scale_, scale_);
		//�����͈̔͂����΂����������_���Ō���
		Vector2 velo = {
			Math::Function::Random<float>(minVelo.x,maxVelo.x),
			Math::Function::Random<float>(minVelo.y,maxVelo.y) };
		//�����͈̔͂����]�������_���Ō���
		float rot = Math::Function::Random<float>(minRot, maxRot);

		//���܂������W�ɃG�~�b�^�[���̂̍��W�𑫂��Đ������ʒu��
		p.pos = randomPos + pos_;
		//���ł������ɍ��킹�ĉ�]
		p.rot = rot;
		p.velo = velo;
		p.accel = accel;
		p.num_frame = (uint32_t)life;
		p.scale = randomScale;
		p.startScale = randomScale;
		p.endScale = { 0,0 };
		p.color = color;
		//�C�[�W���O�p�̃^�C�}�[��ݒ�A�J�n
		p.timer.maxTime_ = life;
		p.timer.Start();
	}
}

void Emitter::SetScale(Vector2& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//�g�k�p�Ɍ��̃T�C�Y��ۊ�
}

void Emitter::SetScalingTimer(float timer)
{
	scalingTimer_.maxTime_ = timer;
}

void Emitter::StartScalingTimer(bool isRun)
{
	if (isRun)
	{
		scalingTimer_.Start();	//�ݒ�Ɠ����Ƀ^�C�}�[���X�^�[�g
	}
	else
	{
		scalingTimer_.ReverseStart();	//�ݒ�Ɠ����Ƀ^�C�}�[���X�^�[�g
	}
}
