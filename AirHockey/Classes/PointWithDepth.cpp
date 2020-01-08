#include "PointWithDepth.h"

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
	_firstVanishingPoint = { visibleSize.width / 2, visibleSize.height / 2 };
	_nowVanishingPoint = _firstVanishingPoint;

	// �����_�𓮂����{��
	_magnification = { X_MAGNIFICATION, Y_MAGNIFICATION };

	// �傫���̏�����
	_scale = 0;

}

PointWithDepth::~PointWithDepth()
{
}

void PointWithDepth::SetWorldPosition(float local_x, float local_y, float local_z)
{
	// ���ײĂ̑傫���̐ݒ�(1�`0)�܂�
	_scale = (local_z - _zDepth.second) / (_zDepth.first - _zDepth.second);

	// X��Y�̍��W�v�Z
	PointNormalize(_nowVanishingPoint);
	this->x = _firstVanishingPoint.x + (_scale - 1) * ((local_x + _nowVanishingPoint.x * _magnification.x) * _normalizePoint.x);
	this->y = _firstVanishingPoint.y + (_scale - 1) * ((local_y + _nowVanishingPoint.y * _magnification.y) * _normalizePoint.y);
}

float PointWithDepth::GetScale(void)
{
	return _scale;
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
