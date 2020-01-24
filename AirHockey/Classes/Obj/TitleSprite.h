#pragma once
#include "Obj.h"

class TitleSprite :
	public Obj
{
public:
	// (座標,奥行,ﾃｸｽﾁｬｰの大きさ,色)
	TitleSprite(cocos2d::Vec2 pos, float zDepth, const std::string &filename);
	TitleSprite();
	~TitleSprite();
	
private:
	bool Init(void);
	void update(float dt) override;

protected:
	cocos2d::Vec3 _localPos;	// ｹﾞｰﾑ上の座標(ｽｸﾘｰﾝに表示される座標とは違う)
	float _localSize;			// ｹﾞｰﾑ上のｽﾌﾟﾗｲﾄの大きさ(ｽｸﾘｰﾝに表示される大きさとは違う)
};
