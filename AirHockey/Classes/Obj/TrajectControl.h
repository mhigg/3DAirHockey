#pragma once
#include <array>
#include "cocos2d.h"

// ボールの軌道の状態
enum class State
{
	NORMAL,
	CURVE,
	MAX
};

class Player;

/// ボールの軌道制御用クラス
class TrajectControl
{
public:
	TrajectControl();
	~TrajectControl();

	void AccelSpeed();								// スピード加算用
	void ResetVel();								// 速度の初期化用
	void SetVel(const cocos2d::Vec2& vec);
	cocos2d::Vec3 GetVel(const State& state);		// 速度取得用
	bool CalBezierPoint(const cocos2d::Vec2& vec);
private:
	cocos2d::Vec3 CalNormalVel();			// ボールを通常で跳ね返した時の速度取得用
	cocos2d::Vec3 CalCurveVel();			// カーブする軌道の速度計算用 

	std::array<cocos2d::Vec3, 29> _points;	// 曲線の端点(点の数によって、ベクトルを細かく設定できる)

	cocos2d::Vec3 _vel;						// 速度の設定
	cocos2d::Vec3 _speed;					// ボールのスピード
	const cocos2d::Vec3 _defSpeed;			// ボールの固定スピード
};

