#include "Player.h"
#include "Ball.h"
#include "GameManager.h"

/* �w�b�_�[�Ȃǂ��t�H���_�[�w�肷��\��Ȃ̂ŁA��Ńp�X�C�����s���@�� */

GameManager::GameManager()
{
	_player.reset(Player::createPlayer(1.0));
	this->scheduleUpdate();
}

GameManager::~GameManager()
{
}

GameManager * GameManager::createGameMng()
{
	return GameManager::create();
}

void GameManager::update(float dt)
{
}
