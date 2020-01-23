#pragma once
#include "cocos2d.h"
#include "../Manager/PointWithDepth.h"

class Obj :
	public cocos2d::Sprite
{
public:
	Obj();
	~Obj();
	cocos2d::Vec3 GetLocalPos(void) const;					// _localPos�̎擾
private:
	bool Init(void);
	virtual void update(float dt) = 0;

protected:
	cocos2d::Vec3 _localPos;	// �ްя�̍��W(��ذ݂ɕ\���������W�Ƃ͈Ⴄ)
	float _localSize;			// �ްя�̽��ײĂ̑傫��(��ذ݂ɕ\�������傫���Ƃ͈Ⴄ)
};