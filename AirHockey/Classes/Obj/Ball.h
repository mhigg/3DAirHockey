#pragma once
#include <vector>
#include <tuple>
#include "Obj.h"
#include "TrajectControl.h"

class Ball :
	public Obj
{
public:
	Ball(std::vector<float> depth);
	Ball();
	~Ball();

	cocos2d::Vec3 GetLocalPos(void) const;					// _localPosの取得
	std::tuple<bool, bool, bool> GetIsReverse() const;		// 反転用フラグの取得用
private:
	bool Init(void);
	void ChangeIsReverse();							// 反転用フラグの更新用
	void update(float dt) override;



	std::unique_ptr<TrajectControl> _traject;		// ボールの軌道制御用
	std::tuple<bool, bool, bool> _isReverse;		// 速度反転用のフラグ(0 : X, 1 : Y, 2 : Z)
	std::vector<float> _wallDepth;					// 壁の奥行

	float _radius;									// 球の半径

	State _ballState;								// ボールの状態保存用					

	
};
