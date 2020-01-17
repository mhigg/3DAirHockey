#pragma once
#include <vector>
#include "cocos2d.h"

// ボールの軌道の状態
enum class State
{
	NORMAL,
	CURVE,
	MAX
};

/// ボールの軌道制御用クラス
class TrajectControl
{
public:
	TrajectControl();
	~TrajectControl();

	cocos2d::Vec3 GetVel(State& state);		// 速度取得用
	void CalBezierPoint();				
private:
	cocos2d::Vec3 CalCurveVel();			// カーブする軌道の速度計算用 
	const cocos2d::Vec3 _vel;	
	std::vector<cocos2d::Vec3> _points;		// 曲線の端点(とりあえず、可変長) ◆
};

