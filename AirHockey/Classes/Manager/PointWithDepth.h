#pragma once
#include "cocos2d.h"
#include <utility>

// 定義
#define lpPointWithDepth PointWithDepth::GetInstance()		// ｼﾝｸﾞﾙﾄﾝ

// using宣言
using Z_DEPTH		= std::pair<float, float>;	// 奥行の始点と終点
using VANISHIN_P	= std::pair<float, float>;	// 消失点のXとYの座標

class PointWithDepth
	: public cocos2d::Node
{
public:
	static PointWithDepth& GetInstance()	// ｱﾄﾞﾚｽが欲しいためﾎﾟｲﾝﾀｰか参照で
	{
		return (*s_Instance);				// *で中身(ｸﾗｽの実体)を返す
	}

	cocos2d::Point SetWorldPosition(cocos2d::Vec3 localPos);	// scaleと座標を1点透視図法で計算する
	float GetScale(float local_z);								// ｽﾌﾟﾗｲﾄ用のscareのｹﾞｯﾀｰ

	void SetVanishingPoint(cocos2d::Point pos);					// 消失点を動かしたいときに使うｾｯﾀｰ
	void ResetVanishingPoint(void);								// 動かした消失点を初期値に戻す

private:
	// 関数ｵﾌﾞｼﾞｪｸﾄ
	// ﾃﾞｽﾄﾗｸﾀを呼ぶ
	struct PointWithDepthDeleter
	{
		void operator()(PointWithDepth* pointWithDepth)const
		{
			delete pointWithDepth;
		}
	};
	PointWithDepth();
	~PointWithDepth();

	static std::unique_ptr<PointWithDepth, PointWithDepthDeleter> s_Instance;	// ｲﾝｽﾀﾝｽ用ﾕﾆｰｸﾎﾟｲﾝﾀｰ

	void PointNormalize(cocos2d::Point VanishingPoint);					// 
	Z_DEPTH _zDepth;							// 奥行の始点と終点
	cocos2d::Point _firstVanishingPoint;		// 初期の消失点のXとYの座標
	cocos2d::Point _nowVanishingPoint;			// 更新する消失点のXとYの座標
	cocos2d::Point _normalizePoint;				// 
	cocos2d::Point _magnification;				// 消失点を動かす倍率()
	cocos2d::Rect  _moveVanishingPoint;			// 消失点を動かせる範囲
	float _scale;								// ｽﾌﾟﾗｲﾄの大きさ
};