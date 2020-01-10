#pragma once
#include "Obj.h"
#include <vector>
#include "OPRT_State.h"
#include "EffectMng.h"

class Ball :
	public Obj
{
public:
	Ball(std::vector<float> depth);
	Ball();
	~Ball();
private:
	bool Init(void);
	void update(float dt) override;

	std::vector<float> _wallDepth;	// �ǂ̉��s
	float _radius;					// �ʂ̔��a
	
	bool xReverse = false;
	bool yReverse = false;
	bool zReverse = false;
	
	OPRT_State* _state;
};
