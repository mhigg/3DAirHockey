#pragma once
#include "SpriteAffectDepth.h"

class StageWall :
	public SpriteAffectDepth
{
public:
	// (座標,奥行,ﾃｸｽﾁｬｰの大きさ,色)
	StageWall(cocos2d::Vec2 pos, float zDepth,cocos2d::Point size, cocos2d::Color3B color);
	// (座標,奥行,壁の番号)
	StageWall(float zDepth, int num ,bool lineFlag = true);
	StageWall();
	~StageWall();

	void SetWallColorChangeFlag(bool flag);	// 色を変えるﾌﾗｸﾞのｾｯﾀｰ(Ballｸﾗｽで使用)
private:
	void update(float dt)override;
	void WallColor(cocos2d::Color3B color);	// 壁の色の変更

	cocos2d::Color3B _nowColor;				// 現在の壁の色
	cocos2d::Color3B _normalColor;			// 通常の壁の色
	cocos2d::Color3B _changeColor;			// ﾎﾞｰﾙ通過時に変わる壁の色
	bool _colorChangeFlag;					// 色を変えるﾌﾗｸﾞ(trueで_changeColor,falseで_normalColor)

	int _wallNum;							// 壁の番号(手前が0番目で1ずつ増えている)
};