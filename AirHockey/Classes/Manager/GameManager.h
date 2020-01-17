#pragma once

#include <memory>
#include "cocos2d.h"

class Player;
class Ball;

// ZOrder�p
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
	std::vector<float> GetDepth() const;
private:
	CREATE_FUNC(GameManager);

	void update(float dt);
	void Init();

	std::vector<float> _zdepth;		// �[�x�l��ۑ��������

	const cocos2d::Vec2 _moveRange;
	const float _maxDepth;	// ���s�̍ő�l
	const int _wallMax;		// �ǂ̍ő吔
};

