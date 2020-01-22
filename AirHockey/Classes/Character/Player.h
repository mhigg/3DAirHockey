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
	/// ���Ő[�x�l��n���悤�ɂ��Ă���
	float GetDepth() const;			/// gameManager�̊֐�����ύX���Ă����@��
private:
	void MoveUpdate();
	void update(float dt);

	std::unique_ptr<OPRT_State> _oprtState;
	float _depth;			// �[�x�l
};

