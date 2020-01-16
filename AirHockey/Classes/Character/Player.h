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
	/// ���Ő[�x�l��n���悤�ɂ��Ă���
	static Player* createPlayer(const float& depth);
	float GetDepth() const;
private:
	bool IsMoveRange(cocos2d::Vec2& pos);		// �ړ��͈͓����m�F���邽�߂̊֐�
	void update(float dt);
	CREATE_FUNC(Player);

	std::unique_ptr<OPRT_State> _oprtState;

	cocos2d::Size _size;
	static float _depth;			// �[�x�l
};

