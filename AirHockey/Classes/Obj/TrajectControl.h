﻿#pragma once
#include <array>
#include "cocos2d.h"

// ボールの軌道の状態
enum class State
{
	NORMAL,		// 通常
	CURVE,		// カーブ
	MAX
};

class Player;

/// ボールの軌道制御用クラス
class TrajectControl
{
public:
	TrajectControl();
	~TrajectControl();

	void ResetVel();								// 速度の再設定用
	void AccelSpeed();								// スピードの加速用
	void ChangeVel(const cocos2d::Vec2& vec);		// 速度変更用
	cocos2d::Vec3 GetVel(const State& state);		// 速度取得用
	bool CalBezierPoint(const cocos2d::Vec2& vec);
private:
	cocos2d::Vec3 CalNormalVel();
	cocos2d::Vec3 CalCurveVel();			// カーブする軌道の速度計算用 

	std::array<cocos2d::Vec3, 29> _points;	// 曲線の端点

	cocos2d::Vec3 _vel;						// ボールの速度
	cocos2d::Vec3 _speed;					// ボールのスピード
	const cocos2d::Vec3 _defSpeed;			// ボールの通常スピード
};

