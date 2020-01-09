#pragma once

#include <memory>
#include "cocos2d.h"

class MouseCtl;
class OPRT_State;

class Player : 
	public cocos2d::Node
{
public:
	Player();
	~Player();

	static Player* createPlayer();
private:
	void update(float dt);
	CREATE_FUNC(Player);
	std::unique_ptr<MouseCtl> _mouse;
	std::unique_ptr<OPRT_State> _oprtState;
};

