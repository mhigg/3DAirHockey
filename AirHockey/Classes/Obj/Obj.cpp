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
	// Šî’ê‚Ìinit()‚É•K‚¸’Ê‚é‚æ‚¤‚É‚·‚é
	if (!cocos2d::Sprite::init())
	{
		return false;
	}

	// ‰Šú‰»
	_localPos = { 0,0,1 };
	_localSize = 1;

	return true;
}
