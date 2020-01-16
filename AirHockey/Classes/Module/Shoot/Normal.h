#pragma once
#include "cocos2d.h"

/// ˆê‰ñ‚¾‚¯ŒÄ‚Ño‚·‚æ‚¤‚É—\’è@Ÿ

struct Normal
{
	cocos2d::Vec3 operator()(cocos2d::Node* ball)
	{
		return cocos2d::Vec3::ZERO;
	}
};