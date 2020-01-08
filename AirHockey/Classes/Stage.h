#pragma once
#include "cocos2d.h"
#include <array>

class Stage :
	public cocos2d::Node
{
public:
	Stage();
	~Stage();
private:
	void update(float dt);

};