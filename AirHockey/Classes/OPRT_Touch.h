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
	void Update(void);			// ��ԍX�V�p
	cocos2d::Vec2 GetPoint(void);
	void Init();				// ���ݓo�^�p
	bool initOnceFlag = false;	// Init����񂾂��ʂ����߂��׸�
	
};



