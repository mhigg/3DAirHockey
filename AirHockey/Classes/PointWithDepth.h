#pragma once
#include "cocos2d.h"
#include <utility>

// using宣言
using Z_DEPTH		= std::pair<float, float>;	// 奥行の始点と終点
using VANISHIN_P	= std::pair<float, float>;	// 消失点のXとYの座標

class PointWithDepth
	:public cocos2d::Point, public cocos2d::Node
{
public:
	PointWithDepth();
	~PointWithDepth();

	void SetWorldPosition(float local_x, float local_y, float local_z);	// scaleと座標を1点透視図法で計算する
	float GetScale(void);												// ｽﾌﾟﾗｲﾄ用のscareのｹﾞｯﾀｰ

	void SetVanishingPoint(cocos2d::Point pos);							// 消失点を動かしたいときに使うｾｯﾀｰ

private:
	void PointNormalize(cocos2d::Point VanishingPoint);					// 
	Z_DEPTH _zDepth;						// 奥行の始点と終点
	cocos2d::Point _firstVanishingPoint;	// 初期の消失点のXとYの座標
	cocos2d::Point _nowVanishingPoint;		// 更新する消失点のXとYの座標
	cocos2d::Point _normalizePoint;			// 
	cocos2d::Point _magnification;			// 消失点を動かす倍率()

	float _scale;														// ｽﾌﾟﾗｲﾄの大きさ
};