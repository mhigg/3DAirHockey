#include "Ball.h"
#include "OPRT_Key.h"

Ball::Ball(std::vector<float> depth)
{
	_wallDepth = depth;
	Init();
}

Ball::Ball()
{
	Init();
}

Ball::~Ball()
{
}

void Ball::update(float dt)
{
	if (_localPos.z > _wallDepth[29])
	{
		zReverse = true;
	}
	else if (_localPos.z < _wallDepth[0])
	{
		zReverse = false;
	}

	if (!zReverse)
	{
		_localPos.z+=3;
	}
	else
	{
		_localPos.z-=3;
	}
	
	if (_localPos.x - _radius < -350)
	{
		xReverse = false;
	}
	else if (_localPos.x + _radius > +350)
	{
		xReverse = true;
	}

	if (!xReverse)
	{
		_localPos.x += 2;
	}
	else
	{
		_localPos.x -= 2;
	}
	
	if (_localPos.y - _radius < -250)
	{
		yReverse = false;
	}
	else if (_localPos.y + _radius > +250)
	{
		yReverse = true;
	}

	if (!yReverse)
	{
		_localPos.y += 2;
	}
	else
	{
		_localPos.y -= 2;
	}

	// 座標の更新
	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));


	if (_state->GetState(INPUT_ID::UP) <= TRIGGER_STATE::ON)
	{
		lpPointWithDepth.GetInstance().SetVanishingPoint(cocos2d::Vec2{ 0, +1 });
	}
	if (_state->GetState(INPUT_ID::DOWN) <= TRIGGER_STATE::ON)
	{
		lpPointWithDepth.GetInstance().SetVanishingPoint(cocos2d::Vec2{ 0, -1 });
	}
	if (_state->GetState(INPUT_ID::LEFT) <= TRIGGER_STATE::ON)
	{
		lpPointWithDepth.GetInstance().SetVanishingPoint(cocos2d::Vec2{ -1, 0 });
	}
	if (_state->GetState(INPUT_ID::RIGHT) <= TRIGGER_STATE::ON)
	{
		lpPointWithDepth.GetInstance().SetVanishingPoint(cocos2d::Vec2{ +1, 0 });
	}


}

bool Ball::Init(void)
{
	// 初期座標
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_localPos = { 0,0,_wallDepth[0]};
	// ﾃｸｽﾁｬｰ
	setTexture("ball.png");
	// 半径(画像の大きさ/2)
	_radius = getContentSize().width / 2;

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

	_state = new OPRT_Key(this);

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();

	return true;
}
