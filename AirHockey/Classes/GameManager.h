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

	std::unique_ptr<Player> _player;		// プレイヤー(後で二人プレイ用に修正する)　◆
	std::unique_ptr<Ball> _ball;
};

