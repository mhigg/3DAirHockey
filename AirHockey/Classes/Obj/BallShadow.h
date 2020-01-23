#pragma once
#include "Obj.h"
#include <vector>

// 影の位置
enum SHADOW
{
	UP,		// 上
	DOWN,	// 下
	LEFT,	// 左
	RIGHT,	// 右
	MAX
};


class BallShadow :
	public Obj
{
public:
	BallShadow(int num, const std::string fileName);
	BallShadow();
	~BallShadow();
private:
	bool Init(void);
	bool Init(const std::string fileName);
	void update(float dt)override;	// 状態更新

	SHADOW _shadowPlace;			// 上下左右の影のenum保存用
};
