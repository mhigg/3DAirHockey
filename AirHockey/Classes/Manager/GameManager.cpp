#include "../Character/Player.h"
#include "../Obj/Ball.h"
#include "../Obj/BallAfter.h"
#include "../Obj/Shadow.h"
#include "GameManager.h"

USING_NS_CC;

GameManager::GameManager() : _maxDepth(1000.f), _wallMax(30), _moveRange(1024 / 2, 576 / 2), _playerDepth(3)
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
		auto ballShadow = new Shadow(k, "ball");
		auto playerShadow = new Shadow(k, "player", "1");
		this->addChild(ballShadow);
		this->addChild(playerShadow);
	}

	/// 残像の生成
	auto ballAfter = new BallAfter();
	ballAfter->setName("ballAfter");
	this->addChild(ballAfter, static_cast<int>(SpriteNum::BALL));

	/// プレイヤーの生成
	Player* player;
	int layer;
	for (int i = 0; i < 2; ++i)
	{
		/// プレイヤーの深度値を設定している(左 : 1P, 右 : 2P)
		depth  = (i == 0 ? _zdepth[_playerDepth] : _zdepth[_wallMax - _playerDepth - 1]);
		layer  = (i == 0 ? static_cast<int>(SpriteNum::PLAYER) : static_cast<int>(SpriteNum::SHADOW));
		player = new Player(depth);

		/// プレイヤーの名前を設定している
		player->setName("player" + std::to_string(i + 1));

		/// プレイヤーの追加
		this->addChild(player, layer);
	}
}

void GameManager::update(float dt)
{
}
