#pragma once
#include "cocos2d.h"

/// ��񂾂��Ăяo���悤�ɗ\��@��

struct Normal
{
	cocos2d::Vec3 operator()(cocos2d::Node* ball)
	{
		return cocos2d::Vec3::ZERO;
	}
};