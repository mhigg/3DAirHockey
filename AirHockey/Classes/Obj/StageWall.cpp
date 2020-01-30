#include "StageWall.h"
#include "Manager/GameManager.h"

USING_NS_CC;

StageWall::StageWall(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B color)
{
	// ｽﾃｰｼﾞﾃｸｽﾁｬｰ(壁)のｻｲｽﾞ
	cocos2d::Rect deformationRect = { 0,0,size.x,size.y };
	// 画像
	//setTextureRect(deformationRect);
	setTexture("image/wall.png");
	// ｱﾝｶｰﾎﾟｲﾝﾄの設定
	setAnchorPoint({ 0.5,0.5 });
	// 色の設定
// 初期
	_normalColor = cocos2d::Color3B(0, 255, 0);
	setColor(_normalColor);
	// 現在の色
	_nowColor = getColor();
	// 変更した際の色
	_changeColor = cocos2d::Color3B(0, 0, 255);
	// 色の変更ﾌﾗｸﾞ
	_colorChangeFlag = false;

	// 座標
	_localPos = cocos2d::Vec3{ pos.x,pos.y,zDepth };

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// 画面解像度の取得
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	// 消失点までの線の描画
	auto line = cocos2d::DrawNode::create();
	// 線の太さ(cocosは半径になる)
	float radius = 2.0f;
	//drawSegment(始点,終点,太さ,色);
	// 左上
	line->drawSegment(cocos2d::Vec2(0, size.y), lpPointWithDepth.GetVanishingPoint(), radius, cocos2d::Color4F(0, 1.0f, 0, 1.0f));
	// 左下
	line->drawSegment(cocos2d::Vec2(0, 0), lpPointWithDepth.GetVanishingPoint(), radius, cocos2d::Color4F(0, 1.0f, 0, 1.0f));
	// 右上
	line->drawSegment(cocos2d::Vec2(size.x, size.y), lpPointWithDepth.GetVanishingPoint(), radius, cocos2d::Color4F(0, 1.0f, 0, 1.0f));
	// 右下
	line->drawSegment(cocos2d::Vec2(size.x, 0), lpPointWithDepth.GetVanishingPoint(), radius, cocos2d::Color4F(0, 1.0f, 0, 1.0f));
	addChild(line);

	// 1ﾌﾚｰﾑごとにupdateを
	cocos2d::Node::scheduleUpdate();

}

StageWall::StageWall(float zDepth, int num)
{
	// 画像
	setTexture("image/wall.png");

	// ｱﾝｶｰﾎﾟｲﾝﾄの設定
	setAnchorPoint({ 0.5,0.5 });
	// 座標
	_localPos = Vec3{ 0,0,zDepth };

	// posとｽﾌﾟﾗｲﾄの大きさを一点透視図法に置き換える
	// 一点透視図法にした時の座標のｾｯﾄ
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));
	// 一点透視図法にした時の画像のｻｲｽﾞ設定
	setScale(lpPointWithDepth.GetScale(_localPos.z));

	// 色の設定
	// 初期
	_normalColor = Color3B(0, 255, 0);
	setColor(_normalColor);
	// 現在の色
	_nowColor = getColor();
	// 変更した際の色
	_changeColor = Color3B(0, 0, 255);
	// 色の変更ﾌﾗｸﾞ
	_colorChangeFlag = false;

	// 線
	//// 画面解像度の取得
	//auto visibleSize = Director::getInstance()->getVisibleSize();
	//// 画面の大きさの取得
	//auto size = getContentSize();
	//// 消失点までの線の描画
	//auto line = DrawNode::create();
	//// 線の太さ(cocosは半径になる)
	//float radius = 2.0f;
	//// 色
	//auto lineColor = Color4F{ _nowColor,1.0f };
	////drawSegment(始点,終点,太さ,色);
	//// 左上
	//line->drawSegment(Vec2(0, size.height), lpPointWithDepth.GetVanishingPoint(), radius, lineColor);
	//// 左下
	//line->drawSegment(Vec2(0, 0), lpPointWithDepth.GetVanishingPoint(), radius, lineColor);
	//// 右上
	//line->drawSegment(Vec2(size.width, size.height), lpPointWithDepth.GetVanishingPoint(), radius, lineColor);
	//// 右下
	//line->drawSegment(Vec2(size.width, 0), lpPointWithDepth.GetVanishingPoint(), radius, lineColor);
	//addChild(line);

	// 自分の壁の番号
	_wallNum = num;

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
	setPosition(lpPointWithDepth.SetWorldPosition(_localPos));

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

	if (_wallNum ==0)
	{
		/// 次の壁を取得
		auto layer = (GameManager*)Director::getInstance()->getRunningScene()->getChildByName("StageLayer");
		auto nextWall = (SpriteAffectDepth*)layer->getChildByName("Wall" + std::to_string(_wallNum +29 ));

		// 子の要素全削除(DrawLine)
		removeAllChildrenWithCleanup(true);

		// 消失点までの描画
		auto line = DrawNode::create();
		// 画像のｻｲｽﾞ取得
		auto size = getContentSize();
		// 次の壁のPos
		auto nextWallPos = nextWall->getPosition();
		// 次の壁の大きさ
		auto nextWallScale =nextWall->getScale();
		// 線の太さ(cocosは半径になる)
		float radius = 2.0f;
		// 色
		auto lineColor = Color4F::WHITE;
		// 左上
		line->drawSegment(Vec2(0, size.height),
			Vec2((nextWallPos.x - ((size.width / 2) * nextWallScale)), (nextWallPos.y + ((size.height / 2) * nextWallScale))),
				radius, lineColor);
		// 左下
		line->drawSegment(Vec2(0, 0),
			Vec2((nextWallPos.x - ((size.width / 2) * nextWallScale)), (nextWallPos.y - ((size.height / 2) * nextWallScale))),
			radius, lineColor);
		// 右上
		line->drawSegment(Vec2(size.width,size.height),
			Vec2((nextWallPos.x + ((size.width / 2) * nextWallScale)), (nextWallPos.y + ((size.height / 2) * nextWallScale))),
			radius, lineColor);
		// 右下
		line->drawSegment(Vec2(size.width, 0),
			Vec2((nextWallPos.x + ((size.width / 2) * nextWallScale)), (nextWallPos.y - ((size.height / 2) * nextWallScale))),
			radius, lineColor);

		//// 左上
		//line->drawSegment(
		//	Vec2(0, size.height),
		//	Vec2(-(a  - b), size.height -((getPosition().y + (size.height / 2)* getScale())-(nextWallPos.y + (size.height / 2)* nextWallScale))),
		//	radius, lineColor);
		//// 左下
		//line->drawSegment(
		//	Vec2(0, 0),
		//	Vec2(nextWallPos.x - (size.width / 2)* nextWallScale, nextWallPos.y - (size.height / 2)* nextWallScale),
		//	radius, lineColor);
		//// 右上
		//line->drawSegment(
		//	Vec2(size.width, size.height),
		//	Vec2(nextWallPos.x + (size.width / 2)* nextWallScale, nextWallPos.y + (size.height / 2)* nextWallScale),
		//	radius, lineColor);
		//// 右下
		//line->drawSegment(
		//	Vec2(size.width, 0),
		//	Vec2(nextWallPos.x + (size.width / 2)* nextWallScale, nextWallPos.y - (size.height / 2)* nextWallScale),
		//	radius, lineColor);
		line->setGlobalZOrder(this->getGlobalZOrder()+1);
		addChild(line);
	}

}

void StageWall::WallColor(cocos2d::Color3B color)
{
	// 現在の色の更新
	_nowColor = color;
	// 色変更
	setColor(_nowColor);
}
