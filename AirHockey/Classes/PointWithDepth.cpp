#include "PointWithDepth.h"

std::unique_ptr<PointWithDepth, PointWithDepth::PointWithDepthDeleter> PointWithDepth::s_Instance(new PointWithDepth());	// �ݽ�ݽ�p�ư��߲���

#define Z_START_POINT	(0)		// Z(���s�̎n�_)
#define Z_END_POINT		(1200)	// Z(���s�̏I�_)
#define X_MAGNIFICATION (3.0f)		// X�̏����_�𓮂����{��
#define Y_MAGNIFICATION (3.0f)		// Y�̏����_�𓮂����{��

PointWithDepth::PointWithDepth()
{
	// ���s�̐ݒ�
	_zDepth = std::make_pair(Z_START_POINT, Z_END_POINT);

	// �����_�̐ݒ�(�Ƃ肠�����ŉ�ʒ����ɐݒ�)
	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	_firstVanishingPoint = { 1024 / 2, 576 / 2 };
	_nowVanishingPoint = _firstVanishingPoint;

	// �����_�𓮂����{��
	_magnification = { X_MAGNIFICATION, Y_MAGNIFICATION };

	// �傫���̏�����
	_scale = 0;

}

PointWithDepth::~PointWithDepth()
{
}

cocos2d::Point PointWithDepth::SetWorldPosition(cocos2d::Vec3 localPos)
{
	// ���ײĂ̑傫���̐ݒ�
	GetScale(localPos.z);

	// X��Y�̍��W�v�Z
	PointNormalize(_nowVanishingPoint);

	cocos2d::Point pos;

	// ���s�̎�
	pos.x = _firstVanishingPoint.x + (_scale - 1) * (_nowVanishingPoint.x * _magnification.x * _normalizePoint.x);
	pos.y = _firstVanishingPoint.y + (_scale - 1) * (_nowVanishingPoint.y * _magnification.y * _normalizePoint.y);

	// ���W�̍X�V
	pos.x += (_scale) * (localPos.x /** _magnification.x * _normalizePoint.x*/);
	pos.y += (_scale) * (localPos.y/* * _magnification.y * _normalizePoint.y*/);

	return pos;
}

float PointWithDepth::GetScale(float local_z)
{
	// ���ײĂ̑傫���̐ݒ�(1�`0)�܂�
	return _scale = (local_z - _zDepth.second) / (_zDepth.first - _zDepth.second);;
}

void PointWithDepth::SetVanishingPoint(cocos2d::Point pos)
{
	_nowVanishingPoint = _nowVanishingPoint + pos;
}

void PointWithDepth::PointNormalize(cocos2d::Point vanishingPoint)
{
	// -1~1�܂ł̐��������
	_normalizePoint.x = -((vanishingPoint.x / _firstVanishingPoint.x) - 1);
	_normalizePoint.y = -((vanishingPoint.y / _firstVanishingPoint.y) - 1);
}
