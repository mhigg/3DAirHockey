#include "../Character/Player.h"
#include "../Obj/Ball.h"
#include "../Obj/BallShadow.h"
#include "../Obj/BallAfter.h"
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

std::vector<float> GameManager::GetDepths() const
{
	return _zdepth;
}

void GameManager::Init()
{
	/// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float mag = _maxDepth / (_wallMax * _wallMax);

	/// 深度値保存用の一次変数(_wallDepth, _zdepthの0番目は値が壊れているので0でいいかもしれない)　◆
	float depth;

	for (int x = _wallMax; x > 0; x--)
	{
		depth = x * x * mag;
		depth = _maxDepth - depth;
		_zdepth.emplace_back(depth);
	}
	/// 仮の初期化 ◆
	_zdepth[0] = 0.f;

	/// ボールの生成
	auto ball = new Ball(_zdepth);
	ball->setName("ball");
	this->addChild(ball, static_cast<int>(SpriteNum::BALL));

	/// ボールの影の生成
	for (int k = 0; k < 4; k++)
	{
		auto ballShadow = new BallShadow(k);
		this->addChild(ballShadow);
	}

	/// 残像の生成
	auto ballAfter = new BallAfter();
	ballAfter->setName("ballAfter");
	this->addChild(ballAfter, static_cast<int>(SpriteNum::BALL));

	/// プレイヤーの生成
	Player* player;
	for (int i = 0; i < 2; ++i)
	{
		depth  = (i == 0 ? _zdepth[0] : _zdepth[_wallMax - 1]);
		player = new Player(depth);
		player->setName("player" + std::to_string(i + 1));
		this->addChild(player, static_cast<int>(SpriteNum::PLAYER));
	}
}

void GameManager::update(float dt)
{
}
