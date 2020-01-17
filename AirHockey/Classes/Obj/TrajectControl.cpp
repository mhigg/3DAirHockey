#include "TrajectControl.h"
#include "../Manager/GameManager.h"
#include "../Obj/Ball.h"
#include "../Character/Player.h"

USING_NS_CC;

TrajectControl::TrajectControl() : _speed(2.f,2.f,2.f)
{
	_vel = _speed;
}

TrajectControl::~TrajectControl()
{
}

cocos2d::Vec3 TrajectControl::GetVel(State& state)
{
	if (state == State::NORMAL)
	{
		/// �f�t�H���g�̑��x��Ԃ�
		/// �����ɑ��x�𔽓]�����邩�̏�����ǉ�����
		return _vel;
	}
	else if (state == State::CURVE)
	{
		/// �v�Z�������x��Ԃ�
		return CalCurveVel();
	}
	/// ��O����
	return _vel;
}

void TrajectControl::CalBezierPoint()
{
	/// �����Ă���V�[�����Q�[���V�[���̎��Ƃ����������K�v�ȏꍇ
	/// ���菈���Ƃ��Ēǉ����Ă���

	/// �[�_�̐����ɕK�v�Ȃ��̂��擾���Ă���
	auto runScene	= Director::getInstance()->getRunningScene();
	auto gameMng	= (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto player		= (Player*)gameMng->getChildByName("player");
	auto ball		= (Ball*)gameMng->getChildByName("ball");

	/// �ꎟ�x�W�F�̋Ȑ��𐶐����邽�߂ɕK�v�Ȃ���
	Vec3 start, mid, end;
	float a, b;

	start = Vec3(player->getPositionX(), player->getPositionY(), player->GetDepth());
	mid = Vec3(300, 200, gameMng->GetDepth()[13]);
	end = Vec3(200, 50, gameMng->GetDepth()[29]);				/// ���̐ݒ�@��

	/// �x�W�F�Ȑ��̌v�Z���g���A�[�_�̈ʒu�����߂Ă���
	/// �[�x�l�ɂ���āA�o�^���鏇�Ԃ�ς��Ȃ���΂Ȃ�Ȃ��@��
	for (int i = 0; i < _points.size(); ++i)
	{
		b = (float)(i / _points.size());
		a = 1.f - b;

		/// �[�_�̓o�^�����Ă���
		_points[i].x = (a * a * start.x) + (2 * a * b * mid.x) + (b * b * end.x);
		_points[i].y = (a * a * start.y) + (2 * a * b * mid.y) + (b * b * end.y);
		_points[i].z = (a * a * start.z) + (2 * a * b * mid.z) + (b * b * end.z);
	}
}

cocos2d::Vec3 TrajectControl::CalCurveVel()
{
	/// ������Z���W���擾���āA�i�s�����̈��̕ǂ̈ʒu�ɂ��鐧��_�̃x�N�g����
	/// �擾���āA���x�~�x�N�g�����ʂ�Ԃ��悤�ɂ���
	auto runScene = Director::getInstance()->getRunningScene();
	auto gameMng  = (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto ball	  = (Ball*)gameMng->getChildByName("ball");
	/// Z���̐i�s�������m�F���āA�[�_������ʒu��ς���

	return Vec3::ZERO;
}
