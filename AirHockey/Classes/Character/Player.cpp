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
	// 中央
	auto sprite = Sprite::create("image/player/player_center.png");
	sprite->setName("center");
	//this->setContentSize(sprite->getContentSize());
	/// プレイヤーのタグ名を設定している(仮)　◆
	this->setName("player");
	this->addChild(sprite);
	
	// 左上
	sprite = Sprite::create("image/player/player_left_up.png");
	sprite->setAnchorPoint({ 1, 0 });
	sprite->setName("leftUp");
	this->addChild(sprite);
	// 左下
	sprite = Sprite::create("image/player/player_left_down.png");
	sprite->setAnchorPoint({ 1, 1 });
	sprite->setName("leftDown");
	this->addChild(sprite);

	// 右上
	sprite = Sprite::create("image/player/player_light_up.png");
	sprite->setAnchorPoint({ 0, 0 });
	sprite->setName("lightUp");
	this->addChild(sprite);
	// 右下
	sprite = Sprite::create("image/player/player_light_down.png");
	sprite->setAnchorPoint({ 0, 1 });
	sprite->setName("lightDown");
	this->addChild(sprite);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// 座標を真ん中にセット
	this->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	// 消失点の変更
	lpPointWithDepth.GetInstance().SetVanishingPoint((-this->getPosition() + Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height)));
	ratio = cocos2d::Vec2(0.5f, 0.5f);
	/// 仮のマウス設定
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_oprtState.reset(new MouseCtl(this));
#else
	_gyro.reset(new OPRT_Gyro());
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

void Player::MoveUpdate()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/// 画面サイズの取得
	auto scrSize = Director::getInstance()->sharedDirector()->getOpenGLView()->getFrameSize();
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto pos	 = _oprtState->GetPoint();
#else
	auto sensor = _gyro->GetSensor()/10;
	// 縦移動
	cocos2d::Vec2 pos = Vec2(visibleSize.width * ratio.x, visibleSize.height * ratio.y);
	if(abs(sensor.x) > 0.0f)
	{
        ratio.x += sensor.x;
    }
	if(abs(sensor.z) > 0.0f)
    {
	    ratio.y += sensor.y;
    }
	if(ratio.x > 1)
	{
	    ratio.x = 1;
    }
	else if (ratio.x < 0)
	{
		ratio.x = 0;
	}
	if(ratio.y > 1)
    {
	    ratio.y = 1;
    }
	if (ratio.y < 0)
	{
		ratio.y = 0;
	}
#endif
	Size size	 = { this->getChildByName("leftUp")->getContentSize().width + this->getChildByName("lightUp")->getContentSize().width,
					 this->getChildByName("leftDown")->getContentSize().height + this->getChildByName("lightDown")->getContentSize().height};
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
	MoveUpdate();
	
	// 奥行きの深さによって、プレイヤーのサイズを変更するようにしている
	setScale(lpPointWithDepth.GetInstance().GetScale(_depth));
}
