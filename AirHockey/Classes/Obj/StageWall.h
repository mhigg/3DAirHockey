#pragma once
#include "Obj.h"

class StageWall :
	public Obj
{
public:
	StageWall(cocos2d::Vec2 pos, float zDepth,cocos2d::Point size, cocos2d::Color3B* color);
	StageWall();
	~StageWall();
private:
	void update(float dt)override;

};