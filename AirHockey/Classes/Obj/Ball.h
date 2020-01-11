#pragma once
#include "Obj.h"
#include <vector>
#include "../Controller/OPRT_State.h"
#include "../Effekseer/EffectMng.h"

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

	std::vector<float> _wallDepth;	// ï«ÇÃâúçs
	float _radius;					// ã ÇÃîºåa
	
	bool xReverse = false;
	bool yReverse = false;
	bool zReverse = false;
	
	OPRT_State* _state;
};
