#include "StageWall.h"

StageWall::StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B * color)
{
	// ｽﾃｰｼﾞﾃｸｽﾁｬｰ(壁)のｻｲｽﾞ
	cocos2d::Rect deformationRect = { 0,0,size.x,size.y };
	// 画像
	setTextureRect(deformationRect);
	// ｱﾝｶｰﾎﾟｲﾝﾄの設定
	setAnchorPoint({ 0.5,0.5 });
	// 色
	setColor(*color);
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

void StageWall::update(float dt)
{
	setPosition(lpPointWithDepth.GetInstance().SetWorldPosition(_localPos));
	setScale(lpPointWithDepth.GetInstance().GetScale(_localPos.z));

}
