#pragma once

#include <memory>
#include "cocos2d.h"

class Collision
{
public:

	static Collision& GetInstance()
	{
		return *s_Instance;
	}
	// •¨‘Ì“¯Žm‚Ì“–‚½‚è”»’è(2D)
	bool HitCollision2D(const cocos2d::Vec2& aPos, const cocos2d::Size& aSize,
						const cocos2d::Vec2& bPos, const cocos2d::Size& bSize) const;


private:
	Collision();
	~Collision();

	struct ColDeleter
	{
		void operator()(Collision* pointer)
		{
			delete pointer;
		}
	};


	static std::unique_ptr<Collision, ColDeleter> s_Instance;
};

