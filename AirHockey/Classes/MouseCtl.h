#pragma once

#include "cocos2d.h"

class MouseCtl
{
public:
	MouseCtl(cocos2d::Node* node);
	~MouseCtl();

	// ���W�擾�p
	cocos2d::Vec2 GetPos() const;		
private:
	cocos2d::Vec2 _mPos;		// �}�E�X���W�ۑ��p	
};

