#pragma once
#include <array>
#include "Obj.h"

/// �{�[���̎c�������p�N���X
class BallAfter :
	public Obj
{
public:
	BallAfter();
	~BallAfter();

	/// ���W�X�V�p(�{�[���̃��[�J�����W���擾����)
	void Update(const cocos2d::Vec3& lPos);
private:
	void Init();
	void update(float dt);

	std::array<cocos2d::Vec3, 30> _points;
	std::array<cocos2d::Sprite*, 30> _images;
};

