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

	cocos2d::Vec3 GetLocalPos(void);		// _localPos‚Ìæ“¾
private:
	bool Init(void);
	void update(float dt) override;

	std::vector<float> _wallDepth;	// •Ç‚Ì‰œs
	float _radius;					// ‹Ê‚Ì”¼Œa
	
	bool xReverse = false;
	bool yReverse = false;
	bool zReverse = false;
	
};
