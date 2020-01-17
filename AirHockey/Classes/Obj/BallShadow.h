#pragma once
#include "Obj.h"
#include <vector>

// âeÇÃà íu
enum SHADOW
{
	UP,		// è„
	DOWN,	// â∫
	LEFT,	// ç∂
	RIGHT,	// âE
	MAX
};


class BallShadow :
	public Obj
{
public:
	BallShadow(int num);
	BallShadow();
	~BallShadow();
private:
	bool Init(void);
	void update(float dt)override;

	SHADOW _shadowPlace;
};
