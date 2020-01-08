#include "PointWithDepth.h"

#define Z_START_POINT	(0)		// Z(奥行の始点)
#define Z_END_POINT		(1200)	// Z(奥行の終点)
#define X_MAGNIFICATION (3.0f)		// Xの消失点を動かす倍率
#define Y_MAGNIFICATION (3.0f)		// Yの消失点を動かす倍率

PointWithDepth::PointWithDepth()
{
	// 奥行の設定
	_zDepth = std::make_pair(Z_START_POINT, Z_END_POINT);

	// 消失点の設定(とりあえずで画面中央に設定)
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_firstVanishingPoint = { visibleSize.width / 2, visibleSize.height / 2 };
	_nowVanishingPoint = _firstVanishingPoint;

	// 消失点を動かす倍率
	_magnification = { X_MAGNIFICATION, Y_MAGNIFICATION };

	// 大きさの初期化
	_scale = 0;

}

PointWithDepth::~PointWithDepth()
{
}

void PointWithDepth::SetWorldPosition(float local_x, float local_y, float local_z)
{
	// ｽﾌﾟﾗｲﾄの大きさの設定(1～0)まで
	_scale = (local_z - _zDepth.second) / (_zDepth.first - _zDepth.second);

	// XとYの座標計算
	PointNormalize(_nowVanishingPoint);
	this->x = _firstVanishingPoint.x + (_scale - 1) * ((local_x + _nowVanishingPoint.x * _magnification.x) * _normalizePoint.x);
	this->y = _firstVanishingPoint.y + (_scale - 1) * ((local_y + _nowVanishingPoint.y * _magnification.y) * _normalizePoint.y);
}

float PointWithDepth::GetScale(void)
{
	return _scale;
}

void PointWithDepth::SetVanishingPoint(cocos2d::Point pos)
{
	_nowVanishingPoint = _nowVanishingPoint + pos;
}

void PointWithDepth::PointNormalize(cocos2d::Point vanishingPoint)
{
	// -1~1までの数字を作る
	_normalizePoint.x = -((vanishingPoint.x / _firstVanishingPoint.x) - 1);
	_normalizePoint.y = -((vanishingPoint.y / _firstVanishingPoint.y) - 1);
}
