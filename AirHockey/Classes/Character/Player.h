#pragma once

#include <memory>
#include "Controller/OPRT_State.h"

enum class PL_ANCHOR
{
	CENTER,
	LEFTUP,
	LEFTDOWN,
	RIGHTUP,
	RIGHTDOWN,
	MAX
};

class Player : 
	public cocos2d::Sprite
{
public:
	Player(const float& zdepth);
	~Player();
	/// 仮で深度値を渡すようにしている
	float GetDepth() const;			/// gameManagerの関数名を変更しておく　◆
private:
	void MoveUpdate();
	void update(float dt);

	std::unique_ptr<OPRT_State> _oprtState;
	float _depth;			// 深度値
};

