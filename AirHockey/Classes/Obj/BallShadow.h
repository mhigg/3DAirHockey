#pragma once
#include "Obj.h"
#include <vector>

// �e�̈ʒu
enum SHADOW
{
	UP,		// ��
	DOWN,	// ��
	LEFT,	// ��
	RIGHT,	// �E
	MAX
};


class BallShadow :
	public Obj
{
public:
	BallShadow(int num, const std::string fileName);
	BallShadow();
	~BallShadow();
private:
	bool Init(void);
	bool Init(const std::string fileName);
	void update(float dt)override;	// ��ԍX�V

	SHADOW _shadowPlace;			// �㉺���E�̉e��enum�ۑ��p
};
