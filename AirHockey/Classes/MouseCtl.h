#pragma once

#include "cocos2d.h"

class MouseCtl
{
public:
	MouseCtl(cocos2d::Node* node);
	~MouseCtl();

	// 座標取得用
	cocos2d::Vec2 GetPos() const;		
private:
	cocos2d::Vec2 _mPos;		// マウス座標保存用	
};

