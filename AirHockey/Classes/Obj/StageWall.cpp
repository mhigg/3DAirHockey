#include "StageWall.h"

StageWall::StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B color)
{
	// ｽﾃｰｼﾞﾃｸｽﾁｬｰ(壁)のｻｲｽﾞ
	cocos2d::Rect deformationRect = { 0,0,size.x,size.y };
	// 画像
	setTextureRect(deformationRect);
	// ｱﾝｶｰﾎﾟｲﾝﾄの設定
	setAnchorPoint({ 0.5,0.5 });
	// 色の設定
	_normalColor = color;
	setColor(_normalColor);
	// 現在の色
	_nowColor = getColor();
	// 変更した際の色
	_changeColor = cocos2d::Color3B(
		0,
		191 - (191 * (zDepth / 1000)),
		255 - (255 * (zDepth / 1000)) );
	// 色の変更ﾌﾗｸﾞ
	_colorChangeFlag = false;

	// 座標
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();

}

StageWall::StageWall()
{
}

StageWall::~StageWall()
{
}

void StageWall::SetWallColorChangeFlag(bool flag)
{
	_colorChangeFlag = flag;
}

void StageWall::update(float dt)
{
	// 座標の更新
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));

	// 色の変更
	if (_colorChangeFlag && _nowColor != _changeColor)
	{
		// ﾎﾞｰﾙ通過時の色に変更
		WallColor(_changeColor);
	}
	else if (!_colorChangeFlag && _nowColor != _normalColor)
	{
		// 通常時の色
		WallColor(_normalColor);
	}
	else{}
}

void StageWall::WallColor(cocos2d::Color3B color)
{
	// 現在の色の更新
	_nowColor = color;
	// 色変更
	setColor(_nowColor);
}
