#pragma once

#include <memory>
#include "cocos2d.h"

class Player;
class Ball;

class GameManager :
	public cocos2d::Node
{
public:
	GameManager();
	~GameManager();

	static GameManager* createGameMng();

	cocos2d::Vec2 GetMovingRange() const;
private:
	CREATE_FUNC(GameManager);

	void ballCreate();
	void update(float dt);
	void Init();

	const cocos2d::Vec2 _moveRange;
	const float _maxDepth;	// ���s�̍ő�l
	const int _wallMax;		// �ǂ̍ő吔
};

