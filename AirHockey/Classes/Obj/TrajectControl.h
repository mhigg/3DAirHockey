#pragma once
#include <array>
#include "cocos2d.h"

// �{�[���̋O���̏��
enum class State
{
	NORMAL,
	CURVE,
	MAX
};

/// �{�[���̋O������p�N���X
class TrajectControl
{
public:
	TrajectControl();
	~TrajectControl();

	cocos2d::Vec3 GetVel(const State& state);		// ���x�擾�p
	void CalBezierPoint();				
private:
	cocos2d::Vec3 CalNormalVel();
	cocos2d::Vec3 CalCurveVel();			// �J�[�u����O���̑��x�v�Z�p 

	std::array<cocos2d::Vec3, 29> _points;	// �Ȑ��̒[�_(�Ƃ肠�����Œ蒷) ��
	cocos2d::Vec3 _vel;						// �{�[���̑��x(��������񂩂������)�@��

	const cocos2d::Vec3 _speed;				// �{�[���̃X�s�[�h
};

