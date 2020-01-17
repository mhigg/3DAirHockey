#pragma once
#include "Obj.h"
#include <vector>
#include <tuple>

class TrajectControl;

class Ball :
	public Obj
{
public:
	Ball(std::vector<float> depth);
	Ball();
	~Ball();

	cocos2d::Vec3 GetLocalPos(void) const;		// _localPosの取得
	std::tuple<bool, bool, bool> GetIsReverse() const;
private:
	bool Init(void);
	void ChangeIsReverse();							// 反転用フラグの更新用
	void update(float dt) override;

	std::unique_ptr<TrajectControl> _traject;		// ボールの軌道制御用
	std::vector<float> _wallDepth;	// 壁の奥行
	float _radius;					// 玉の半径

	bool _isCurve = false;		// ベジェの端点生成用フラグ(デバッグ)

	/// マジックナンバーの指定は後で修正しよう！　◆
	std::tuple<bool, bool, bool> _isReverse;		// 速度反転用のフラグ(0 : X, 1 : Y, 2 : Z)
};
