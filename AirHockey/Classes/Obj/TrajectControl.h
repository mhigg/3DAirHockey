#pragma once
#include <vector>
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

	cocos2d::Vec3 GetVel(State& state);		// ���x�擾�p
	void CalBezierPoint();				
private:
	cocos2d::Vec3 CalCurveVel();			// �J�[�u����O���̑��x�v�Z�p 
	const cocos2d::Vec3 _vel;	
	std::vector<cocos2d::Vec3> _points;		// �Ȑ��̒[�_(�Ƃ肠�����A�ϒ�) ��
};

