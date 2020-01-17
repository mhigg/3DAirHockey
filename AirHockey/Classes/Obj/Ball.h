#pragma once
#include "Obj.h"
#include <vector>
#include <tuple>

class TrajectControl;

class Ball :
	public Obj
{
public:
	Ball(std::vector<float> depth);
	Ball();
	~Ball();

	cocos2d::Vec3 GetLocalPos(void) const;		// _localPos�̎擾
	std::tuple<bool, bool, bool> GetIsReverse() const;
private:
	bool Init(void);
	void ChangeIsReverse();							// ���]�p�t���O�̍X�V�p
	void update(float dt) override;

	std::unique_ptr<TrajectControl> _traject;		// �{�[���̋O������p
	std::vector<float> _wallDepth;	// �ǂ̉��s
	float _radius;					// �ʂ̔��a

	bool _isCurve = false;		// �x�W�F�̒[�_�����p�t���O(�f�o�b�O)

	/// �}�W�b�N�i���o�[�̎w��͌�ŏC�����悤�I�@��
	std::tuple<bool, bool, bool> _isReverse;		// ���x���]�p�̃t���O(0 : X, 1 : Y, 2 : Z)
};
