#include "../Character/Player.h"
#include "../Obj/Ball.h"
#include "GameManager.h"

USING_NS_CC;

GameManager::GameManager() : _maxDepth(1000.f), _wallMaxNum(30)
{
	/// �v���C���[�̒ǉ�
	this->addChild(Player::createPlayer(1.0));
	/// �{�[���̒ǉ�
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
	// 2���֐��Ŕz�u����̂Ÿ��̂̊J������쐬
	float magnification = _maxDepth / (_wallMaxNum * _wallMaxNum);
	// �[�x�l�ۑ��p�̈ꎟ�ϐ�
	float depth;
	for (int x = _wallMaxNum; x > 0; x--)
	{
		depth = x * x * magnification;
		depth = _maxDepth - depth;
		zdepth.emplace_back(depth);
	}
	// ̨���ނ̊�{�T�C�Y
	Point wallSize = { 700,500 };

	auto ball = new Ball(zdepth);
	ball->setName("ball");
	this->addChild(ball);
}

void GameManager::update(float dt)
{
}
