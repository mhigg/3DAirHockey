#pragma once

#include <memory>
#include "cocos2d.h"

class Player;
class Ball;

// ZOrder用
enum class SpriteNum
{
	SHADOW,
	BALL,
	PLAYER,
};

class GameManager :
	public cocos2d::Node
{
public:
	GameManager();
	~GameManager();

	static GameManager* createGameMng();

	cocos2d::Vec2 GetMovingRange() const;
	std::vector<float> GetDepths() const;
private:
	CREATE_FUNC(GameManager);

	void update(float dt);
	void Init();

	std::vector<float> _zdepth;		// 深度値を保存するもの

	const cocos2d::Vec2 _moveRange;
	const float _maxDepth;	// 奥行の最大値
	const int _wallMax;		// 壁の最大数
	const int _playerDepth;
};

