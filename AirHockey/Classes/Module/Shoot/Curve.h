#pragma once
#include "cocos2d.h"

/// –ˆ‰ñŒÄ‚Ño‚·‚æ‚¤‚É‚·‚é—\’è@Ÿ

struct Curve
{
	cocos2d::Vec3 operator()(cocos2d::Node* ball)
	{
		return cocos2d::Vec3::ZERO;
	}
};