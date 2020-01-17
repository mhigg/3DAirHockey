#include <cmath>
#include "Collision.h"

std::unique_ptr<Collision, Collision::ColDeleter> Collision::s_Instance(new Collision());

Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::HitCollision2D(const cocos2d::Vec2 & aPos, const cocos2d::Size & aSize,
							   const cocos2d::Vec2 & bPos, const cocos2d::Size & bSize) const
{
	return (abs(aPos.x - bPos.x) < (aSize.width / 2)  + (bSize.width / 2)) &&
		   (abs(aPos.y - bPos.y) < (aSize.height / 2) + (bSize.height / 2));
}
