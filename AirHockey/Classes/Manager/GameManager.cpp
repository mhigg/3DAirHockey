#include "../Character/Player.h"
#include "../Obj/Ball.h"
#include "GameManager.h"

USING_NS_CC;

GameManager::GameManager() : _maxDepth(1000.f), _wallMaxNum(30)
{
	/// プレイヤーの追加
	this->addChild(Player::createPlayer(1.0));
	/// ボールの追加
	ballCreate();
	this->setName("gameManager");
	this->scheduleUpdate();
}

GameManager::~GameManager()
{
}

GameManager * GameManager::createGameMng()
{
	return GameManager::create();
}

void GameManager::ballCreate()
{
	std::vector<float> zdepth;
	// 2次関数で配置するのでｸﾞﾗﾌの開き具合を作成
	float magnification = _maxDepth / (_wallMaxNum * _wallMaxNum);
	// 深度値保存用の一次変数
	float depth;
	for (int x = _wallMaxNum; x > 0; x--)
	{
		depth = x * x * magnification;
		depth = _maxDepth - depth;
		zdepth.emplace_back(depth);
	}
	// ﾌｨｰﾙﾄﾞの基本サイズ
	Point wallSize = { 700,500 };

	auto ball = new Ball(zdepth);
	ball->setName("ball");
	this->addChild(ball);
}

void GameManager::update(float dt)
{
}
