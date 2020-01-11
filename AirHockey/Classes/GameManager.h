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
private:
	CREATE_FUNC(GameManager);

	void ballCreate();
	void update(float dt);
};

