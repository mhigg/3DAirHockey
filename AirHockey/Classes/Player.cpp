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
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_mouse.reset(new MouseCtl(this));
#else
	_oprtState.reset(new OPRT_Touch());
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
	/// 座標の更新を行っている
	this->setPosition(_mouse->GetPos());
#else

#endif
}
