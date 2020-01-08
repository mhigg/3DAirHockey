#include "Player.h"
#include "MouseCtl.h"

USING_NS_CC;

Player::Player()
{
	/// 仮の画像を追加している
	auto sprite = Sprite::create("player.png");
	this->addChild(sprite);

	this->setPosition(Vec2::ZERO);
	/// 仮のマウス設定
	_mouse.reset(new MouseCtl(this));

	this->scheduleUpdate();
}

Player::~Player()
{
}

Player * Player::createPlayer()
{
	return Player::create();
}

void Player::update(float dt)
{
	/// 座標の更新を行っている
	this->setPosition(_mouse->GetPos());
}
