#include "Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/OPRT_Touch.h"
#include "../Controller/OPRT_Gyro.h"
#include "Manager/PointWithDepth.h"
#include "Manager/GameManager.h"

USING_NS_CC;

Player::Player(const float& zdepth)
{
	_depth = zdepth;

	/// 仮の画像を追加している　◆
	// 中央
	auto sprite = Sprite::create("image/player/player_center.png");
	sprite->setTag(static_cast<int>(PL_ANCHOR::CENTER));
	this->addChild(sprite);
	
	// 左上
	sprite = Sprite::create("image/player/player_left_up.png");
	sprite->setAnchorPoint({ 1, 0 });
	sprite->setTag(static_cast<int>(PL_ANCHOR::LEFTUP));
	this->addChild(sprite);
	// 左下
	sprite = Sprite::create("image/player/player_left_down.png");
	sprite->setAnchorPoint({ 1, 1 });
	sprite->setTag(static_cast<int>(PL_ANCHOR::LEFTDOWN));
	this->addChild(sprite);

	// 右上
	sprite = Sprite::create("image/player/player_light_up.png");
	sprite->setAnchorPoint({ 0, 0 });
	sprite->setTag(static_cast<int>(PL_ANCHOR::RIGHTUP));
	this->addChild(sprite);

	// 右下
	sprite = Sprite::create("image/player/player_light_down.png");
	sprite->setAnchorPoint({ 0, 1 });
	sprite->setTag(static_cast<int>(PL_ANCHOR::RIGHTDOWN));
	this->addChild(sprite);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin =	 Director::getInstance()->getVisibleOrigin();

	// 座標を真ん中にセット
	this->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	// 消失点の変更
	//lpPointWithDepth.GetInstance().SetVanishingPoint((-this->getPosition() + Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height)));
	/// 仮のマウス設定
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_oprtState.reset(new MouseCtl(this));
#else
	_oprtState.reset(new OPRT_Gyro(this));
#endif
	this->scheduleUpdate();
}

Player::~Player()
{
}

float Player::GetDepth() const
{
	return _depth;
}

void Player::MoveUpdate()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/// 画面サイズの取得
	auto scrSize = Director::getInstance()->sharedDirector()->getOpenGLView()->getFrameSize();
	auto pos	 = _oprtState->GetPoint();
	Size size	 = { this->getChildByTag(static_cast<int>(PL_ANCHOR::LEFTUP))->getContentSize().width + this->getChildByTag(static_cast<int>(PL_ANCHOR::RIGHTUP))->getContentSize().width,
					 this->getChildByTag(static_cast<int>(PL_ANCHOR::LEFTDOWN))->getContentSize().height + this->getChildByTag(static_cast<int>(PL_ANCHOR::RIGHTDOWN))->getContentSize().height};
	// 消失点の変更
	// lpPointWithDepth.GetInstance().SetVanishingPoint((-pos + Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height)));

	/// X軸の移動範囲チェック
	if ((pos.x + size.width / 2 < scrSize.width) &&
		(pos.x - size.width / 2 > 0))
	{
		this->setPositionX(pos.x);
	}

	/// Y軸の移動範囲チェック
	if ((pos.y + size.height / 2 < scrSize.height) &&
		(pos.y - size.height / 2 > 0))
	{
		this->setPositionY(pos.y);
	}
}

void Player::update(float dt)
{
	_oprtState->Update();
	MoveUpdate();
	
	// 奥行きの深さによって、プレイヤーのサイズを変更するようにしている
	setScale(lpPointWithDepth.GetInstance().GetScale(_depth));
}
