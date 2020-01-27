#pragma once
#include "cocos2d.h"
#include "../Manager/PointWithDepth.h"

// ｽﾌﾟﾗｲﾄを1点透視図法に対応させるのに使うクラス
class SpriteAffectDepth :
	public cocos2d::Sprite
{
public:
	// (座標,奥行,ﾃｸｽﾁｬｰのﾊﾟｽ)
	SpriteAffectDepth(cocos2d::Vec2 pos, float zDepth, const std::string &filename);
	// (座標,奥行,ﾃｸｽﾁｬｰの大きさ,色)
	SpriteAffectDepth(cocos2d::Vec2 pos, float zDepth, cocos2d::Point size, cocos2d::Color3B color);
	SpriteAffectDepth();
	~SpriteAffectDepth();

	virtual void update(float dt);

	cocos2d::Vec3 GetLocalPos(void) const;					// _localPosの取得

private:
	bool Init(void);

protected:
	cocos2d::Vec3 _localPos;	// ｹﾞｰﾑ上の座標(ｽｸﾘｰﾝに表示される座標とは違う)
	float _localSize;			// ｹﾞｰﾑ上のｽﾌﾟﾗｲﾄの大きさ(ｽｸﾘｰﾝに表示される大きさとは違う)
};

