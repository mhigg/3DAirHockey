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
	/// ���Ő[�x�l��n���悤�ɂ��Ă���
	static Player* createPlayer(const float& depth);
	float GetDepth() const;			/// gameManager�̊֐�����ύX���Ă����@��
private:
	void MoveUpdate();
	void update(float dt);
	CREATE_FUNC(Player);

	std::unique_ptr<OPRT_State> _oprtState;
	std::unique_ptr<OPRT_Gyro> _gyro;
	cocos2d::Vec2 ratio;
	static float _depth;			// �[�x�l
};

