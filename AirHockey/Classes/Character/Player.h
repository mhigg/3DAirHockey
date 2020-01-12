#pragma once

#include <memory>
#include "cocos2d.h"
#include "Controller/OPRT_State.h"

class Player : 
	public cocos2d::Sprite
{
public:
	Player();
	~Player();
	/// ‰¼‚Å[“x’l‚ğ“n‚·‚æ‚¤‚É‚µ‚Ä‚¢‚é
	static Player* createPlayer(const float& depth);
	float GetDepth() const;
private:
	void update(float dt);
	CREATE_FUNC(Player);

	std::unique_ptr<OPRT_State> _oprtState;

	cocos2d::Size _size;
	static float _depth;			// [“x’l
};

