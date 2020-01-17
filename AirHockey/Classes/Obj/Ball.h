#pragma once
#include "Obj.h"
#include <vector>

class Ball :
	public Obj
{
public:
	Ball(std::vector<float> depth);
	Ball();
	~Ball();

	cocos2d::Vec3 GetLocalPos(void);		// _localPos�̎擾
private:
	bool Init(void);
	void update(float dt) override;

	std::vector<float> _wallDepth;	// �ǂ̉��s
	float _radius;					// �ʂ̔��a
	
	bool xReverse = false;
	bool yReverse = false;
	bool zReverse = false;
	
};
