#include "Obj.h"

Obj::Obj()
{
	Init();
}

Obj::~Obj()
{
}

bool Obj::Init(void)
{
	// ����init()�ɕK���ʂ�悤�ɂ���
	if (!cocos2d::Sprite::init())
	{
		return false;
	}

	return true;
}
