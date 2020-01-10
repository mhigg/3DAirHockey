#include "Player.h"
#include "MouseCtl.h"
#include "Input_Touch.h"

USING_NS_CC;

Player::Player()
{
	/// ���̉摜��ǉ����Ă���
	auto sprite = Sprite::create("player.png");
	this->addChild(sprite);

	this->setPosition(Vec2::ZERO);
	/// ���̃}�E�X�ݒ�
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_mouse.reset(new MouseCtl(this));
#else
	_oprtState.reset(new Input_Touch(this));
#endif

	this->scheduleUpdate();
}

Player::~Player()
{
}

Player * Player::createPlayer(const float& depth)
{
	//_depth = depth;
	return Player::create();
}

void Player::update(float dt)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	/// ���W�̍X�V���s���Ă���
	this->setPosition(_mouse->GetPos());
#else
	this->setPosition(_oprtState->GetPos());
#endif
}
