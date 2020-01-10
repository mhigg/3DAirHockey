#include "Player.h"
#include "Ball.h"
#include "GameManager.h"

/* ヘッダーなどをフォルダー指定する予定なので、後でパス修正を行う　◆ */

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
