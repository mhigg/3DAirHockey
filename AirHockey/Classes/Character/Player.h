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
	float getDepth() const;
private:
	void update(float dt);
	CREATE_FUNC(Player);

	std::unique_ptr<OPRT_State> _oprtState;

	cocos2d::Size _size;

	const float debug = 0;
	//static float _depth;			// �[�x�l (createPlayer��static�̂��߁Astatic��t���Ă��邪�C�����l����) ��
};

