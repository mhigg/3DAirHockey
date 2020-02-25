#include "Player.h"
#include "../Controller/MouseCtl.h"
#include "../Controller/OPRT_Touch.h"
#include "../Controller/OPRT_Network.h"
#include "Manager/PointWithDepth.h"
#include "Manager/GameManager.h"

USING_NS_CC;

Player::Player(bool isHost, const float& zdepth)
{
	/// 座標の初期化
	_prePos	  = Vec2::ZERO;
	_localPos = { 0,0,zdepth };

	Init(zdepth);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// windows
	_oprtState.reset(new MouseCtl(this));
#else
	// android
	bool flag = false;
	_oprtState.reset(new Oprt_Touch(this, flag, isHost));
#endif

	this->scheduleUpdate();
}

Player::Player(bool isHost, const float& zdepth, int provIsFront)
{
	/// 座標の初期化
	_prePos	  = Vec2::ZERO;
	_localPos = { 0,0,zdepth };

	Init(zdepth);

	// プレイヤーの操作方法設定
	if (provIsFront == 0)
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		_oprtState.reset(new MouseCtl(this));
#else
		_oprtState.reset(new Oprt_Touch(this, false, isHost));
#endif
	}
	else
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

		// 奥側のマレットは通信
		_oprtState.reset(new Oprt_Touch(this, true, isHost));
#else
		_oprtState.reset(new OPRT_Network(this, isHost));
#endif
	}

	this->scheduleUpdate();
}

Player::~Player()
{
}

void Player::Init(const float & zdepth)
{
	/// 座標の初期化
	_prePos = Vec2::ZERO;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 座標を真ん中にセット
	this->setPosition(lpPointWithDepth.SetWorldPosition(_localPos));

	/// 表示時間の初期化
	_dispCnt = -1;

	/// プレイヤーの差分用画像の初期化
	InitTextureInfo();
}

void Player::InitTextureInfo()
{
	// 中央
	auto sprite = Sprite::create("image/player/player_center.png");
	sprite->setName("center");
	sprite->setTag(static_cast<int>(PL_ANC::CENTER));
	this->addChild(sprite);

	// 左上
	sprite = Sprite::create("image/player/player_leftup.png");
	sprite->setAnchorPoint({ 1, 0 });
	sprite->setName("leftup");
	sprite->setTag(static_cast<int>(PL_ANC::LEFTUP));
	this->addChild(sprite);

	// 左下
	sprite = Sprite::create("image/player/player_leftdown.png");
	sprite->setAnchorPoint({ 1, 1 });
	sprite->setName("leftdown");
	sprite->setTag(static_cast<int>(PL_ANC::LEFTDOWN));
	this->addChild(sprite);

	// 右上
	sprite = Sprite::create("image/player/player_rightup.png");
	sprite->setAnchorPoint({ 0, 0 });
	sprite->setName("rightup");
	sprite->setTag(static_cast<int>(PL_ANC::RIGHTUP));
	this->addChild(sprite);

	// 右下
	sprite = Sprite::create("image/player/player_rightdown.png");
	sprite->setAnchorPoint({ 0, 1 });
	sprite->setName("rightdown");
	sprite->setTag(static_cast<int>(PL_ANC::RIGHTDOWN));
	this->addChild(sprite);

	/// 画像のパス保存用
	std::string imagePath;

	/// 画像情報の登録
	for (auto anchor : this->getChildren())
	{
		/// ボールがヒットしていない時の画像登録
		imagePath = "image/player/player_" + anchor->getName() + ".png";
		_texInfo[anchor->getTag()].first = Director::getInstance()->getTextureCache()->addImage(imagePath);

		/// ボールがヒットした時の画像登録
		imagePath = "image/player/player_" + anchor->getName() + "_hit" + ".png";
		_texInfo[anchor->getTag()].second = Director::getInstance()->getTextureCache()->addImage(imagePath);
	}
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
		/// 真ん中
		return Vec2(_localPos.x, _localPos.y);
	}
	else if (name == "leftup")
	{
		/// 左上
		Size size = this->getChildByName(name)->getContentSize();
		return Vec2(_localPos.x, _localPos.y) + Vec2(-size.width / 2, size.height / 2);
	}
	else if (name == "rightup")
	{
		/// 右上
		Size size = this->getChildByName(name)->getContentSize();
		return Vec2(_localPos.x, _localPos.y) + Vec2(size.width / 2, size.height / 2);
	}
	else if (name == "leftdown")
	{
		/// 左下
		Size size = this->getChildByName(name)->getContentSize();
		return Vec2(_localPos.x, _localPos.y) + Vec2(-size.width / 2, -size.height / 2);
	}
	else if (name == "rightdown")
	{
		/// 右下
		Size size = this->getChildByName(name)->getContentSize();
		return Vec2(_localPos.x, _localPos.y) + Vec2(size.width / 2, -size.height / 2);
	}
	else {}

	return Vec2::ZERO;
}

cocos2d::Vec2 Player::GetMoveDistance() const
{
	return (Vec2(_localPos.x, _localPos.y) - _prePos);
}

void Player::ChangeImage(const int & num)
{
	/// ボールと当たった時の画像に変更している
	Sprite* sp = (Sprite*)this->getChildByTag(num);
	sp->setTexture(_texInfo[num].second);

	/// 画像を表示する時間
	_dispCnt = 40;
}

void Player::GyroSet(bool setFlag)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	auto touch = (Oprt_Touch*)_oprtState.get();
	touch->GyroActive(setFlag);
#endif
}

void Player::MoveUpdate()
{
	/// マレットの画像サイズの取得
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	/// プレイヤーの操作ごとに設定された座標の取得
	Vec2 pos	 = _oprtState->GetPoint() - visibleSize / 2;

	/// ゲーム画面のオフセット
	Vec2 offset  = visibleSize / 2;

	/// マレットの幅と高さの取得
	float width, height;
	width  = (this->getChildByName("leftup")->getContentSize().width +
			  this->getChildByName("rightup")->getContentSize().width);
	height = (this->getChildByName("leftdown")->getContentSize().height +
			  this->getChildByName("rightdown")->getContentSize().height);

	/// X軸の移動範囲チェック
	if ((pos.x + offset.x + width / 2 < visibleSize.width) &&
		(pos.x + offset.x - width / 2 > 0))
	{
		_localPos.x = pos.x;
	}

	/// Y軸の移動範囲チェック
	if ((pos.y + offset.y + height / 2 < visibleSize.height) &&
		(pos.y + offset.y - height / 2 > 0))
	{
		_localPos.y = pos.y;
	}

	// 奥行の深さによって、サイズ変更を行っている
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	/// ワールド座標に変換している
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
}

void Player::ResetTexture()
{
	Sprite* sp;
	for (auto anchor : this->getChildren())
	{
		/// ボールヒット時に変更した画像を元に戻す
		sp = (Sprite*)anchor;
		sp->setTexture(_texInfo[anchor->getTag()].first);
	}
	_dispCnt = -1;
}

void Player::update(float dt)
{
	_prePos = Vec2(_localPos.x, _localPos.y);
	_oprtState->Update();

	MoveUpdate();

	/// 表示時間が0になった時に入る
	if (_dispCnt == 0)
	{
		ResetTexture();
	}
	else
	{
		/// 表示時間の減算
		--_dispCnt;
	}
}
