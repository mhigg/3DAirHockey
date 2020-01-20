#pragma once

#include <memory>
#include "Controller/OPRT_State.h"

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
	static float _depth;			// �[�x�l
};

