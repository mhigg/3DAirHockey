#include "PointWithDepth.h"

std::unique_ptr<PointWithDepth, PointWithDepth::PointWithDepthDeleter> PointWithDepth::s_Instance(new PointWithDepth());	// ｲﾝｽﾀﾝｽ用ﾕﾆｰｸﾎﾟｲﾝﾀｰ

#define Z_START_POINT	(0)			// Z(奥行の始点)
#define Z_END_POINT		(1500)		// Z(奥行の終点)
#define X_VanishingPoint_MAX (1024)	// X方向に消失点を動かす最大値
#define Y_VanishingPoint_MAX (576)	// Y方向に消失点を動かす最大値

PointWithDepth::PointWithDepth()
{
	// 奥行の設定
	_zDepth = std::make_pair(Z_START_POINT, Z_END_POINT);

	// 消失点の設定(とりあえずで画面中央に設定)
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	// 初期位置
	_firstVanishingPoint = { 1024 / 2, 576 / 2 };
	_nowVanishingPoint = _firstVanishingPoint;

	// 消失点の移動範囲の下限上限
	_moveVanishingPoint = { 0,0,X_VanishingPoint_MAX,Y_VanishingPoint_MAX };
	// 大きさの初期化
	_scale = 0;

}

PointWithDepth::~PointWithDepth()
{
}

cocos2d::Point PointWithDepth::SetWorldPosition(cocos2d::Vec3 localPos, float scale)
{
	// ｽﾌﾟﾗｲﾄの大きさの設定
	GetScale(localPos.z);

	// XとYの座標計算
	PointNormalize(_nowVanishingPoint);

	cocos2d::Point pos;

	// 奥行の式
	pos.x = _firstVanishingPoint.x + (_scale - scale) * (_nowVanishingPoint.x * _normalizePoint.x);
	pos.y = _firstVanishingPoint.y + (_scale - scale) * (_nowVanishingPoint.y * _normalizePoint.y);

	// 座標の更新
	pos.x += (_scale) * (localPos.x);
	pos.y += (_scale) * (localPos.y);

	return pos;
}

float PointWithDepth::GetScale(float local_z)
{
	// ｽﾌﾟﾗｲﾄの大きさの設定(1〜0)まで
	_scale = (local_z - _zDepth.second) / (_zDepth.first - _zDepth.second);

	// ﾏｲﾅｽになったときは0に設定
	if (_scale < 0)
	{
		_scale = 0;
	}

	return _scale;
}

cocos2d::Point PointWithDepth::GetVanishingPoint(void)
{
	return _nowVanishingPoint;
}

void PointWithDepth::SetVanishingPoint(cocos2d::Point pos)
{
	auto tmpVanishingPoint = pos;

	if (tmpVanishingPoint.x < _moveVanishingPoint.origin.x + 1)
	{
		tmpVanishingPoint.x = _moveVanishingPoint.origin.x + 1;
	}
	if (tmpVanishingPoint.x > _moveVanishingPoint.size.width)
	{
		tmpVanishingPoint.x = _moveVanishingPoint.size.width;
	}
	if (tmpVanishingPoint.y < _moveVanishingPoint.origin.y + 1)
	{
		tmpVanishingPoint.y = _moveVanishingPoint.origin.y + 1;
	}
	if (tmpVanishingPoint.y > _moveVanishingPoint.size.height)
	{
		tmpVanishingPoint.y = _moveVanishingPoint.size.height;
	}
	_nowVanishingPoint = tmpVanishingPoint;
}

void PointWithDepth::ResetVanishingPoint(void)
{
	_nowVanishingPoint = _firstVanishingPoint;
}

void PointWithDepth::PointNormalize(cocos2d::Point vanishingPoint)
{
	// -1~1までの数字を作る
	_normalizePoint.x = ((_firstVanishingPoint.x / vanishingPoint.x) - 1);
	_normalizePoint.y = ((_firstVanishingPoint.y / vanishingPoint.y) - 1);
}
