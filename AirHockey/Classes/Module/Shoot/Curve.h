#pragma once
#include "cocos2d.h"

/// ����Ăяo���悤�ɂ���\��@��

struct Curve
{
	cocos2d::Vec3 operator()(cocos2d::Node* ball)
	{
		return cocos2d::Vec3::ZERO;
	}
};