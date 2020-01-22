#include "Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/OPRT_Touch.h"
#include "../Controller/OPRT_Gyro.h"
#include "Manager/PointWithDepth.h"
#include "Manager/GameManager.h"

USING_NS_CC;

Player::Player(const float& zdepth)
{
	/// 座標の初期化
	_localPos = {0,0,zdepth};

	/// 仮の画像を追加　◆
	// 中央
	auto sprite = Sprite::create("image/player/player_center_hit.png");
	sprite->setName("center");
	this->addChild(sprite);
	
	// 左上
	sprite = Sprite::create("image/player/player_left_up_hit.png");
	sprite->setAnchorPoint({ 1, 0 });
	sprite->setName("leftup");
	this->addChild(sprite);
	// 左下
	sprite = Sprite::create("image/player/player_left_down_hit.png");
	sprite->setAnchorPoint({ 1, 1 });
	sprite->setName("leftdown");
	this->addChild(sprite);

	// 右上
	sprite = Sprite::create("image/player/player_light_up_hit.png");
	sprite->setAnchorPoint({ 0, 0 });
	sprite->setName("rightup");
	this->addChild(sprite);

	// 右下
	sprite = Sprite::create("image/player/player_light_down_hit.png");
	sprite->setAnchorPoint({ 0, 1 });
	sprite->setName("rightdown");
	this->addChild(sprite);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 座標を真ん中にセット
	this->setPosition(Vec2(_localPos.x + visibleSize.width / 2, _localPos.y + visibleSize.height / 2));
	// 消失点の変更
	//lpPointWithDepth.SetVanishingPoint((-this->getPosition() + Vec2(_localPos.x + visibleSize.width, _localPos.y + visibleSize.height)));
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
	return _localPos.z;
}

cocos2d::Vec2 Player::GetAnchorPos(const std::string& name)
{
	/// アンカーポイントに設定してある座標を返している
	if (name == "center")
	{
		return this->getPosition();
	}
	else if (name == "leftup")
	{
		Size size = this->getChildByName(name)->getContentSize();
		return this->getPosition() + Vec2(-size.width / 2, size.height / 2);
	}
	else if (name == "rightup")
	{
		Size size = this->getChildByName(name)->getContentSize();
		return this->getPosition() + Vec2(size.width / 2, size.height / 2);
	}
	else if (name == "leftdown")
	{
		Size size = this->getChildByName(name)->getContentSize();
		return this->getPosition() + Vec2(-size.width / 2, -size.height / 2);
	}
	else if (name == "rightdown")
	{
		Size size = this->getChildByName(name)->getContentSize();
		return this->getPosition() + Vec2(size.width / 2, -size.height / 2);
	}
	else {}
	return Vec2::ZERO;
}

void Player::MoveUpdate()
{
	
	/// 画面サイズの取得
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Vec2 pos	 = _oprtState->GetPoint() - visibleSize / 2;
	Vec2 offset  = visibleSize / 2;

	/// サイズの取得
	float width, height;

	width  = (this->getChildByName("leftup")->getContentSize().width +
			  this->getChildByName("rightup")->getContentSize().width);
	height = (this->getChildByName("leftdown")->getContentSize().height +
			  this->getChildByName("rightdown")->getContentSize().height);

	Size size = { width, height };

	// 消失点の変更
	// lpPointWithDepth.SetVanishingPoint((-pos + Vec2(_localPos.x + visibleSize.width, _localPos.y + visibleSize.height)));

	/// X軸の移動範囲チェック
	if ((pos.x + offset.x + size.width / 2 < visibleSize.width) &&
		(pos.x + offset.x - size.width / 2 > 0))
	{
		_localPos.x = pos.x;
	}

	/// Y軸の移動範囲チェック
	if ((pos.y + offset.y + size.height / 2 < visibleSize.height) &&
		(pos.y + offset.y - size.height / 2 > 0))
	{
		_localPos.y = pos.y;
	}
	// 奥行きの深さによって、サイズを変更するようにしている
	setScale(lpPointWithDepth.GetScale(_localPos.z));
	
	/// ワールド座標に変換している
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
}

void Player::update(float dt)
{
	_oprtState->Update();
	MoveUpdate();
}
