#pragma once

#include <memory>
#include "Controller/OPRT_State.h"

class Player : 
	public cocos2d::Sprite
{
public:
	Player();
	~Player();
	/// 仮で深度値を渡すようにしている
	static Player* createPlayer(const float& depth);
	float GetDepth() const;			/// gameManagerの関数名を変更しておく　◆
private:
	void MoveUpdate();
	void update(float dt);
	CREATE_FUNC(Player);

	std::unique_ptr<OPRT_State> _oprtState;
	static float _depth;			// 深度値
};

