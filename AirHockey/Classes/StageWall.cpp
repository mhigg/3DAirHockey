#include "StageWall.h"
#include "OPRT_Key.h"

StageWall::StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Color3B* color)
{
	// ｽﾃｰｼﾞﾃｸｽﾁｬｰ(壁)のｻｲｽﾞ
	//cocos2d::Rect deformationRect = { 0,0,size.x,size.y };

	// 画像
	setTexture("wall.png");
	// ｱﾝｶｰﾎﾟｲﾝﾄの設定
	setAnchorPoint({ 0.5,0.5 });
	// 色
	setColor(*color);
	// 座標
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	_pointDepth = new PointWithDepth();
	_pointDepth->SetWorldPosition(_localPos.x, _localPos.y, _localPos.z);
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(*_pointDepth);
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(_pointDepth->GetScale());

	_state = new OPRT_Key(this);

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();
}

StageWall::StageWall()
{
}

StageWall::~StageWall()
{
}

void StageWall::update(float dt)
{
	_pointDepth->SetWorldPosition(_localPos.x, _localPos.y, _localPos.z);
	setPosition(*_pointDepth);
	setScale(_pointDepth->GetScale());

	if (_state->GetState(INPUT_ID::UP) <= TRIGGER_STATE::ON)
	{
		_pointDepth->SetVanishingPoint(cocos2d::Vec2{ 0, +1 });
	}
	if (_state->GetState(INPUT_ID::DOWN) <= TRIGGER_STATE::ON)
	{
		_pointDepth->SetVanishingPoint(cocos2d::Vec2{ 0, -1 });
	}
	if (_state->GetState(INPUT_ID::LEFT) <= TRIGGER_STATE::ON)
	{
		_pointDepth->SetVanishingPoint(cocos2d::Vec2{ -1, 0 });
	}
	if (_state->GetState(INPUT_ID::RIGHT) <= TRIGGER_STATE::ON)
	{
		_pointDepth->SetVanishingPoint(cocos2d::Vec2{ +1, 0 });
	}
}
