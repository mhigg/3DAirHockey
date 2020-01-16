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
	/// 仮で深度値を渡すようにしている
	static Player* createPlayer(const float& depth);
	float GetDepth() const;
private:
	bool IsMoveRange(cocos2d::Vec2& pos);		// 移動範囲内か確認するための関数
	void update(float dt);
	CREATE_FUNC(Player);

	std::unique_ptr<OPRT_State> _oprtState;

	cocos2d::Size _size;
	static float _depth;			// 深度値
};

