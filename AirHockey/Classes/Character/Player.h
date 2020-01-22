#pragma once

#include <memory>
#include "Controller/OPRT_State.h"

class Player : 
	public cocos2d::Sprite
{
public:
	Player(const float& zdepth);
	~Player();

	/// [“x’l‚Ìæ“¾—p
	float GetDepth() const;
	cocos2d::Vec2 GetAnchorPos(const std::string& name);
private:
	void MoveUpdate();
	void update(float dt);

	std::unique_ptr<OPRT_State> _oprtState;

	cocos2d::Vec3 _localPos;
};

