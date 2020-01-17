#include "../Character/Player.h"
#include "../Obj/Ball.h"
#include "../Obj/BallShadow.h"
#include "GameManager.h"

USING_NS_CC;

GameManager::GameManager() : _maxDepth(1000.f), _wallMax(30), _moveRange(1024 / 2, 576 / 2)
{
	Init();
	this->setName("GameManager");
	this->scheduleUpdate();
}

GameManager::~GameManager()
{
}

GameManager * GameManager::createGameMng()
{
	return GameManager::create();
}

cocos2d::Vec2 GameManager::GetMovingRange() const
{
	return _moveRange;
}

void GameManager::Init()
{
	/// ボールの生成
	std::vector<float> zdepth;

	/// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float mag = _maxDepth / (_wallMax * _wallMax);

	/// 深度値保存用の一次変数
	float depth;

	for (int x = _wallMax; x > 0; x--)
	{
		depth = x * x * mag;
		depth = _maxDepth - depth;
		zdepth.emplace_back(depth);
	}
	auto ball = new Ball(zdepth);
	ball->setName("ball");
	this->addChild(ball);

	/// ボールの影の生成
	auto ballShadow = new BallShadow();
	this->addChild(ballShadow);

	/// プレイヤーの生成
	this->addChild(Player::createPlayer(zdepth[0]));
}

void GameManager::update(float dt)
{
}
