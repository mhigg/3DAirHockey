#pragma once

#include <memory>
#include "cocos2d.h"
#include "Controller/OPRT_State.h"
#include "Controller/OPRT_Gyro.h"

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
	std::unique_ptr<OPRT_Gyro> _gyro;
	cocos2d::Vec2 ratio;
	static float _depth;			// 深度値
};

