#pragma once
#include "OPRT_State.h"

struct Oprt_Touch :
	public OPRT_State
{
public:
	Oprt_Touch();
	Oprt_Touch(cocos2d::Node* node);
	~Oprt_Touch();

	cocos2d::Vec2 GetPoint(void) const;
private:
	void Update(void);		// 状態更新
	// タッチ座標
	cocos2d::Vec2 _point;
};