#include "../Character/Player.h"
#include "../Obj/Ball.h"
#include "GameManager.h"

/* �w�b�_�[�Ȃǂ��t�H���_�[�w�肷��\��Ȃ̂ŁA��Ńp�X�C�����s���@�� */
USING_NS_CC;

GameManager::GameManager()
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
	/// �{�[���̐����͂Ƃ肠�����A�R�s�y�@��
	std::vector<float> zdepth;
	// �ð�ނ̕Ǎ쐬
	// ���s�̍ő�l
	float maxDepth = 1000;
	// �ǂ̍ő吔
	int wallMaxNum = 30;
	// 2���֐��Ŕz�u����̂Ÿ��̂̊J������쐬
	float magnification = maxDepth / (wallMaxNum * wallMaxNum);
	// ���s�̍쐬
	for (float x = wallMaxNum; x > 0; x--)
	{
		float depth = x * x * magnification;
		depth = maxDepth - depth;
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
