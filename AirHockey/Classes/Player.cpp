#include "Player.h"
#include "MouseCtl.h"

USING_NS_CC;

Player::Player()
{
	/// ���̉摜��ǉ����Ă���
	auto sprite = Sprite::create("player.png");
	this->addChild(sprite);

	this->setPosition(Vec2::ZERO);
	/// ���̃}�E�X�ݒ�
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
	/// ���W�̍X�V���s���Ă���
	this->setPosition(_mouse->GetPos());
}
