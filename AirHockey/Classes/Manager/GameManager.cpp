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

std::vector<float> GameManager::GetDepth() const
{
	return _zdepth;
}

void GameManager::Init()
{
	/// 2���֐��Ŕz�u����̂Ÿ��̂̊J������쐬
	float mag = _maxDepth / (_wallMax * _wallMax);

	/// �[�x�l�ۑ��p�̈ꎟ�ϐ�
	float depth;

	for (int x = _wallMax; x > 0; x--)
	{
		depth = x * x * mag;
		depth = _maxDepth - depth;
		_zdepth.emplace_back(depth);
	}

	/// �{�[���̐���
	auto ball = new Ball(_zdepth);
	ball->setName("ball");
	this->addChild(ball);

	/// �{�[���̉e�̐���
	auto ballShadow = new BallShadow();
	this->addChild(ballShadow);

	/// �v���C���[�̐���
	this->addChild(Player::createPlayer(_zdepth[0]));
}

void GameManager::update(float dt)
{
}
