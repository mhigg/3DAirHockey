#pragma once
#include "cocos2d.h"
#include "../Manager/PointWithDepth.h"

class Obj :
	public cocos2d::Sprite
{
public:
	Obj();
	~Obj();
private:
	bool Init(void);
	virtual void update(float dt) = 0;

protected:
	cocos2d::Vec3 _localPos;
};