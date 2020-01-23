#include "Obj.h"

Obj::Obj()
{
	Init();
}

Obj::~Obj()
{
}

cocos2d::Vec3 Obj::GetLocalPos(void) const
{
	return _localPos;
}

bool Obj::Init(void)
{
	// ����init()�ɕK���ʂ�悤�ɂ���
	if (!cocos2d::Sprite::init())
	{
		return false;
	}

	// ������
	_localPos = { 0,0,1 };
	_localSize = 1;

	return true;
}
