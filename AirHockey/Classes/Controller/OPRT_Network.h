#pragma once
#include "OPRT_State.h"

class OPRT_Network :
	public OPRT_State
{
public:
	OPRT_Network();
	~OPRT_Network();

	void Update(void);
	cocos2d::Vec2 GetPoint(void) const;


};
