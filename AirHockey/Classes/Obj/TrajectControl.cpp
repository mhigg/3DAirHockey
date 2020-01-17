#include "TrajectControl.h"
#include "../Manager/GameManager.h"
USING_NS_CC;

TrajectControl::TrajectControl()
{
}

TrajectControl::~TrajectControl()
{
}

cocos2d::Vec3 TrajectControl::GetVel(State& state)
{
	if (state == State::NORMAL)
	{
		/// �f�t�H���g�̑��x��Ԃ�
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
	auto runScene = Director::getInstance()->getRunningScene();
	auto gameMng = (GameManager*)runScene->getChildByName("GameLayer")->getChildByName("GameManager");
	auto player = (Player*)
	auto ball = (Ball*)gameMng->getChildByName("ball");

	/// �ꎟ�x�W�F�̋Ȑ��𐶐����邽�߂ɕK�v�Ȃ���
	Vec3 start, mid, end;

	start = 

	for (int i = 0; i < gameMng->GetDepth().size(); ++i)
	{

	}
}

cocos2d::Vec3 TrajectControl::CalCurveVel()
{
	/// ������Z���W���擾���āA�i�s�����̈��̕ǂ̈ʒu�ɂ��鐧��_�̃x�N�g����
	/// �擾���āA���x�~�x�N�g�����ʂ�Ԃ��悤�ɂ���
	return Vec3::ZERO;
}
