#include "SpriteAffectDepth.h"

SpriteAffectDepth::SpriteAffectDepth(cocos2d::Vec2 pos, float zDepth, const std::string & filename)
{
	Init();

	// 座標
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// ﾃｸｽﾁｬｰ
	setTexture(filename);

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();
}

SpriteAffectDepth::SpriteAffectDepth(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B color)
{
	Init();

	// ｽﾃｰｼﾞﾃｸｽﾁｬｰ(壁)のｻｲｽﾞ
	cocos2d::Rect deformationRect = { 0,0,size.x,size.y };
	// 画像
	setTextureRect(deformationRect);
	// 色の設定
	setColor(color);

	// 座標
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();
}

SpriteAffectDepth::SpriteAffectDepth()
{
	init();
}

SpriteAffectDepth::~SpriteAffectDepth()
{
}

bool SpriteAffectDepth::Init(void)
{
	// 基底のinit()に必ず通るようにする
	if (!cocos2d::Sprite::init())
	{
		return false;
	}
	return true;
}

void SpriteAffectDepth::update(float dt)
{
	// 座標の更新
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
}

cocos2d::Vec3 SpriteAffectDepth::GetLocalPos(void) const
{
	return _localPos;
}
