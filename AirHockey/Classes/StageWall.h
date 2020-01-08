#pragma once
#include "cocos2d.h"
#include "PointWithDepth.h"
#include "OPRT_State.h"

class StageWall :
	public cocos2d::Sprite
{
public:
	StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Color3B* color);
	StageWall();
	~StageWall();
private:
	void update(float dt);
	cocos2d::Vec3 _localPos;
	PointWithDepth* _pointDepth;

	OPRT_State* _state;
};