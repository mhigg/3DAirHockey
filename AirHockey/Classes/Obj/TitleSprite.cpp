#include "TitleSprite.h"

TitleSprite::TitleSprite(cocos2d::Vec2 pos, float zDepth, const std::string &filename)
{
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

TitleSprite::TitleSprite()
{
}

TitleSprite::~TitleSprite()
{
}

bool TitleSprite::Init(void)
{
	return false;
}

void TitleSprite::update(float dt)
{
	// 座標の更新
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
}
