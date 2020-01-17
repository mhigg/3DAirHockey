#pragma once
#include "Obj.h"
#include <vector>

// �e�̈ʒu
enum SHADOW
{
	UP,		// ��
	DOWN,	// ��
	LEFT,	// ��
	RIGHT,	// �E
	MAX
};


class BallShadow :
	public Obj
{
public:
	BallShadow(cocos2d::Vec3 vec);
	BallShadow();
	~BallShadow();
private:
	bool Init(void);
	void update(float dt)override;

	SHADOW _shadowPlace;
};
