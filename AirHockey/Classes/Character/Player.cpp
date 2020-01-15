#include "Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/Input_Touch.h"
#include "Manager/PointWithDepth.h"

USING_NS_CC;

float Player::_depth;

Player::Player()
{
	/// 仮の画像を追加している　◆
	auto sprite = Sprite::create("player.png");
	setTexture("player.png");
	/// プレイヤーのタグ名を設定している(仮)　◆
	this->setName("player");
	this->addChild(sprite);
	this->setPosition(Vec2::ZERO);

	/// 仮のマウス設定
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_oprtState.reset(new MouseCtl(this));
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
	_depth = depth;
	return Player::create();
}

float Player::GetDepth() const
{
	return _depth;
}

void Player::update(float dt)
{
	/// 座標の更新を行っている
	this->setPosition(_oprtState->GetPoint());
	lpPointWithDepth.GetInstance().SetVanishingPoint(_oprtState->GetPoint() / 100 - cocos2d::Vec2(400,300) / 100);
}
