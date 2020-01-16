#include "Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/Input_Touch.h"
#include "Manager/PointWithDepth.h"
#include "Manager/GameManager.h"

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

bool Player::IsMoveRange(cocos2d::Vec2& pos)
{
	auto gameMng = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("GameLayer")->getChildByName("GameManager");

	return ((pos.x - this->getContentSize().width / 2 < gameMng->GetMovingRange().x * 2) &&
		    (pos.x + this->getContentSize().width / 2 > 0) &&
		    (pos.y - this->getContentSize().height / 2 < gameMng->GetMovingRange().y * 2) &&
		    (pos.y + this->getContentSize().height / 2 > 0)); 
}

void Player::update(float dt)
{
	/*if (Director::getInstance()->getRunningScene()->getName() == "GameScene")
	{
		if (IsMoveRange(_oprtState->GetPoint()))
		{
			/// 座標の更新を行っている
			this->setPosition(_oprtState->GetPoint());
		}
	}*/

	/// 座標の更新を行っている
	this->setPosition(_oprtState->GetPoint());

	/// 座標の更新を行っている
	// this->setPosition(_oprtState->GetPoint());

	//setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(Vec3(_oprtState->GetPoint().x, _oprtState->GetPoint().y, _depth)));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetInstance().GetScale(_depth));
	
	lpPointWithDepth.GetInstance().SetVanishingPoint(_oprtState->GetPoint() / 100 - cocos2d::Vec2(400,300) / 100);
}
