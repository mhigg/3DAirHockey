#pragma once

#include "cocos2d.h"
#include "OPRT_State.h"

class MouseCtl : 
	public OPRT_State
{
public:
	MouseCtl(cocos2d::Node* node);
	~MouseCtl();

	void Update();
	// À•Wæ“¾—p
	cocos2d::Vec2 GetPoint() const;		
};

