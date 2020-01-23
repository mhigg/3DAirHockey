#pragma once
#include "cocos2d.h"
#include "../Manager/PointWithDepth.h"

class Obj :
	public cocos2d::Sprite
{
public:
	Obj();
	~Obj();
	cocos2d::Vec3 GetLocalPos(void) const;					// _localPosの取得
private:
	bool Init(void);
	virtual void update(float dt) = 0;

protected:
	cocos2d::Vec3 _localPos;	// ｹﾞｰﾑ上の座標(ｽｸﾘｰﾝに表示される座標とは違う)
	float _localSize;			// ｹﾞｰﾑ上のｽﾌﾟﾗｲﾄの大きさ(ｽｸﾘｰﾝに表示される大きさとは違う)
};