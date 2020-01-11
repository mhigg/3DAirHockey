#pragma once
#include "OPRT_State.h"

struct Input_Touch :
	public OPRT_State
{
public:
	Input_Touch();
	Input_Touch(cocos2d::Node* sprite);
	~Input_Touch();

private:
	void Update(void);		// 状態更新
	cocos2d::Vec2 GetPoint(void);
	// タッチ座標
	cocos2d::Vec2 _point;
};