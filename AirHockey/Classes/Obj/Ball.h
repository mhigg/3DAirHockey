#pragma once
#include "Obj.h"
#include <vector>
#include <tuple>

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
	void update(float dt) override;

	std::vector<float> _wallDepth;	// 壁の奥行
	float _radius;					// 玉の半径

	/// マジックナンバーの指定は後で修正しよう！　◆
	std::tuple<bool, bool, bool> _isReverse;		// 速度反転用の判定(0 : X, 1 : Y, 2 : Z)
};
