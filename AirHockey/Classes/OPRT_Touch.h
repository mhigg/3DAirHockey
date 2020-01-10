#pragma once
#include "OPRT_State.h"
#include <vector>

class OPRT_Touch :
	public OPRT_State
{
public:
	OPRT_Touch();
	OPRT_Touch(cocos2d::Node* sprite);
	~OPRT_Touch();

private:
	void Update(void);			// 状態更新用
	cocos2d::Vec2 GetPoint(void);
	void Init();				// ﾎﾞﾀﾝ登録用
	bool initOnceFlag = false;	// Initを一回だけ通すためのﾌﾗｸﾞ
	
};



